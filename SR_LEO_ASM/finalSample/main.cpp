// PR 2011 fall sample code
// T.A. Chen Shen-Shi 
// d98922030@csie.ntu.edu.tw

#include "faceDefine.h"
#include  <direct.h> 
#include  <windows.h>
// include toolkit class
#include "FaceDetection.h"
#include "OpenCVASMAlignment.h"
#include "FaceCrop.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include  <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <fstream>
using namespace cv;
using namespace std;



FaceDetection *face_detector;
OpenCVASMAlignment *myAlignment;
FaceCrop *FaceCropper;
vector<FaceInfo> CropFaceSet;   //Crop璶盎代戈癟
vector<IplImage*> FaceSet;      //Cropη顶紇钩
vector<FP> FaceFP;              //Alignment
char tmp[100];
char folder[100]; //矪瞶紇钩戈Ж
char studentID[100]; //厩ネID
char type[100];
char folderwithout[100];
char filename[150];
char dir[300];
char resultdir[200];
char savingfolder[200];

void FaceCropping(IplImage *img){
	        IplImage *ProcessImage = cvCloneImage(img);
			CropFaceSet.clear();
			FaceSet.clear(); 
			
			FaceCropper = new FaceCrop();
			FaceCropper->Initial(ProcessImage, FaceFP);
			FaceCropper->setParameter("CropConfig.txt");
			//FaceCropper ->setParameter(0.9,1.5,2.0,80,100,5.0);
            //FaceCropper->CropAllGrayLevelFace(&CropFaceSet);
			FaceCropper->CropAllFace(&CropFaceSet, &FaceSet );
			cvReleaseImage(&ProcessImage);
            delete FaceCropper;
			
}

void main()
{
	char *src;
	 src ="C:\\test\\6.tif";
	 IplImage *img;
	IplImage *drawimg;
	ofstream outfile;
	outfile.open("C:\\test\\8.out");
	img= cvLoadImage("C:\\test\\6.tif",1);
	drawimg = cvLoadImage(src,1);



	vector<CvRect> faceWindow; //store each face's rect position
	vector<CvMat*> alignmentResult1;// stroe facial points
	face_detector = new FaceDetection("cascade Data\\haarcascade.xml");
    myAlignment = new OpenCVASMAlignment();
	myAlignment->setModelPath("ASM Data\\FrontalFace_best.amf");


	if (myAlignment->loadModel() == -1) {
		fprintf(stderr,"Could not load ASM model...\n");
		return;
	}
	
	cvNamedWindow("originalimage");
	cvShowImage("originalimage",img);
 	faceWindow = face_detector->Detect(img);//羪盎代
	if(faceWindow.size() != 0) //璝Τ盎代羪
	{
		 for(int i=0; i<faceWindow.size(); i++)
									 cvRectangle(drawimg,cvPoint(faceWindow[i].x, faceWindow[i].y),cvPoint(faceWindow[i].x+faceWindow[i].width, faceWindow[i].y+faceWindow[i].height),CV_RGB(255,0,0));
		myAlignment->SetImage(img);
	//	 for(int i=0; i<faceWindow.size(); i++)	
		alignmentResult1.push_back(myAlignment->calcAlignment(faceWindow[0]));
								for(int i=0; i<alignmentResult1.size(); i++){
								   for (int k = 0; k < alignmentResult1[i]->rows; k++){
									   if(k==61)
                                        cvCircle(drawimg, cvPoint(cvmGet(alignmentResult1[i], k, 0), cvmGet(alignmentResult1[i], k, 1)), 1, CV_RGB(0, 0, 255), 2);
									   else
										cvCircle(drawimg, cvPoint(cvmGet(alignmentResult1[i], k, 0), cvmGet(alignmentResult1[i], k, 1)), 1, CV_RGB(0, 255, 0), 2);
								        outfile<<k+1<<" "<<cvmGet(alignmentResult1[i], k, 0)<<" "<<cvmGet(alignmentResult1[i], k, 1)<<endl;
								   }
								 }
							}
		cvNamedWindow("ivFace1");
	cvShowImage("ivFace1",drawimg);
	
							
							cvWaitKey(0);
							alignmentResult1.clear();
							faceWindow.clear();
							FaceFP.clear();	
							cvReleaseImage(&img);
			                cvReleaseImage(&drawimg);	
							outfile.close();
waitKey(0);
    		
}