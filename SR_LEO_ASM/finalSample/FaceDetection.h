
#include "opencvheader.h" 
using namespace std; 


class FaceDetection{

	public:
	
		vector<CvRect> rFace;	
		FaceDetection(string cascadeDataPath);
		~FaceDetection();

		vector<CvRect> Detect(IplImage *inImg);

	private:
		CvHaarClassifierCascade* faceCascade;
		CvMemStorage* storage;
		
		//parameter
		float FaceDetectScale;
		int FaceDetectMinNeighbor;
		int FaceDetectAlogrithm;
		// CV_HAAR_DO_CANNY_PRUNING     1
		// CV_HAAR_SCALE_IMAGE          2
		// CV_HAAR_FIND_BIGGEST_OBJECT  4
		// CV_HAAR_DO_ROUGH_SEARCH      8
		int FaceDetectSearchWindowSize;
		

};