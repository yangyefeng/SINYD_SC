#ifndef _Protocol_Def_H_
#define _Protocol_Def_H_

#define ROOT_DATASRC "source"
#define ROOT_CONTENT "content"
#define SECOND_INFO "info"
#define SECOND_CONTENT_DATA "content_data"
#define THIRD_DATA "data"
#define SECOND_TYPE "type"
#define CODE_ID "code_id"
#define NUM_ID "num_id"

//Json����ͷ��Ϣ
enum EN_DATASRC_INFO
{
    DATASRC_INFO_MIXID = 0,    //��ͻ�ID
	DATASRC_INFO_MIXTYPE       //���վ����
};

enum EN_DATA_INFO
{
	DATA_INFO_PACKETID = 0,  //��id
	DATA_INFO_SENSORID,      //�ź���id
	DATA_INFO_VALUE,         //ֵ
	DATA_INFO_TIME,          //ʱ��
	DATA_INFO_TYPE           //�ź�������
};

enum EN_CONTENT_INFO
{
	CONTENT_INFO_CONTENT_TYPE = 0,   //Json��Content����
	CONTENT_INFO_DATA_TYPE           //Json��Data����
};

//�������ݣ������ݣ���Ӧ�ַ���id
enum EN_PRODECT_INFO
{
	PRODECT_INFO_MIXTURE_ID = 16,  //�̺�
	PRODECT_INFO_TIME,             //��¼ʱ��
	PRODECT_INFO_DRYMIXING_TIME,   //�ɰ�ʱ��
	PRODECT_INFO_WETMIXING_TIME,   //ʪ��ʱ��
	PRODECT_INFO_MIXING_TEMP,      //������¶�
	PRODECT_INFO_ASPHALT_TEMP,     //�����¶�
	PRODECT_INFO_AGGR_TEMP,        //�����¶�
	PRODECT_INFO_ILLEGAL           //Υ���ʶ
};

enum EN_CONTENT_TYPE
{
	CONTENT_TYPE_INIT = 0,
	CONTENT_TYPE_DB,          //����DB����
	CONTENT_TYPE_CONFIG,      //���������ļ�����
	CONTENT_TYPE_TEXT         //������ģ����
};

enum EN_DATA_TYPE
{
	DATA_TYPE_INIT = 0,
	DATA_TYPE_MIXDATA,          //��������Ϣ
	DATA_TYPE_SRCDATA,          //Դ������Ϣ
	DATA_TYPE_CONFIG_FONT,      //��ģ������Ϣ
	DATA_TYPE_HOPPER            //�ϲ�������Ϣ
};


#endif	//_Protocol_Def_H_