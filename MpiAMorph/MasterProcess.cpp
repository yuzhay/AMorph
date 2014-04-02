#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include "dirent.h"
#include <direct.h>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;

//Массив состояний процессов
ProcessState *NodeStates;

//Время старта
clock_t start;

queue<unsigned long> *tasks;

void MPState1();
void MPState2();
void MPState3(int);


//Номер текущего процесса
int myNode;

//Количество процессов
int totalNodes; 


//Управляющий процесс
void MasterProcess(char *srcFile,char *dstFile, int totalNodes)
{
	tasks = new queue<unsigned long>();

	queue<unsigned long *> q;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	cout << "Total nodes: " << totalNodes << endl;

	NodeStates = (ProcessState*)calloc(totalNodes,sizeof(ProcessState));

	PrepareSolutionDir();

	//Синхронизация процессов
	MPI_Barrier(MPI_COMM_WORLD);

	//Загрузка исходного изображения
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//Загрузка конечного изображения
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	for (int i = 0; i < sizeMtx; i++)
	{
		unsigned long *arr = (unsigned long *)malloc(sizeof(long)* 2);
		arr[0] = i;
		arr[1] = i+1;
		q.push(arr);
	}

	

	//Обход в ширину

	//Состояние 0
	//Раздать вершины подграфов по узлам
	for (int i = 0; i < totalNodes -1  && i < sizeMtx; i++)
	{
		//MPI_Send:
		//buf	 -	 адрес начала расположения пересылаемых данных;
		//count	 -	 число пересылаемых элементов;
		//datatype	 -	 тип посылаемых элементов;
		//dest	 -	 номер процесса-получателя в группе, связанной с коммуникатором comm;
		//tag	 -	 идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
		//comm	 -	 коммуникатор области связи.

		//MPI_Send((void *)q.front(),2,MPI_LONG,i+1,1,MPI_COMM_WORLD);

		MPI_Request req;
		MPI_Isend((void *)&i,1,MPI_LONG,i+1,TAG_NEW_TASK,MPI_COMM_WORLD, &req);
		NodeStates[i+1] = STATE_WORKING;
		//Print(0,i+1,1,i);
		//q.pop();
	}

	//Необходимо добавить проверку, если количество 
	//узлов больше, чем размер матрицы
	if(totalNodes > sizeMtx && sizeMtx > DIVISIBILITY_CRITERIA)
	{
		//MPI_Status status;
		//unsigned long currNode = 0;

		////Сделать опрос среди узлом на делимость поддерева
		//for (int i = 0; i < totalNodes - sizeMtx; i++)
		//{
		//	unsigned long len = 0;
		//	unsigned long *vector = IsDivisibilityCriteria(i + 1, &len);

		//	//Если из дерева можно выделить свободное поддерево
		//	if(vector != NULL)
		//	{
		//		//Назначить работу свободному узлу
		//		MPI_Send((void *)vector,len,MPI_LONG,sizeMtx + currNode,1,MPI_COMM_WORLD);
		//		NodeStates[sizeMtx + currNode] = STATE_WORKING;
		//		Print(0,sizeMtx + currNode,1,len);
		//	}else
		//	{

		//	}
		//}
	}

	//Если размер матрицы больше, чем количество узлов
	if(sizeMtx > totalNodes - 1)
	{
		for (int i = 0; i < sizeMtx - (totalNodes - 1); i++)
		{
			tasks->push((totalNodes-1) + i);
		}
	}


	MPState1();

	cout << "=================Calculation ended"<<endl;
	//Освобождение памяти
	free(src_vector);
	free(dst_vector);
	free(NodeStates);
	delete tasks;
}

MPI_Request NewTask(int dest, unsigned long *vector, unsigned long len)
{
	MPI_Request req;

	//cout << "[0] NewTask blocked\n" << flush;
	MPI_Isend((void *)vector,len,MPI_LONG,dest,TAG_NEW_TASK,MPI_COMM_WORLD,&req);
	//cout << "[0] NewTask unblocked\n"<< flush;
	//Print(0,dest,TAG_NEW_TASK,vector,len);
	NodeStates[dest] = STATE_WORKING;
	return req;
}

MPI_Request SendNodeTerminate()
{
	bool buf;
	for (register long i = 1; i < totalNodes; i++)
	{
		if(NodeStates[i] == STATE_DONE)
		{
			//Print(0,status.MPI_SOURCE,TAG_NODE_TERMINATE,"");
			//cout << "[0] SendNodeTerminate blocked\n"<< flush;
			MPI_Request req;
			MPI_Isend(&buf,1, MPI_C_BOOL,i,TAG_NODE_TERMINATE,MPI_COMM_WORLD,&req);
			//cout << "[0] SendNodeTerminate unblocked\n"<< flush;
			return req;
		}
	}

}

