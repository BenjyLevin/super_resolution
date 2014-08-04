#ifndef _OPENCVASMALIGNMENT_H_
#define _OPENCVASMALIGNMENT_H_

#include <vector>
#include <string>
#include <iostream>
#include "opencvasm_include/asmfitting.h"

#pragma comment(lib,"opencvasm_lib/asmlibrary.lib")

using namespace std;

#define BUFFER_SIZE 256

#define OPENCVASMCENTER  0
#define OPENCVASMLEFT	 1
#define OPENCVASMRIGHT	 2

class OpenCVASMAlignment
{
    public:
        OpenCVASMAlignment(string configfile);
		OpenCVASMAlignment();
		int setModelPath(string mpath, int pos = OPENCVASMCENTER);
        int loadConfig(string filename);
        int loadModel();
        void SetImage(const IplImage *src_Img);
        CvMat* calcAlignment(const CvRect rect, int pos = OPENCVASMCENTER);
		CvMat* calcAlignment(CvMat *init_points, int pos = OPENCVASMCENTER);

    private:
        IplImage *srcImg;// Source image
        asmfitting asmfit[3];
        string model_path[3];
	    int leftEyePoint[3];
	    int rightEyePoint[3];
	    int mouthPoint[3];
	    int nosePoint[3];
        int max_iteration;
        float enlarge_scale;
        CvSize smallestThreshold;
        CvSize largestThreshold;
};

#endif