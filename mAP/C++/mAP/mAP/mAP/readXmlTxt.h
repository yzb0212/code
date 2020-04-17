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
	string imgid;//ͼ����
	float socre;//�������Ŷ�
	float x1;  //��������ϵ�����µ�����
	float y1;
	float x2;
	float y2;
};

//�����ַ����ո�ָ��ַ���
void splitString(string tempString,//���ָ�������ַ���
	vector<string>& res);//�ָ���֮��ĵ����ַ�������


//�����ض��ַ���һ���ַ����Էָ�
void splitWord1(string word, string &x, string &y);

//�����ض��ַ���һ���ַ����Էָ�
void splitWord2(string image, vector<string> &vecs, string flag);

void splitWord(string word, float& x, float& y);

//��ȡtxt�ļ��е���Ϣ
void readTestClasstxt(string filename,//txt�ļ�·��
	string* className,//Ŀ���������
	vector< vector<ObjInfo> > & obj//txt��Ŀ����Ϣ
	);

//��ȡ���Լ���txt�ļ�
void readTesttxt(string filepath,//���Լ�txt�ļ�·��
	vector<string>  &imglabel//�洢���Լ���ͼ��������
	);

//��ȡ���Լ���ͼ���xml�ļ�
void readTestxml1(vector<string>  imglabel,//���Լ�ͼ��xml�ļ�����
	string xmlpath,     //ͼ��xml�ļ�·��
	string classname,   //���
	vector<ObjInfo> &GTobj //xml�ļ��и�ͼ�����ʵ������Ϣ
	);

//�������ȡ�ļ��µĲ��Լ���ÿ��ͼƬ��xml�ļ�
void readFolderXML(string filename,//������������ļ���·��(��ָ����ȡ*.xml�ļ�)
	vector<string> &imageNum //�ļ���������ͼƬ��XML�ļ�
	);

//�����ض��ַ���ȡ�ļ����������ļ���׺
string extractXMLName(string image);

//liunx�½���ʹ�ã��ж�������ļ��ǲ��Ƕ���xml�ļ�
void judgeXml(vector<string> &xmlLabel);

#endif
