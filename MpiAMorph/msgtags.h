static const char * MsgTagsStr[] = 
{ 
	"TAG_TASK_COMPLETED",
	"TAG_NEW_TASK",
	"TAG_DEVIDE_TASK",
	"TAG_NODE_TERMINATE"
};

enum MsgTags
{
	TAG_TASK_COMPLETED,//»сполн€ющий процесс закончил работу
	TAG_NEW_TASK,//»сполн€ющий процесс получает новый узел дл€ вычислени€
	TAG_DEVIDE_TASK, // ритерий делимости поддерева
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