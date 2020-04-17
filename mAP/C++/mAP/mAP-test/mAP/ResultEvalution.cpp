// ResultEvalution.cpp : 定义控制台应用程序的入口点。
//


#include <string>
#include "iostream"
#include <iomanip>
#include "mAP.h"

using namespace std;


int main(int argc,char *argv[])
{
	
	string resultTxt; //检测结果的txt文件夹的存储路径
        //cout<<"input resultTxt : ";
	//cin >> resultTxt;
	resultTxt = argv[1];

	string XmlTestPath; //真实情况的xml文件夹路径
        //cout<<"input XmlTestPath : ";
	//cin >> XmlTestPath;
	XmlTestPath = argv[2];
	//float AP[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; //每个类别的AP值，最后一个数存储 mAP
	float *AP;
	float *precision;
	float *recall;
	mAPScore(resultTxt.c_str(), XmlTestPath.c_str(), AP, precision, recall);

	//string classname[7] = { "飞机", "船只", "储蓄罐", "桥梁", "码头", "操场", "平均分" };
	//for (int i = 0; i < 7; i++)
	//{
	//		cout.flags(ios::left);
       //		cout << classname[i] << "\t" << setw(20) << precision[i] << setw(20) << recall[i] << endl;
	//}
	for (int i = 0; i < 7; i++){
	    cout << precision[i] << ",";
	}
	for (int i = 0; i < 7; i++){
	    cout << recall[i] << ",";
	}
        cout << endl;

	return 0;
}

