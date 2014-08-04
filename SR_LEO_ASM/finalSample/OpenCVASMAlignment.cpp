#include <iostream>
#include <fstream>
#include "OpenCVASMAlignment.h"

using namespace std;


OpenCVASMAlignment::OpenCVASMAlignment()
{
	model_path[OPENCVASMCENTER] = "";
    model_path[OPENCVASMLEFT] = "";
    model_path[OPENCVASMRIGHT] = "";
    smallestThreshold.width = 50;
    smallestThreshold.height = 50;
    largestThreshold.width = 500;
    largestThreshold.height = 500;
    enlarge_scale = 1.2;
    max_iteration = 50;
    srcImg = NULL;
}

OpenCVASMAlignment::OpenCVASMAlignment(std::string configfile)
{
    if(loadConfig(configfile) == -1)
    {
        cout << "Load Config File Error!";
        return;
    }
    if (loadModel() == -1)
    {
        cout << "Load Model File Error!";
        return;
    }

    srcImg = NULL;
}

int OpenCVASMAlignment::loadConfig(string filename)
{
    ifstream input(filename.c_str());

    char buffer[BUFFER_SIZE];
    int length;
    size_t found;
    string var, value;

    if (input == NULL)
		return -1;

    while(!input.eof())
    {
        input.getline(buffer, BUFFER_SIZE);
        if(buffer[0] == '#')
            continue;
        string temp = "";
        length = strlen(buffer);
        for(int i = 0; i < length; i++)
        {
            if(buffer[i] != ' ' && buffer[i] != '\n')
                temp += buffer[i];
        }
        found = temp.find("=");
        if(found != string::npos)
        {
            var = temp.substr(0, found);
            value = temp.substr(found + 1);
            istringstream convert(value, istringstream::in);
            
            if(var.compare("FRONT_DATABASE_PATH") == 0)
                model_path[OPENCVASMCENTER] = value;
            else if(var.compare("FRONTFACE_LEFT_EYE_POINT") == 0)
                convert >> leftEyePoint[OPENCVASMCENTER];
            else if(var.compare("FRONTFACE_RIGHT_EYE_POINT") == 0)
                convert >> rightEyePoint[OPENCVASMCENTER];
            else if(var.compare("FRONTFACE_MOUTH_POINT") == 0)
                convert >> mouthPoint[OPENCVASMCENTER];
            else if(var.compare("FRONTFACE_NOSE_POINT") == 0)
                convert >> nosePoint[OPENCVASMCENTER];
            else if(var.compare("LEFT_DATABASE_PATH") == 0)
                model_path[OPENCVASMLEFT] = value;
            else if(var.compare("LEFTFACE_LEFT_EYE_POINT") == 0)
                convert >> leftEyePoint[OPENCVASMLEFT];
            else if(var.compare("LEFTFACE_RIGHT_EYE_POINT") == 0)
                convert >> rightEyePoint[OPENCVASMLEFT];
            else if(var.compare("LEFTFACE_MOUTH_POINT") == 0)
                convert >> mouthPoint[OPENCVASMLEFT];
            else if(var.compare("LEFTFACE_NOSE_POINT") == 0)
                convert >> nosePoint[OPENCVASMLEFT];
            else if(var.compare("RIGHT_DATABASE_PATH") == 0)
                model_path[OPENCVASMRIGHT] = value;
            else if(var.compare("RIGHTFACE_LEFT_EYE_POINT") == 0)
                convert >> leftEyePoint[OPENCVASMRIGHT];
            else if(var.compare("RIGHTFACE_RIGHT_EYE_POINT") == 0)
                convert >> rightEyePoint[OPENCVASMRIGHT];
            else if(var.compare("RIGHTFACE_MOUTH_POINT") == 0)
                convert >> mouthPoint[OPENCVASMRIGHT];
            else if(var.compare("RIGHTFACE_NOSE_POINT") == 0)
                convert >> nosePoint[OPENCVASMRIGHT];
            else if(var.compare("SAMLLEST_THRESHOLD_WIDTH") == 0)
                convert >> smallestThreshold.width;
            else if(var.compare("SMALLEST_THRESHOLD_HEIGHT") == 0)
                convert >> smallestThreshold.height;
            else if(var.compare("LARGEST_THRESHOLD_WIDTH") == 0)
                convert >> largestThreshold.width;
            else if(var.compare("LARGEST_THRESHOLD_HEIGHT") == 0)
                convert >> largestThreshold.height;
            else if(var.compare("ENLARGE_REGION_SCALE") == 0)
                convert >> enlarge_scale;
            else if(var.compare("MAX_ITERATION") == 0)
                convert >> max_iteration;
        }
    }
    input.close();
    return 0;
}

int OpenCVASMAlignment::loadModel()
{
    for(int i = 0; i < 1; i++)
    {
        if(asmfit[i].Read(model_path[i].c_str()) == false)
		    return -1;
    }
    return 0;
}

int OpenCVASMAlignment::setModelPath(string mpath, int pos)
{
	if (pos < 0) {
		pos = 0;
	}
	else if (pos > 2) {
		pos = 2;
	}
	model_path[pos] = mpath;

	return 0;
}

void OpenCVASMAlignment::SetImage(const IplImage *src_Img)
{
    if(srcImg != NULL)
        cvReleaseImage(&srcImg);
    srcImg = cvCloneImage(src_Img);
}

