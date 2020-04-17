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

//�Լ������obj�������ŶȽ��п�������
//��������
void quickSort(vector<ObjInfo> &obj, //�������������	
	           int start,//��ʼ���
			   int end  //��β���
	)
{
	if (start< end)
	{
		int i = start, j = end;
		ObjInfo x = obj[start];

		while (i < j)
		{
			while (i < j && (-obj[j].socre) >= (-x.socre)) // ���������ҵ�һ��С��x����  
			{
				j--;
			}
			if (i < j)
			{
				obj[i] = obj[j];
				i++;
			}
				
			while (i < j && (-obj[i].socre) < (-x.socre)) // ���������ҵ�һ�����ڵ���x����  
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

		quickSort(obj, start, i - 1); // �ݹ����  
		quickSort(obj, i + 1, end);
	}
}

//�Լ������obj�������ŶȽ�������
//ð������
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


//������ʶ������AP
void CalAP(vector<ObjInfo> obj,		//��ȡ�ļ������txt�ļ���Ϣ
	       vector<ObjInfo> GTobj,	//�������ʵ������Ϣ
		   string classname,		//�������
	       float &AP,				//��ø����APֵ
		   float &precision,		//��׼��
		   float &recall			// �ٻ���
	)
{
       int objnum = obj.size();
	
	if (objnum == 0)
	{
		AP = 0;
	}
	else
	{
		//quickSort(obj, 0, objnum - 1); //�������Ŷȶ�Ŀ����дӴ�С����
		bubbleSortScore(obj);//�������Ŷȶ�Ŀ����дӴ�С����


		int GTobjnum = GTobj.size();
		int *flag = new int[GTobjnum];  //��ʾ��ʵĿ���Ƿ��ѱ��ж�
		memset(flag, 0, GTobjnum*sizeof(int));

		int i, j;

		int *tp = new int[objnum];  //ÿ��Ŀ�������Ա��
		int *fp = new int[objnum];  //ÿ��Ŀ������Ա��
		memset(tp, 0, objnum*sizeof(int));
		memset(fp, 0, objnum*sizeof(int));

		for (i = 0; i < objnum; i++)
		{
			float ovmax = 0.00001;   //��󸲸�
			int jmax; //��󸲸�����ı��
			float areaobj = (obj[i].y2 - obj[i].y1 + 1)*(obj[i].x2 - obj[i].x1 + 1);

			for (j = 0; j < GTobjnum; j++)
			{
				if (obj[i].imgid == GTobj[j].imgid) //�ж��Ƿ���һ��Ӱ����
				{
					float xx1 = max(obj[i].x1, GTobj[j].x1);
					float yy1 = max(obj[i].y1, GTobj[j].y1);
					float xx2 = min(obj[i].x2, GTobj[j].x2);
					float yy2 = min(obj[i].y2, GTobj[j].y2);
					float w = max(float(0), xx2 - xx1 + 1);
					float h = max(float(0), yy2 - yy1 + 1);
					float inter = w*h; //����
					float areaGTobj = (GTobj[j].y2 - GTobj[j].y1 + 1)*(GTobj[j].x2 - GTobj[j].x1 + 1);
					float uni = areaobj + areaGTobj - inter;//����

					float ovr = inter / uni;
					if (ovmax < ovr)
					{
						ovmax = ovr;
						jmax = j;
					}

					if (j + 1 < GTobjnum && GTobj[j].imgid != GTobj[j + 1].imgid)
					{
						break; //����ѭ��
					}
				}
			}

			if (ovmax >= 0.5 && flag[jmax] == 0)   //�ཻռ�ȳ���0.5
			{
				tp[i] = 1;
				flag[jmax] = 1;     //��ʾ��ʵ����и�Ŀ���ѱ�����
			}
			else
			{
				fp[i] = 1;
			}

		}// end for i

		for (i = 1; i < objnum; i++)
		{
			tp[i] = tp[i] + tp[i - 1];  //�����Խ����ۼ�
			fp[i] = fp[i] + fp[i - 1];  //�����Խ����ۼ�
		}

		float *rec = new float[objnum];  //�ٻ�������
		memset(rec, float(0.0), objnum*sizeof(float));

		float *prec = new float[objnum];// ��׼������
		memset(prec, float(0.0), objnum*sizeof(float));

		int areaNum = 0;//���ڼ���RPC�����Ŀ�����


		for (i = 0; i < objnum; i++)
		{
			if (tp[i] <= GTobjnum)//�ж��ٻ����ǲ��ǵ���1
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

		AP = voc_ap(rec, prec, areaNum);  //����ƽ��ֵ����

		float TP = tp[objnum-1];
		float FP = fp[objnum-1];
		// ��׼��
		precision = TP / objnum;
		// �ٻ���
		recall = TP / GTobjnum;
	}

}

//���ݲ�׼�ʺͲ�ȫ���������AP
float voc_ap(float *rec,//�ٻ�������
	float *prec,//��׼������	
	int num //Ŀ��ĸ���
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



