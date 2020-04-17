#ifndef MAP_H
#define MAP_H

//#include "JudgeObj.h"
//#include "readXmlTxt.h"
#include <iostream>
#include <string>

using namespace std;

//����mAP��AP
extern "C" void mAPScore(char const* resultTxt, char const* XmlTestPath, float *&AP, float *&precision, float *&recall);



#endif
