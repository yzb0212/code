//#include "stdafx.h"
#include "readXmlTxt.h"
#include <string>
#include "iostream"
#include "fstream"
#include <vector>
#include <sstream>
#include <cassert>
#include "UTF8ToGBK.h"
#include <dirent.h>
#include <sys/types.h>



using namespace std;

//��ȡtxt�ļ��е���Ϣ
void readTestClasstxt(string filename,//txt�ļ�·��
	                  string* className,//Ŀ���������
	         vector< vector<ObjInfo> > & obj//txt��Ŀ����Ϣ
	)
{
	ifstream myfile(filename.c_str());
	string temp;
	if (!myfile.is_open())
	{
		cout << "open resulttxt failed" << endl;
	}

	string imgName;
	int i, j;

	while (getline(myfile, temp))
	{
		vector<string> res;//��ȡÿһ�е��ַ���������
		splitString(temp, res);//�ָ�һ���ַ�
		if (res.size() == 1) //ͼ���ǩ
		{
			imgName = res[0];
		}
		if (res.size() == 5)//ͼ���е�Ŀ������Ϣ
		{
			ObjInfo tempObj;
			tempObj.imgid = imgName;// ͼ����
            
			//txt�е����Ľ���UTF-8ת��
           
            #ifdef _WIN32
			string s3 = Utf8ToGbk(res[1].c_str());
			res[1] = s3;
            #else
			char dst_gbk[1024] = {0};
			Utf8ToGbk((char*)res[1].c_str(), strlen(res[1].c_str()), dst_gbk, sizeof(dst_gbk));
			res[1]=dst_gbk;
            #endif
            
			
			for (i = 0; i < 6; i++)
			{
				if (res[1] == className[i])
				{
                                        //cout<<"into txt "<<endl;
					tempObj.socre = atof(res[2].c_str());  //Ŀ�����Ŷ�
					splitWord(res[3], tempObj.x1, tempObj.y1);  //���ϵ�����
					splitWord(res[4], tempObj.x2, tempObj.y2);//���µ�����
					float tempX1 = min(tempObj.x1, tempObj.x2);
					float tempX2 = max(tempObj.x1, tempObj.x2);
					float tempY1 = min(tempObj.y1, tempObj.y2);
					float tempY2 = max(tempObj.y1, tempObj.y2);
					tempObj.x1 = tempX1;
					tempObj.x2 = tempX2;
					tempObj.y1 = tempY1;
					tempObj.y2 = tempY2;
					obj[i].push_back(tempObj); //����Ŀ����Ϣ
				}
			}
		}

		vector<string>().swap(res);//�ͷ�res�����ڴ�
	}
}

//�����ַ����ո�ָ��ַ���
void splitString(string tempString,//���ָ�������ַ���f
	vector<string>& res)//�ָ���֮��ĵ����ַ�������
{
	//���ָ���ַ��������кܶ�ո� 
	string word = tempString;

	//�ݴ��word�ж�ȡ���ַ��� 
	string result;

	//���ַ�������input�� 
	stringstream input(word);

	//���������result�У�������res�� 
	while (input >> result)
	{
		res.push_back(result);
	}
}

//�����ض��ַ���һ���ַ����Էָ�
void splitWord1(string word, string &x, string &y)
{
	char * strc = new char[strlen(word.c_str()) + 1];
	strcpy(strc, word.c_str());
	vector<string> vecs;
	char* tempstr = strtok(strc, ","); //�����ض��ַ���,���ָ��ַ���
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, ",");//�����ض��ַ����ַ������зָ�
	}
	
	x = vecs[0];
	y = vecs[1];

	delete[] strc; strc = NULL;
	vector<string>().swap(vecs);
}

//�����ض��ַ���һ���ַ����Էָ�
void splitWord2(string image, vector<string> &vecs, string flag)
{
	char * strc = new char[strlen(image.c_str()) + 1];
	strcpy(strc, image.c_str());

	char* tempstr = strtok(strc, flag.c_str()); //�����ض��ַ���.���ָ��ַ���
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, flag.c_str());//�����ض��ַ����ַ������зָ�
	}

	delete[] strc;
	strc = NULL;
}

void splitWord(string word, float& x, float& y)
{
	string x1, y1;
	splitWord1(word, x1, y1);

	vector<string> vecs;
	splitWord2(x1, vecs, "(");
	x = atof(vecs[0].c_str());

	vecs.clear();
	splitWord2(y1, vecs, ")");
	y = atof(vecs[0].c_str());

	vector<string>().swap(vecs);
}


//��ȡ���Լ���txt�ļ�
void readTesttxt(string filepath,//���Լ�txt�ļ�·��
	vector<string>  &imglabel//�洢���Լ���ͼ��������
	)
{
	imglabel.clear();

	ifstream myfile(filepath.c_str());
	string temp;

	if (!myfile.is_open())
	{
		cout << "open failed" << endl;
	}

	while (getline(myfile, temp))  //ѭ������ÿһ�д���temp��
	{
		imglabel.push_back(temp);
	}

	myfile.close();
}