MPI_Request SendDevideRequest()
{
	MPI_Request req;
	bool buf;

	for (register long i = 1; i < totalNodes; i++)
	{
		if(NodeStates[i] == STATE_WORKING)
		{
			//cout << "[0] SendDevideRequest blocked\n"<< flush;
			MPI_Isend(&buf,1, MPI_C_BOOL,i,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&req);
			//cout << "[0] SendDevideRequest unblocked\n"<< flush;

			return req;
		}
	}

	SendNodeTerminate();

	return 0;
}

void TaskCompleted(MPI_Status *status)
{
	int error, count;
	bool buf;

	error = MPI_Get_count(status,MPI_C_BOOL,&count);

	MPI_Recv(&buf,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_TASK_COMPLETED,MPI_COMM_WORLD,status);

	//cout <<"[0] State 0 MPI_Probe unblocked\n"<< flush;
	NodeStates[status->MPI_SOURCE] = STATE_DONE;
	//Print(status->MPI_SOURCE, 0, TAG_TASK_COMPLETED,"Done");


	bool allNodesAreDone = AllNodesAre(STATE_DONE);



	if(tasks->size()>0)
	{
		//cout << "[0] State 1 0\n" << flush;
		unsigned long val = tasks->front();
		tasks->pop();
		NewTask(status->MPI_SOURCE,&val,1);
	}else if(allNodesAreDone)
	{

	}
	else
	{
		SendDevideRequest();
	}
}

void DevideTaskReceived(MPI_Status *status)
{
	int error, count;
	bool buf;
	unsigned long *vector;

	MPI_Status taskStatus;

	error = MPI_Get_count(status,MPI_LONG,&count);
	//cout << "[0] DevideTaskReceived blocked\n"<< flush;
	vector = (unsigned long *)calloc(count, sizeof(unsigned long));
	MPI_Recv(vector,count,MPI_LONG,MPI_ANY_SOURCE,TAG_DEVIDE_TASK,MPI_COMM_WORLD,status);
	//cout << "[0] DevideTaskReceived unblocked\n"<< flush;

	if(count > 0)
	{
		for (register long i = 1; i < totalNodes; i++)
		{
			if(NodeStates[i] == STATE_DONE)
			{
				MPI_Request r = NewTask(i,vector,count);
				MPI_Wait(&r,&taskStatus);

				free(vector);
				return;
			}
		}
	}
}

void Print(MPI_Status *status)
{
	int error, count;
	char *buf;

	error = MPI_Get_count(status,MPI_CHAR,&count);
	buf = (char *)calloc(count, sizeof(char));
	MPI_Recv(buf,count,MPI_CHAR,MPI_ANY_SOURCE,TAG_PRINT,MPI_COMM_WORLD,status);

	cout << buf << flush;

	free(buf);
}

void MPState1()
{
	MPI_Status status;
	int error;
	int count;

	MPI_Barrier(MPI_COMM_WORLD);

	start = std::clock();

	while(true)
	{
		
		if(AllNodesAre(STATE_DONE) == true)
		{
			bool buf;
			MPI_Request req;
			for (register long i = 1; i < totalNodes; i++)
			{
					MPI_Isend(&buf,1, MPI_C_BOOL,i,TAG_NODE_TERMINATE,MPI_COMM_WORLD,&req);
			}

			//cout << "0 Barrier" << endl << flush;;
			MPI_Barrier(MPI_COMM_WORLD);
			//cout << "+0 Barrier" << endl << flush;;
			Print(0,0,0,"");
			return;
		}

		error = MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

		switch(status.MPI_TAG)
		{
		case TAG_DEVIDE_TASK:
			{
				DevideTaskReceived(&status);
				break;
			}
		case TAG_TASK_COMPLETED:
			{
				TaskCompleted(&status);
				break;
			}
		case TAG_PRINT:
			{
				Print(&status);
				break;
			}
		}
	}
}

//void MPState2()
//{
//	MPI_Status status;
//	int error;
//	bool buf;
//	int count;
//
//	while(true)
//	{
//		//Позволяет проверить входные сообщения без их реального приема
//		error = MPI_Probe(MPI_ANY_SOURCE,TAG_TASK_COMPLETED,MPI_COMM_WORLD,&status);
//
//		//Получить длину принимаемого сообщения
//		error = MPI_Get_count(&status,MPI_LONG,&count);
//
//		MPI_Recv(&buf,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_TASK_COMPLETED,MPI_COMM_WORLD,&status);
//		NodeStates[status.MPI_SOURCE] = STATE_DONE;
//		Print(status.MPI_SOURCE, 0, TAG_TASK_COMPLETED,"1111111111");
//
//		MPState3(status.MPI_SOURCE);
//
//	}
//}

