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
	DepthParameter, //��������� �������� ������� ������
	VectorParameter,//��������� �������� ����� ������������

	SlaveWorkStarted,//����������� ������� ����� ������
	SlaveWorkEnded,//����������� ������� �������� ������
	SlaveNewNode,//����������� ������� �������� ����� ���� ��� ����������
	SlaveSubTreeDevisibility //�������� ��������� ���������
};

enum ProcessState
{
	Unset,
	STATE1_WORKING,
	STATE_DONE,
	State3
};