CvMat* OpenCVASMAlignment::calcAlignment(const CvRect rect, int pos)
{
    IplImage* faceImg = NULL;
    CvMat *result = NULL;
	CvRect detectedRect, initRect;
    asm_shape *shapes = NULL;
    asm_shape *detshapes = NULL;
    double tmpX, tmpY;
    double scaleX, scaleY;


    pos = OPENCVASMCENTER;

    // Detection window
	detectedRect.width = (int) (rect.width * enlarge_scale);
	detectedRect.height = (int) (rect.height * enlarge_scale);
	detectedRect.x = rect.x - (detectedRect.width - rect.width) / 2;
	detectedRect.y = rect.y - (detectedRect.height - rect.height) / 2;
	if (detectedRect.x < 0)
		detectedRect.x = 0;
	if (detectedRect.x + detectedRect.width > srcImg->width)
		detectedRect.width = srcImg->width - detectedRect.x;
	if (detectedRect.y < 0)
		detectedRect.y = 0;
	if (detectedRect.y + detectedRect.height > srcImg->height)
		detectedRect.height = srcImg->height - detectedRect.y;

    //initial detshapes
    if(detectedRect.width < smallestThreshold.width || detectedRect.height < smallestThreshold.height)
    {
        scaleX = smallestThreshold.width / (double) detectedRect.width;
        scaleY = smallestThreshold.height / (double) detectedRect.height;
    }
    else if(detectedRect.width > largestThreshold.width || detectedRect.height > largestThreshold.height)
    {
        scaleX = largestThreshold.width / (double) detectedRect.width;
        scaleY = largestThreshold.height / (double) detectedRect.height;
    }
    else
    {
        scaleX = 1;
        scaleY = 1;
    }

    detshapes = new asm_shape[1];
    detshapes[0].Resize(2);
    detshapes[0][0].x = (int)(detectedRect.x * scaleX);
	detshapes[0][0].y = (int)(detectedRect.y * scaleY);
    detshapes[0][1].x = (int)(detshapes[0][0].x + detectedRect.width * scaleX);
    detshapes[0][1].y = (int)(detshapes[0][0].y + detectedRect.height * scaleY);

    faceImg = cvCreateImage(cvSize(srcImg->width * scaleX, srcImg->height * scaleY), srcImg->depth, srcImg->nChannels);
    cvResize(srcImg, faceImg, CV_INTER_CUBIC);

    //initialize shape from detect box
    shapes = new asm_shape[1];
    InitShapeFromDetBox(shapes[0], detshapes[0], asmfit[pos].GetMappingDetShape(), asmfit[pos].GetMeanFaceWidth());

    //fitting
    asmfit[pos].Fitting2(shapes, 1, faceImg, max_iteration);
    //asmfit[pos].Fitting2(shapes, 1, srcImg, max_iteration);

    //Set result
	result = cvCreateMat(shapes[0].NPoints(), 2, CV_32FC1);

	for (int i = 0; i < shapes[0].NPoints(); i++) {
		tmpX = shapes[0][i].x / scaleX;
		tmpY = shapes[0][i].y / scaleY;
		cvmSet(result, i, 0, tmpX);
		cvmSet(result, i, 1, tmpY);
	}

    //if one of four points is out of the rectangle, we set the result be -1
/*	if (!tag) {
		for (int i = 0; i < 8; i++)
			cvmSet(result, i, 0, -1);
	}*/

    //test
    /*CvScalar color = CV_RGB(255, 0, 0);
    cvCircle(srcImg, cvPoint(shapes[0][36].x / scaleX  , shapes[0][36].y / scaleY  ), 1, color, 1, 8, 0);
    cvCircle(srcImg, cvPoint(shapes[0][31].x / scaleX  , shapes[0][31].y / scaleY  ), 1, color, 1, 8, 0);
    cvCircle(srcImg, cvPoint(shapes[0][67].x / scaleX  , shapes[0][67].y / scaleY  ), 1, color, 1, 8, 0);
    cvCircle(srcImg, cvPoint(shapes[0][66].x / scaleX  , shapes[0][66].y / scaleY  ), 1, color, 1, 8, 0);

    cvNamedWindow("test");
    cvShowImage("test", srcImg);
    cvWaitKey(0);
    cvDestroyWindow("test");
    cvSaveImage("result2.bmp", srcImg);*/

    cvReleaseImage(&faceImg);
	delete [] shapes;

	return result;
}

CvMat* OpenCVASMAlignment::calcAlignment(CvMat *init_points, int pos)
{
    CvMat *result = NULL;
    asm_shape *shapes = NULL;


	if (pos < OPENCVASMCENTER) {
        pos = OPENCVASMCENTER;
	}
	else if(pos > OPENCVASMRIGHT) {
        pos = OPENCVASMCENTER;
	}

    // Initialize shape
	shapes = new asm_shape[1];
	shapes[0].Resize(init_points->rows);
	for (int i = 0; i < init_points->rows; i++) {
		shapes[0][i].x = cvmGet(init_points, i, 0);
		shapes[0][i].y = cvmGet(init_points, i, 1);
	}

    // Fitting
    asmfit[pos].Fitting2(shapes, 1, srcImg, max_iteration);

    // Set result
	result = cvCreateMat(shapes[0].NPoints(), 2, CV_32FC1);
	for (int i = 0; i < shapes[0].NPoints(); i++) {
		cvmSet(result, i, 0, shapes[0][i].x);
		cvmSet(result, i, 1, shapes[0][i].y);
	}

	// Release
	delete [] shapes;

	return result;
}