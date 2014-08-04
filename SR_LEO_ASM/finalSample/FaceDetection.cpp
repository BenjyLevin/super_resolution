#include "FaceDetection.h"

FaceDetection::FaceDetection(string cascadeDataPath)
:FaceDetectScale(1.2),FaceDetectMinNeighbor(3),FaceDetectAlogrithm(4),FaceDetectSearchWindowSize(18)
{

	faceCascade = (CvHaarClassifierCascade*)cvLoad( cascadeDataPath.c_str(), 0, 0, 0 );
	storage = cvCreateMemStorage(0);
}

FaceDetection::~FaceDetection()
{
	cvReleaseMemStorage(&storage);

}


vector<CvRect> FaceDetection::Detect(IplImage *inImg)
{			
	if(!inImg){
		cout << "ERROR: Could not load  Face image" << endl;
		return rFace;
	}
	if( !faceCascade )
	{
		cout << "ERROR: Could not load  Face classifier cascade" << endl;
		return rFace;
	}
	else
	{
		rFace.clear();
		IplImage *gray=cvCreateImage(cvSize(inImg->width,inImg->height),8,1);
		cvCvtColor(inImg,gray,CV_BGR2GRAY);
		cvClearMemStorage( storage );
		CvSeq* faces = cvHaarDetectObjects( gray, faceCascade, storage,
			FaceDetectScale,FaceDetectMinNeighbor, 
			FaceDetectAlogrithm,
			cvSize(FaceDetectSearchWindowSize, FaceDetectSearchWindowSize) );			
		
		cvReleaseImage(&gray);
		if(faces->total==0)
		{
			/*cout << "Detect Face Failed!!";*/
			return rFace;
		}
		else
		{
			for(int i=0;i<faces->total;i++)
			{
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
				rFace.push_back(cvRect(r->x,r->y,r->width,r->height));
			}
			
			return rFace;
		}			
	}
}