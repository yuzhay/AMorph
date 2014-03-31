static const char * MsgTagsStr[] = 
{ 
	"DepthParameter", 
	"VectorParameter",
	"SlaveWorkStarted",
	"SlaveWorkEnded",
	"SlaveNewNode",
	"SlaveSubTreeDevisibility",
	"SlaveTerminate"
};

enum MsgTags
{
	DepthParameter, //��������� �������� ������� ������
	VectorParameter,//��������� �������� ����� ������������

	SlaveWorkStarted,//����������� ������� ����� ������
	SlaveWorkEnded,//����������� ������� �������� ������
	SlaveNewNode,//����������� ������� �������� ����� ���� ��� ����������
	SlaveSubTreeDevisibility, //�������� ��������� ���������
	SlaveTerminate,
	SlavePrint
};

enum ProcessState
{
	Unset,
	STATE1_WORKING,
	STATE_DONE,
	STATE_TERMINATED,
	State3
};