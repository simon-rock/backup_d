// backup_msg.h
#ifndef _BACKUP_MSG_H_
#define _BACKUP_MSG_H_
#define MSG(X) WM_MSG_##X
#define MSG_DATA(X) STRCT_MSG_##X

#define  WM_MSG_START				1000		//��������
#define  WM_MSG_STOP				1001		//����ֹͣ
#define  WM_MSG_CONFIG              1002		//������Ϣ
#define  WM_MSG_CONFIG_COMPLETE     1003		//������Ϣ
#define  WM_MSG_BACKUP_BRICK	    1004		//�������� brick ID
#define  WM_MSG_BACKUP_BRICK_STOP	1005		//��ֹͣ���� brick ID
#define  WM_MSG_BACKUP_BRICK_ERROR	1006		//���� brick ID ��������
#define  WM_MSG_BACKUP_BRICK_MSG	1007		//���� brick ID ��������

typedef struct 
{
	char BRICK_ID[100];
	char BRICK_PATH[255];
	char MOUNT_PATH[255];
	char BACKUP_PATH[255];
}STRCT_MSG_CONFIG,*pSTRCT_MSG_CONFIG;

typedef struct 
{
	char BRICK_ID[100];
}STRCT_MSG_BACKUP_BRICK,*pSTRCT_MSG_BACKUP_BRICK, STRCT_MSG_BACKUP_BRICK_STOP, *pSTRCT_MSG_BACKUP_BRICK_STOP;

typedef struct 
{
	char BRICK_ID[100];
	char MSG_SHOW[255];
}STRCT_MSG_BACKUP_BRICK_ERROR,*pSTRCT_MSG_BACKUP_BRICK_ERROR,STRCT_MSG_BACKUP_BRICK_MSG,*pSTRCT_MSG_BACKUP_BRICK_MSG;
#endif // _BACKUP_MSG_H_