void MPState2()
{
	MPI_Status status;
	int error;
	bool buf;
	int count;


	int state = 0;

	while(true)
	{
		//PrintNodeStatus();

		switch (state)
		{
		case 0:{
			//Позволяет проверить входные сообщения без их реального приема

			cout << "[0] State 0 MPI_Probe blocked\n"<< flush;
			error = MPI_Probe(MPI_ANY_SOURCE,TAG_TASK_COMPLETED,MPI_COMM_WORLD,&status);

			//Получить длину принимаемого сообщения
			error = MPI_Get_count(&status,MPI_LONG,&count);

			MPI_Recv(&buf,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_TASK_COMPLETED,MPI_COMM_WORLD,&status);

			cout <<"[0] State 0 MPI_Probe unblocked\n"<< flush;
			NodeStates[status.MPI_SOURCE] = STATE_DONE;
			Print(status.MPI_SOURCE, 0, TAG_TASK_COMPLETED,"Done");
			state = 1;

			break;
			   }
		case 1:
			{
				cout << "[0] State 1\n"<< flush;
				//Выделить новое поддерево
				bool allNodesAreFree = AllNodesAreFree();
				if(tasks->size()>0 && allNodesAreFree)
				{
					cout << "[0] State 1 0\n" << flush;
					unsigned long val = tasks->front();
					tasks->pop();

					cout << "[0] State 1 MPI_Send blocked\n" << flush;
					MPI_Send((void *)&val,1,MPI_LONG,status.MPI_SOURCE,1,MPI_COMM_WORLD);
					cout << "[0] State 1 MPI_Send unblocked\n"<< flush;
					Print(0,status.MPI_SOURCE,1,&val,1);
					NodeStates[status.MPI_SOURCE] = STATE_WORKING;
				}
				else
				{
					cout << "[0] State 1 1\n"<< flush;
					bool buf = true;
					int error;
					int count = 0;
					unsigned long *vector;
					MPI_Status st;

					bool nodeFound = false;
					//Опрашиваем узлы о делимости задачи

					for (long i = 1; i < totalNodes; i++)
					{

						if(NodeStates[i] == STATE_WORKING)
						{

							cout << "[0] State 1 1 MPI_Send blocked\n"<< flush;
							MPI_Send(&buf,1, MPI_C_BOOL,i,TAG_DEVIDE_TASK,MPI_COMM_WORLD);
							cout << "[0] State 1 1 MPI_Send unblocked\n"<< flush;


							cout << "[0] State 1 1 MPI_Probe blocked\n"<< flush;
							//Позволяет проверить входные сообщения без их реального приема
							error = MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&st);
							cout << "[0] State 1 1 MPI_Probe unblocked\n"<< flush;

							if(st.MPI_TAG == TAG_TASK_COMPLETED)
							{
								NodeStates[i] = STATE_DONE;
							}else if(st.MPI_TAG == TAG_DEVIDE_TASK)
							{
								//Получить длину принимаемого сообщения
								error = MPI_Get_count(&st,MPI_LONG,&count);


								cout << "[0] State 1 1 MPI_recv blocked\n"<< flush;
								vector = (unsigned long *)calloc(count, sizeof(unsigned long));
								MPI_Recv(vector,count,MPI_LONG,MPI_ANY_SOURCE,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&st);

								cout << "[0] State 1 1 MPI_Recv unblocked\n"<< flush;

								if(count > 0)
								{
									nodeFound = true;

									cout << "[0] State 1 2 MPI_Send blocked\n"<< flush;
									MPI_Send(vector,count, MPI_LONG,status.MPI_SOURCE,1,MPI_COMM_WORLD);
									cout << "[0] State 1 2 MPI_Send unblocked\n"<< flush;

									NodeStates[status.MPI_SOURCE] = STATE_WORKING;
									Print(0,status.MPI_SOURCE,1,vector,count);
									free(vector);

								}
							}


							//PrintNodeStatus();
							break;
						}
					}
					if(!nodeFound)
					{
						bool buf = true;
						Print(0,status.MPI_SOURCE,TAG_NODE_TERMINATE,"");
						cout << "[0] State 1 3 MPI_Send blocked\n"<< flush;
						MPI_Request req;
						MPI_Isend(&buf,1, MPI_C_BOOL,status.MPI_SOURCE,TAG_NODE_TERMINATE,MPI_COMM_WORLD,&req);
						cout << "[0] State 1 3 MPI_Send unblocked\n"<< flush;
						NodeStates[status.MPI_SOURCE] = STATE_DONE;
					}
				}

				PrintNodeStatus();
				if(AllNodesAre(STATE_DONE) == true)
				{
					cout << "0 Barrier" << endl << flush;;
					MPI_Barrier(MPI_COMM_WORLD);
					cout << "+0 Barrier" << endl << flush;;
					return;
					state = 10;
				}else{
					state = 0;
				}

				break;
			}

		default:
			cout<< "222222222222222222222222" <<endl<<flush;
			return;
		}


		//MPState3(status.MPI_SOURCE);

	}
}

