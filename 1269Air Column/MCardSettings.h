
////�����˶����ƿ� 5000ϵ��  �汾Ver1.32��

//���ô�����������ʦ ����ʵ������޸����¼���������
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//�������޸�
//����������趨��
const int   MCARD_COUNT_MAX				=1;	//һ��ֻ1�ſ�		//���=8;			
//������������趨��								  
const int   AXIS_COUNT_MAX				=4;	//һ��ֻ4����		//���=8;
//���IO��չ�ڵ����趨��(��ʾ������ͬһ�������ϵ���չ�Ͽ����Ŀ����Ƕ��٣�						
const int   EX_IONODE_COUNT_MAX			=1;	//һ��ֻ1����չ		//���=8;  
//�ڵ�����˿�������趨��							   
const int   INPUT_PORT_COUNT_MAX		=16;//һ��16			//���=16;
//�ڵ�����˿�������趨��							  
const int   OUTPUT_PORT_COUNT_MAX		=16;//һ��16			//���=16;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//�������޸�

//IO���������ɹ�ʽ//�����߿��˽�һ��
//O[ MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * OUTPUT_PORT_COUNT_MAX ];		//��ǰDO���
//I[ MCARD_COUNT_MAX * (1+EX_IONODE_COUNT_MAX) * INPUT_PORT_COUNT_MAX  ];		//��ǰDi����

/*

//�˶����ƿ� ��ȫ�ص�����ģ�� //ȫ�ֵ�

//�ᰲȫ����
BOOL CALLBACK myAxsSafeFun(UINT uAxs)
{
	//**********************************************
	// �˴���� ���˶�ǰʱ ��ȫ���ƴ���
	//**********************************************
	return TRUE;
}

//IO�����ȫ����
BOOL CALLBACK myDoSafeFun (UINT uIOIndex,UINT uValue)
{
	//**********************************************
	// �˴���� IO ���ʱ ��ȫ���ƴ���
	//**********************************************
	return TRUE;
}

*/



//DMC5000 ϵ���˶����ƿ����˶�������������� 9.7��
//�� 9.7 �˶�����������
//������ ���� ����
//0 ERR_NOERR �ɹ�
//1 ERR_UNKNOWN δ֪����
//2 ERR_PARAERR ��������
//3 ERR_TIMEOUT PCI ͨѶ��ʱ
//4 ERR_CONTROLLERBUSY �������˶�
//6 ERR_CONTILINE �����岹����
//8 ERR_CANNOT_CONNECTETH �޷����Ӵ���
//9 ERR_HANDLEERR �������Ӵ���
//10 ERR_SENDERR PCI �쳣������ PCI ��λ�Ƿ��ɶ�
//12 ERR_FIRMWAREERR �̼��ļ�����
//14 ERR_FIRMWAR_MISMATCH �̼���ƥ��
//20 ERR_FIRMWARE_INVALID_PARA_ERR �̼���������
//22 ERR_FIRMWARE_STATE_ERR �̼���ǰ״̬���������
//24 ERR_FIRMWARE_CARD_NOT_SUPPORT �̼���֧�ֵĹ���
//25 ERR_PASSWORD_ERR �������
//26 ERR_PASSWORD_TIMES_OUT ������������������
//50 ERR_LIST_NUM_ERR LIST �Ŵ���
//51 ERR_LIST_NOT_OEPN LIST ���ڴ�״̬
//52 ERR_PARA_NOT_VALID ����������Ч��Χ
//53 ERR_LIST_HAS_OPEN LIST �Ѿ���
//54 ERR_MAIN_LIST_NOT_OPEN ��Ҫ�� LIST û�г�ʼ��
//55 ERR_AXIS_NUM_ERR ����������Ч��Χ
//56 ERR_AXIS_MAP_ARRAY_ERR ��ӳ���Ϊ��
//57 ERR_MAP_AXIS_ERR ӳ�������
//58 ERR_MAP_AXIS_BUSY ӳ����æ
//59 ERR_PARA_SET_FORBIT �˶��в����и��Ĳ���
//60 ERR_FIFO_FULL ����������
//61 ERR_RADIUS_ERR �뾶Ϊ 0 ��С������ľ����һ��
//62 ERR_MAINLIST_HAS_START ERR_MAINLIST_HAS_START
//63 ERR_ACC_TIME_ZERO �Ӽ���ʱ��Ϊ 0
//64 ERR_MAINLIST_NOT_START ��Ҫ LIST û������