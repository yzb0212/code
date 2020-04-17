//#include "stdafx.h"
#include "mAP.h"
#include "JudgeObj.h"
#include "readXmlTxt.h"
//计算mAP及AP
void mAPScore(char const* resultTxt, char const* XmlTestPath, float *&AP, float *&precision, float *&recall)
{
	int i;

	//目标类别
	string classname[6] = { "飞机", "船只", "储蓄罐", "桥梁", "码头", "操场" };
	AP = new float[7];//每个类别的AP值，最后一个数存储 mAP
	precision = new float[7];
	recall = new float[7];

	for (i = 0; i < 7; i++)
	{
		AP[i] = 0.0;
		precision[i] = 0.0;
		recall[i] = 0.0;
	}
	int classNum = 6;

	//第一步读取resultTxt文件
	vector< vector<ObjInfo> > obj; //存储5个类别的信息
	obj.resize(classNum);
	readTestClasstxt(resultTxt, classname, obj);


	for (i = 0; i < classNum; i++)
	{
		//***************************
		//**第二步读取xml文件
		//**************************
		//string xmlpath = XmlTestPath + "/*.xml";
		vector<string> xmlLabel;     xmlLabel.clear();  //存储测试图像XML文件的容器
		readFolderXML(XmlTestPath, xmlLabel);
                

                //判断xmlLabel容器中是不是都是.xml文件
                judgeXml(xmlLabel);
                

		vector<ObjInfo> GTobj;   GTobj.clear();
		readTestxml1(xmlLabel, XmlTestPath, classname[i], GTobj);

		//****************************
		//**第三步求AP
		//****************************
		CalAP(obj[i], GTobj, classname[i], AP[i], precision[i], recall[i]); //计算每个类别的AP值

		vector<ObjInfo>().swap(GTobj);
		vector<string>().swap(xmlLabel);
	}

	//****************************
	//**第四步求mAP
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
	// 求查准率和召回率的均值
	precision[6] = sum_precision / classNum;
	recall[6] = sum_recall / classNum;

	for (i = 0; i < classNum; i++)
	{
		vector<ObjInfo>().swap(obj[i]);
	}

}
