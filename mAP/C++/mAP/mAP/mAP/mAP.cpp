//#include "stdafx.h"
#include "mAP.h"
#include "JudgeObj.h"
#include "readXmlTxt.h"
//����mAP��AP
void mAPScore(char const* resultTxt, char const* XmlTestPath, float *&AP, float *&precision, float *&recall)
{
	int i;

	//Ŀ�����
	string classname[6] = { "�ɻ�", "��ֻ", "�����", "����", "��ͷ", "�ٳ�" };
	AP = new float[7];//ÿ������APֵ�����һ�����洢 mAP
	precision = new float[7];
	recall = new float[7];

	for (i = 0; i < 7; i++)
	{
		AP[i] = 0.0;
		precision[i] = 0.0;
		recall[i] = 0.0;
	}
	int classNum = 6;

	//��һ����ȡresultTxt�ļ�
	vector< vector<ObjInfo> > obj; //�洢5��������Ϣ
	obj.resize(classNum);
	readTestClasstxt(resultTxt, classname, obj);


	for (i = 0; i < classNum; i++)
	{
		//***************************
		//**�ڶ�����ȡxml�ļ�
		//**************************
		//string xmlpath = XmlTestPath + "/*.xml";
		vector<string> xmlLabel;     xmlLabel.clear();  //�洢����ͼ��XML�ļ�������
		readFolderXML(XmlTestPath, xmlLabel);
                

                //�ж�xmlLabel�������ǲ��Ƕ���.xml�ļ�
                judgeXml(xmlLabel);
                

		vector<ObjInfo> GTobj;   GTobj.clear();
		readTestxml1(xmlLabel, XmlTestPath, classname[i], GTobj);

		//****************************
		//**��������AP
		//****************************
		CalAP(obj[i], GTobj, classname[i], AP[i], precision[i], recall[i]); //����ÿ������APֵ

		vector<ObjInfo>().swap(GTobj);
		vector<string>().swap(xmlLabel);
	}

	//****************************
	//**���Ĳ���mAP
	//****************************
	float mAP = 0.0;
	float sum = 0.0;
	float sum_precision = 0.0;
	float sum_recall = 0.0;
	for (i = 0; i < classNum; i++)
	{
		sum += AP[i];
		sum_precision += precision[i];
		sum_recall += recall[i];
	}
	mAP = sum / classNum;
	AP[6] = mAP;
	// ���׼�ʺ��ٻ��ʵľ�ֵ
	precision[6] = sum_precision / classNum;
	recall[6] = sum_recall / classNum;

	for (i = 0; i < classNum; i++)
	{
		vector<ObjInfo>().swap(obj[i]);
	}

}