void MPState3(int node)
{
	//MPI_Status status;

	////Выделить новое поддерево
	//if(tasks->size()>0 && false)
	//{
	//	//Если есть непройденные корневые вершины
	//	unsigned long val = tasks->front();
	//	tasks->pop();

	//	MPI_Send((void *)&val,1,MPI_LONG,node,1,MPI_COMM_WORLD);
	//	Print(0,node,1,&val,1);
	//	NodeStates[node] = STATE_WORKING;
	//}

	//MPState2();
}

void MPState4()
{

}

void MPState5()
{

}

void MPState6()
{

}

void Print(int src, int dst, int tag, char *str)
{
	char buf[256];
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);

	sprintf(buf, "%.5f\t [%d] -> [%d]: (%s) \"%s\"",cpu_time_used, src,dst,MsgTagsStr[tag],str);
	std::cout << buf <<endl;

}

void Print(int src, int dst, int tag, int str)
{
	char buf[256];
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);
	sprintf(buf, "%.5f\t [%d] -> [%d]: (%s) \"%d\"",cpu_time_used,src,dst,MsgTagsStr[tag],str);
	std::cout << buf <<endl;
}

void Print(int src, int dst, int tag, unsigned long *v, long len)
{
	char buf[256];
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);
	sprintf(buf, "%.5f\t [%d] -> [%d]: (%s) ",cpu_time_used,src,dst,MsgTagsStr[tag]);
	std::cout << buf << "\"";
	for (int i = 0; i < len; i++)
	{
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	std::cout << "\""<< endl;
}

void PrintVector(int node, unsigned long *v, long len)
{
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);
	printf("%.5f\t ",cpu_time_used);
	std::cout<< node << ": ";
	for (int i = 0; i < len; i++)
	{
		if(i % 20 == 0)
			cout<< "\n\t";
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	cout << endl;
}

unsigned long *IsDivisibilityCriteria(int node, unsigned long *len)
{
	MPI_Status status;
	bool result = false;

	//Отправить запрос на выделение поддерева
	MPI_Send((void *)&result,1,MPI_C_BOOL,node,TAG_DEVIDE_TASK,MPI_COMM_WORLD);

	int error,count ;

	//Получить статус ответа
	error = MPI_Probe(node,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&status);

	//Получить длину принимаемого сообщения (для вектора поддерева)
	error = MPI_Get_count(&status,MPI_LONG,&count);

	if(count > 0)
	{
		unsigned long* buffer;

		//Выделить память
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;

		MPI_Recv((void *)buffer,count,MPI_LONG,node,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&status);

		//Сделать проверку на ошибку
		//if(status.MPI_ERROR)

		(*len) = count;
		return buffer;



	}

	return NULL;
}

bool AllNodesAreFree()
{
	for (register unsigned long i = 1; i < totalNodes; i++)
	{
		if(NodeStates[i] == STATE_WORKING)
			return false;
	}

	return true;
}

bool AllNodesAre(ProcessState state)
{
	for (register unsigned long i = 1; i < totalNodes; i++)
	{
		if(NodeStates[i] != state)
		{
			return false;
		}
	}

	return true;
}

void PrintNodeStatus()
{
	std::string str;

	str.append("Node Status:\n");
	char buf[64];
	for (register unsigned long i = 1; i < totalNodes; i++)
	{
		switch(NodeStates[i])
		{

		case STATE_DONE: 
			sprintf(buf,"* [%d] %s\n", i, "Done"); 
			str.append(buf);
			break;
		case STATE_WORKING: 
			sprintf(buf,"* [%d] %s\n", i, "Working"); 
			str.append(buf);
			break;
		default: break;
		}
	}
	cout << str.c_str() << flush;

}


void PrepareSolutionDir()
{
	// These are data types defined in the "dirent" header
	struct dirent *next_file;
	DIR *theFolder;
	char filepath[64];

	mkdir("solutions");

	theFolder = opendir("solutions");

	while ( next_file = readdir(theFolder) )
	{
		// build the full path for each file in the folder
		sprintf(filepath, "%s/%s", "solutions", next_file->d_name);
		remove(filepath);
	}

}
