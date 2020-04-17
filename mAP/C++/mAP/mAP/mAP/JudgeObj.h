#ifndef JUDGEOBJ_H
#define JUDGEOBJ_H

#include "readXmlTxt.h"
#include <vector>

using namespace std;


float max(float a, float b);
float min(float a, float b);

//�Լ������obj�������ŶȽ��п�������
//��������
void quickSort(vector<ObjInfo> &obj, //�������������	
	int start,//��ʼ���
	int end  //��β���
	);

//�Լ������obj�������ŶȽ�������
//ð������
void bubbleSortScore(vector<ObjInfo> &obj);

//���ݲ�׼�ʺͲ�ȫ���������AP
float voc_ap(float *rec,//�ٻ�������
	float *prec,//��׼������	
	int num //Ŀ��ĸ���
	);

//������ʶ������AP
void CalAP(vector<ObjInfo> obj,//��ȡ�ļ������txt�ļ���Ϣ
	vector<ObjInfo> GTobj,//�������ʵ������Ϣ
	string classname,//�������
	float &AP,				//��ø����APֵ
	float &precision,		//��׼��
	float &recall			// �ٻ���
	);


#endif