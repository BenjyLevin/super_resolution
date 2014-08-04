#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
//#include <direct.h>
#include <ctime>
#include <vector>
#include <algorithm>
//#include <windows.h>
#include <iostream>
#include <string>
#include <fstream> 
#include "opencvheader.h"
using namespace std;

typedef struct FP{
	CvPoint lefteye;
	CvPoint	righteye;
	CvPoint	nose;
	CvPoint	mouth;
} FP;

typedef struct FaceInfo{
	IplImage* face;
	CvPoint leftTop;
	CvPoint	rightTop;
	CvPoint	leftdown;
	CvPoint	rightdown;
	//int Pos; //  0 •™ 1§§ 2•k
    //int Gender; // 0•N™Ì§k©   1•N™Ì®k© 
} FaceInfo;