//��ȡ���Լ���ͼ���xml�ļ�
void readTestxml1(vector<string>  imglabel,//���Լ�ͼ��xml�ļ�����
	string xmlpath,     //ͼ��xml�ļ�·��
	string classname,   //���
	vector<ObjInfo> &GTobj //xml�ļ��и�ͼ�����ʵ������Ϣ
	)
{
	int i;
	GTobj.clear();
        
	int imgNum = imglabel.size();   //ͼ��ĸ���
	for (i = 0; i < imgNum; i++)
	{
		string imgXml = xmlpath + "/" + imglabel[i];

		TiXmlDocument mydoc(imgXml.c_str());//xml�ĵ�
		mydoc.LoadFile();//�����ĵ�

		TiXmlElement *RootElement = mydoc.RootElement();//��ø��ڵ�

		ObjInfo objtemp;

		//������xml�ĵ�
		for (TiXmlElement *StuElement = RootElement->FirstChildElement();//��һ����Ԫ��  
			StuElement != NULL;
			StuElement = StuElement->NextSiblingElement())//��һ���ֵ�Ԫ��  
		{
			string name = StuElement->Value();//��ø��ӽڵ������

			if (name == "object")
			{
				int flag = 0;   //����Ƿ�Ϊ���жϵ���

				for (TiXmlElement *sonElement = StuElement->FirstChildElement();
					sonElement != NULL;
					sonElement = sonElement->NextSiblingElement())
				{
					string sonName = sonElement->Value();
					if (sonName == "name")
					{
						string clsnam = sonElement->GetText();//����ӽڵ���ı�
						if (clsnam == classname)
						{
							flag = 1;
						}
					}

					if (sonName == "bndbox" && flag == 1)
					{
						//�����ӽڵ��µ��ӽڵ�
						for (TiXmlElement *sonElement1 = sonElement->FirstChildElement();
							sonElement1 != NULL;
							sonElement1 = sonElement1->NextSiblingElement())
						{
							string sonName1 = sonElement1->Value();


							if (sonName1 == "xmin")
							{
								string num = sonElement1->GetText();//����ӽڵ���ı�
								objtemp.x1 = atof(num.c_str());

							}
							if (sonName1 == "ymin")
							{
								string num = sonElement1->GetText();//����ӽڵ���ı�
								objtemp.y1 = atof(num.c_str());
							}
							if (sonName1 == "xmax")
							{
								string num = sonElement1->GetText();//����ӽڵ���ı�
								objtemp.x2 = atof(num.c_str());
							}
							if (sonName1 == "ymax")
							{
								string num = sonElement1->GetText();//����ӽڵ���ı�
								objtemp.y2 = atof(num.c_str());
							}
						}//end for

						string imgtemp = extractXMLName(imglabel[i]);
						objtemp.imgid = imgtemp;
						GTobj.push_back(objtemp);

					}//end if bndbox
				}// end for

			}//end if name == "object"

		}
	}
}



//�������ȡ�ļ��µĲ��Լ���ÿ��ͼƬ��xml�ļ�
void readFolderXML(string filename,//������������ļ���·��(��ָ����ȡ*.xml�ļ�)
	vector<string> &imageNum //�ļ���������ͼƬ��XML�ļ�
	)
{
	imageNum.clear();
	string temp;

	DIR *dirp = NULL;
	struct dirent *dir_entry = NULL;

	if ( ( dirp = opendir(filename.c_str())) == NULL )
	{
             cout<<filename<<"open fail !"<<endl;
	     exit(1); 
	}
	while ((dir_entry = readdir(dirp)) != NULL)
	{
		temp = dir_entry->d_name;
		imageNum.push_back(temp);
        }

	closedir(dirp);
}


//�����ض��ַ���ȡ�ļ����������ļ���׺
string extractXMLName(string image)
{
	char * strc = new char[strlen(image.c_str()) + 1];
	strcpy(strc, image.c_str());
	vector<string> vecs;

	char* tempstr = strtok(strc, "."); //�����ض��ַ���.���ָ��ַ���
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, ".");//�����ض��ַ����ַ������зָ�
	}
	delete[] strc;
	strc = NULL;

	int num = vecs.size();
	if (num == 2)
	{
		return vecs[0];
	}
	else if (num > 2)
	{
		//string temp = NULL;
		string temp;
		for (int i = 0; i < num - 1; i++)
		{
			temp += vecs[i];
			if (i < num - 2)
			{
				temp += ".";
			}
		}
		return temp;
	}

	return 0;
}

//liunx�½���ʹ�ã��ж�������ļ��ǲ��Ƕ���xml�ļ�
void judgeXml(vector<string> &xmlLabel)
{
	int xmlNum = xmlLabel.size();
	int *flag = new int[xmlNum];

	int i, j;
	for (i = 0; i < xmlNum; i++)
	{
		flag[i] = 0;
		vector<string> res;
		splitWord2(xmlLabel[i], res, ".");

		if (res.size() >= 2)
		{
			for (j = 0; j < res.size(); j++)
			{
				if (res[j] == "xml")
				{
					flag[i] = 1;
				}
			}
		}
		vector<string>().swap(res);
	}

	int start = 0;
	vector<string>::iterator itr = xmlLabel.begin(); //�Լ�������й���
	while (itr != xmlLabel.end())
	{
		if (flag[start] == 0)
		{
			itr = xmlLabel.erase(itr);
		}
		else
		{
			itr++;
		}
		start++;
	}

	delete[] flag; flag = NULL;

}




