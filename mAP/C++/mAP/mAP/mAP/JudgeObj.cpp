//#include "stdafx.h"
#include "JudgeObj.h"
#include <sstream>
#include <assert.h>


float max(float a, float b)
{
	return (a >= b) ? a : b;
}
float min(float a, float b)
{
	return (a < b) ? a : b;
}

//对检测结果的obj根据置信度进行快速排序
//快速排序
void quickSort(vector<ObjInfo> &obj, //进行排序的数组	
	           int start,//开始编号
			   int end  //结尾编号
	)
{
	if (start< end)
	{
		int i = start, j = end;
		ObjInfo x = obj[start];

		while (i < j)
		{
			while (i < j && (-obj[j].socre) >= (-x.socre)) // 从右向左找第一个小于x的数  
			{
				j--;
			}
			if (i < j)
			{
				obj[i] = obj[j];
				i++;
			}
				
			while (i < j && (-obj[i].socre) < (-x.socre)) // 从左向右找第一个大于等于x的数  
			{
				i++;
			}
			if (i < j)
			{
				obj[j] = obj[i];
				j--;
			}
				
		}
		obj[i] = x;

		quickSort(obj, start, i - 1); // 递归调用  
		quickSort(obj, i + 1, end);
	}
}

//对检测结果的obj根据置信度进行排序
//冒泡排序
void bubbleSortScore(vector<ObjInfo> &obj)
{
	int i, j;
	int objNum = obj.size();
	for (i = 0; i < objNum - 1; i++)
	{
		for (j = 0; j < objNum - i - 1; j++)
		{
			if (obj[j].socre < obj[j + 1].socre)
			{
				ObjInfo temp = obj[j];
				obj[j] = obj[j + 1];
				obj[j + 1] = temp;
			}
		}
	}
}


//求出检测识别结果的AP
void CalAP(vector<ObjInfo> obj,		//读取的检测结果的txt文件信息
	       vector<ObjInfo> GTobj,	//各类的真实坐标信息
		   string classname,		//类的名字
	       float &AP,				//求得该类的AP值
		   float &precision,		//查准率
		   float &recall			// 召回率
	)
{
       int objnum = obj.size();
	
	if (objnum == 0)
	{
		AP = 0;
	}
	else
	{
		//quickSort(obj, 0, objnum - 1); //根据置信度对目标进行从大到小排序
		bubbleSortScore(obj);//根据置信度对目标进行从大到小排序


		int GTobjnum = GTobj.size();
		int *flag = new int[GTobjnum];  //表示真实目标是否已被判断
		memset(flag, 0, GTobjnum*sizeof(int));

		int i, j;

		int *tp = new int[objnum];  //每个目标真阳性标记
		int *fp = new int[objnum];  //每个目标假阳性标记
		memset(tp, 0, objnum*sizeof(int));
		memset(fp, 0, objnum*sizeof(int));

		for (i = 0; i < objnum; i++)
		{
			float ovmax = 0.00001;   //最大覆盖
			int jmax; //最大覆盖区域的标号
			float areaobj = (obj[i].y2 - obj[i].y1 + 1)*(obj[i].x2 - obj[i].x1 + 1);

			for (j = 0; j < GTobjnum; j++)
			{
				if (obj[i].imgid == GTobj[j].imgid) //判断是否在一副影像中
				{
					float xx1 = max(obj[i].x1, GTobj[j].x1);
					float yy1 = max(obj[i].y1, GTobj[j].y1);
					float xx2 = min(obj[i].x2, GTobj[j].x2);
					float yy2 = min(obj[i].y2, GTobj[j].y2);
					float w = max(float(0), xx2 - xx1 + 1);
					float h = max(float(0), yy2 - yy1 + 1);
					float inter = w*h; //交集
					float areaGTobj = (GTobj[j].y2 - GTobj[j].y1 + 1)*(GTobj[j].x2 - GTobj[j].x1 + 1);
					float uni = areaobj + areaGTobj - inter;//并集

					float ovr = inter / uni;
					if (ovmax < ovr)
					{
						ovmax = ovr;
						jmax = j;
					}

					if (j + 1 < GTobjnum && GTobj[j].imgid != GTobj[j + 1].imgid)
					{
						break; //跳出循环
					}
				}
			}

			if (ovmax >= 0.5 && flag[jmax] == 0)   //相交占比超过0.5
			{
				tp[i] = 1;
				flag[jmax] = 1;     //表示真实结果中该目标已被检测出
			}
			else
			{
				fp[i] = 1;
			}

		}// end for i

		for (i = 1; i < objnum; i++)
		{
			tp[i] = tp[i] + tp[i - 1];  //真阳性进行累加
			fp[i] = fp[i] + fp[i - 1];  //假阳性进行累加
		}

		float *rec = new float[objnum];  //召回率数组
		memset(rec, float(0.0), objnum*sizeof(float));

		float *prec = new float[objnum];// 查准率数组
		memset(prec, float(0.0), objnum*sizeof(float));

		int areaNum = 0;//用于计算RPC面积的目标个数


		for (i = 0; i < objnum; i++)
		{
			if (tp[i] <= GTobjnum)//判断召回率是不是等于1
			{
				areaNum = i + 1;

				rec[i] = tp[i] / max(float(GTobjnum), 0.000001);
				prec[i] = tp[i] / max(tp[i] + fp[i], 0.000001);
			}
			else
			{
				break;
			}
		}

		AP = voc_ap(rec, prec, areaNum);  //进行平均值计算

		float TP = tp[objnum-1];
		float FP = fp[objnum-1];
		// 查准率
		precision = TP / objnum;
		// 召回率
		recall = TP / GTobjnum;
	}

}

//根据查准率和查全率数组计算AP
float voc_ap(float *rec,//召回率数组
	float *prec,//查准率数组	
	int num //目标的个数
	)
{
	float sum = 0.0;
	sum += (prec[0] * rec[0]);
	for (int i = 1; i < num; i++)
	{
		//float precTemp = (prec[i] + prec[i - 1]) / 2;
		float precTemp = prec[i];
		sum += (precTemp * (rec[i] - rec[i - 1]));
	}

	return sum;

}



