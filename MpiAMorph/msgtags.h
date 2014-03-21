static const char * MsgTagsStr[] = 
{ 
	"DepthParameter", 
	"VectorParameter",
	"SlaveWorkStarted",
	"SlaveWorkEnded",
	"SlaveNewNode",
	"SlaveSubTreeDevisibility"
};

enum MsgTags
{
	DepthParameter, //—ообщение содержит глубину поиска
	VectorParameter,//—ообщение содержит ветор перестановки

	SlaveWorkStarted,//»сполн€ющий процесс начал работу
	SlaveWorkEnded,//»сполн€ющий процесс закончил работу
	SlaveNewNode,//»сполн€ющий процесс получает новый узел дл€ вычислени€
	SlaveSubTreeDevisibility // ритерий делимости поддерева
};

enum ProcessState
{
	Unset,
	STATE1_WORKING,
	STATE_DONE,
	State3
};