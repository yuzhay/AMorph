static const char * MsgTagsStr[] = 
{ 
	"TAG_TASK_COMPLETED",
	"TAG_NEW_TASK",
	"TAG_DEVIDE_TASK",
	"TAG_NODE_TERMINATE"
};

enum MsgTags
{
	TAG_TASK_COMPLETED,//����������� ������� �������� ������
	TAG_NEW_TASK,//����������� ������� �������� ����� ���� ��� ����������
	TAG_DEVIDE_TASK, //�������� ��������� ���������
	TAG_NODE_TERMINATE,
	TAG_PRINT
};

enum ProcessState
{
	Unset,
	STATE_WORKING,
	STATE_DONE,
	STATE_TERMINATED,
	State3
};