#ifndef READXMLTXT_H
#define READXMLTXT_H
#include <string>
#include "tinystr.h"
#include "tinyxml.h"
#include <vector>

#include <fstream>
#include <cassert>


using namespace std;

struct ObjInfo{
	string imgid;//图像编号
	float socre;//对象置信度
	float x1;  //对象的坐上点和右下点坐标
	float y1;
	float x2;
	float y2;
};

//根据字符串空格分割字符串
void splitString(string tempString,//待分割的整的字符串
	vector<string>& res);//分割完之后的单个字符串数组


//根据特定字符对一个字符记性分割
void splitWord1(string word, string &x, string &y);

//根据特定字符对一个字符记性分割
void splitWord2(string image, vector<string> &vecs, string flag);

void splitWord(string word, float& x, float& y);

//读取txt文件中的信息
void readTestClasstxt(string filename,//txt文件路径
	string* className,//目标类别名称
	vector< vector<ObjInfo> > & obj//txt中目标信息
	);

//读取测试集的txt文件
void readTesttxt(string filepath,//测试集txt文件路径
	vector<string>  &imglabel//存储测试集的图像编号容器
	);

//读取测试集中图像的xml文件
void readTestxml1(vector<string>  imglabel,//测试集图像xml文件容器
	string xmlpath,     //图像xml文件路径
	string classname,   //类别
	vector<ObjInfo> &GTobj //xml文件中各图像的真实坐标信息
	);

//批处理读取文件下的测试集中每个图片的xml文件
void readFolderXML(string filename,//输入批处理的文件的路径(需指定读取*.xml文件)
	vector<string> &imageNum //文件夹下所有图片的XML文件
	);

//根据特定字符提取文件名，消除文件后缀
string extractXMLName(string image);

//liunx下进行使用，判读读入的文件是不是都是xml文件
void judgeXml(vector<string> &xmlLabel);

#endif
