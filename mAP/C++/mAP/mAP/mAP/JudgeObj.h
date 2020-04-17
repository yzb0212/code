#ifndef JUDGEOBJ_H
#define JUDGEOBJ_H

#include "readXmlTxt.h"
#include <vector>

using namespace std;


float max(float a, float b);
float min(float a, float b);

//对检测结果的obj根据置信度进行快速排序
//快速排序
void quickSort(vector<ObjInfo> &obj, //进行排序的数组	
	int start,//开始编号
	int end  //结尾编号
	);

//对检测结果的obj根据置信度进行排序
//冒泡排序
void bubbleSortScore(vector<ObjInfo> &obj);

//根据查准率和查全率数组计算AP
float voc_ap(float *rec,//召回率数组
	float *prec,//查准率数组	
	int num //目标的个数
	);

//求出检测识别结果的AP
void CalAP(vector<ObjInfo> obj,//读取的检测结果的txt文件信息
	vector<ObjInfo> GTobj,//各类的真实坐标信息
	string classname,//类的名字
	float &AP,				//求得该类的AP值
	float &precision,		//查准率
	float &recall			// 召回率
	);


#endif