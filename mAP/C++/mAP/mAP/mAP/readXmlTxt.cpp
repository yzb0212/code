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

//读取txt文件中的信息
void readTestClasstxt(string filename,//txt文件路径
	                  string* className,//目标类别名称
	         vector< vector<ObjInfo> > & obj//txt中目标信息
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
		vector<string> res;//读取每一行的字符串的容器
		splitString(temp, res);//分割一行字符
		if (res.size() == 1) //图像标签
		{
			imgName = res[0];
		}
		if (res.size() == 5)//图像中的目标检测信息
		{
			ObjInfo tempObj;
			tempObj.imgid = imgName;// 图像标号
            
			//txt中的中文进行UTF-8转化
           
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
					tempObj.socre = atof(res[2].c_str());  //目标置信度
					splitWord(res[3], tempObj.x1, tempObj.y1);  //左上点坐标
					splitWord(res[4], tempObj.x2, tempObj.y2);//右下点坐标
					float tempX1 = min(tempObj.x1, tempObj.x2);
					float tempX2 = max(tempObj.x1, tempObj.x2);
					float tempY1 = min(tempObj.y1, tempObj.y2);
					float tempY2 = max(tempObj.y1, tempObj.y2);
					tempObj.x1 = tempX1;
					tempObj.x2 = tempX2;
					tempObj.y1 = tempY1;
					tempObj.y2 = tempY2;
					obj[i].push_back(tempObj); //保存目标信息
				}
			}
		}

		vector<string>().swap(res);//释放res容器内存
	}
}

//根据字符串空格分割字符串
void splitString(string tempString,//待分割的整的字符串f
	vector<string>& res)//分割完之后的单个字符串数组
{
	//待分割的字符串，含有很多空格 
	string word = tempString;

	//暂存从word中读取的字符串 
	string result;

	//将字符串读到input中 
	stringstream input(word);

	//依次输出到result中，并存入res中 
	while (input >> result)
	{
		res.push_back(result);
	}
}

//根据特定字符对一个字符记性分割
void splitWord1(string word, string &x, string &y)
{
	char * strc = new char[strlen(word.c_str()) + 1];
	strcpy(strc, word.c_str());
	vector<string> vecs;
	char* tempstr = strtok(strc, ","); //根据特定字符“,”分割字符串
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, ",");//根据特定字符对字符串进行分割
	}
	
	x = vecs[0];
	y = vecs[1];

	delete[] strc; strc = NULL;
	vector<string>().swap(vecs);
}

//根据特定字符对一个字符记性分割
void splitWord2(string image, vector<string> &vecs, string flag)
{
	char * strc = new char[strlen(image.c_str()) + 1];
	strcpy(strc, image.c_str());

	char* tempstr = strtok(strc, flag.c_str()); //根据特定字符“.”分割字符串
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, flag.c_str());//根据特定字符对字符串进行分割
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


//读取测试集的txt文件
void readTesttxt(string filepath,//测试集txt文件路径
	vector<string>  &imglabel//存储测试集的图像编号容器
	)
{
	imglabel.clear();

	ifstream myfile(filepath.c_str());
	string temp;

	if (!myfile.is_open())
	{
		cout << "open failed" << endl;
	}

	while (getline(myfile, temp))  //循环读入每一行存入temp中
	{
		imglabel.push_back(temp);
	}

	myfile.close();
}

//读取测试集中图像的xml文件
void readTestxml1(vector<string>  imglabel,//测试集图像xml文件容器
	string xmlpath,     //图像xml文件路径
	string classname,   //类别
	vector<ObjInfo> &GTobj //xml文件中各图像的真实坐标信息
	)
{
	int i;
	GTobj.clear();
        
	int imgNum = imglabel.size();   //图像的个数
	for (i = 0; i < imgNum; i++)
	{
		string imgXml = xmlpath + "/" + imglabel[i];

		TiXmlDocument mydoc(imgXml.c_str());//xml文档
		mydoc.LoadFile();//加载文档

		TiXmlElement *RootElement = mydoc.RootElement();//获得根节点

		ObjInfo objtemp;

		//遍历该xml文档
		for (TiXmlElement *StuElement = RootElement->FirstChildElement();//第一个子元素  
			StuElement != NULL;
			StuElement = StuElement->NextSiblingElement())//下一个兄弟元素  
		{
			string name = StuElement->Value();//获得该子节点的名称

			if (name == "object")
			{
				int flag = 0;   //标记是否为该判断的类

				for (TiXmlElement *sonElement = StuElement->FirstChildElement();
					sonElement != NULL;
					sonElement = sonElement->NextSiblingElement())
				{
					string sonName = sonElement->Value();
					if (sonName == "name")
					{
						string clsnam = sonElement->GetText();//获得子节点的文本
						if (clsnam == classname)
						{
							flag = 1;
						}
					}

					if (sonName == "bndbox" && flag == 1)
					{
						//遍历子节点下的子节点
						for (TiXmlElement *sonElement1 = sonElement->FirstChildElement();
							sonElement1 != NULL;
							sonElement1 = sonElement1->NextSiblingElement())
						{
							string sonName1 = sonElement1->Value();


							if (sonName1 == "xmin")
							{
								string num = sonElement1->GetText();//获得子节点的文本
								objtemp.x1 = atof(num.c_str());

							}
							if (sonName1 == "ymin")
							{
								string num = sonElement1->GetText();//获得子节点的文本
								objtemp.y1 = atof(num.c_str());
							}
							if (sonName1 == "xmax")
							{
								string num = sonElement1->GetText();//获得子节点的文本
								objtemp.x2 = atof(num.c_str());
							}
							if (sonName1 == "ymax")
							{
								string num = sonElement1->GetText();//获得子节点的文本
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



//批处理读取文件下的测试集中每个图片的xml文件
void readFolderXML(string filename,//输入批处理的文件的路径(需指定读取*.xml文件)
	vector<string> &imageNum //文件夹下所有图片的XML文件
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


//根据特定字符提取文件名，消除文件后缀
string extractXMLName(string image)
{
	char * strc = new char[strlen(image.c_str()) + 1];
	strcpy(strc, image.c_str());
	vector<string> vecs;

	char* tempstr = strtok(strc, "."); //根据特定字符“.”分割字符串
	while (tempstr != NULL)
	{
		vecs.push_back(tempstr);
		tempstr = strtok(NULL, ".");//根据特定字符对字符串进行分割
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

//liunx下进行使用，判读读入的文件是不是都是xml文件
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
	vector<string>::iterator itr = xmlLabel.begin(); //对检测结果进行过滤
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




