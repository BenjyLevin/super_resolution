#include "faceDefine.h"

enum{MAN=1,WOMAN};
enum{LEFT=0,CENTER,RIGHT};
enum{NORMAL,SPECIAL};

class FaceCrop{

	public:
		IplImage* srcImg;
		IplImage* cropImg;
		IplImage* normalizeImg;
		
		int faceCounter;
		char outputFilename[128];

		int numOfFace;
		double a,b,c;
		int normalizeWidth,normalizeHeight;
		double maxRotationAngle;

		CvPoint leftTop;
		CvPoint	rightTop;
		CvPoint	leftdown;
		CvPoint	rightdown;

		vector<FP> fpSet;
		vector<int> sexSet;
		vector<int> poseSet;
		vector<int> normalOrSpecialSet;
		//vector<CvPoint> LeftEye;
		//vector<CvPoint> RightEye;
		//vector<CvPoint> nose;
		//vector<CvPoint> mouth;

		void Initial(IplImage* inputImg , char* inputFilename );
		void Initial(IplImage *inputImg, vector<FP> fp);

		~FaceCrop(){
			cvReleaseImage(&srcImg);
		};

		void setParameter(double parameterA,double parameterB,double parameterC,int width,int height,double angle){
			a=parameterA;
			b=parameterB;
			c=parameterC;
			normalizeWidth=width;
			normalizeHeight=height;
			maxRotationAngle=angle;
		
		};
		void setParameter(char* configFilename);
		void setOutputFilename(char *filename){
			int i=0;
			while(filename[i]!='.')i++;
			strncpy ( outputFilename,filename,i);
			outputFilename[i]='\0';
			
		
		};
		bool Crop(CvPoint LE,CvPoint RE,CvPoint No,CvPoint Mo);
		void saveCropImage(int index);
		void CropAllFace();
		void CropAllFace(vector<FaceInfo> *cropFaceSet);
		void CropAllFace(vector<IplImage*> *cropFaceSet);
		void CropAllFace(vector<FaceInfo> *cropFaceSet,vector<IplImage*> *cropGrayFaceSet);

		void CropAllGrayLevelFace(vector<FaceInfo> *cropFaceSet);
		void CropAllGrayLevelFace(vector<IplImage*> *cropFaceSet);
		void makeDir();
	private:
		
		void getFaceInfo(char *filename);
		void drawPoint();


};
