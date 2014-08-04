#include "FaceCrop.h"

void FaceCrop::Initial(IplImage *inputImg, char *inputFilename){

	srcImg=cvCloneImage(inputImg);
	faceCounter=0;
	getFaceInfo(inputFilename);
	
	//makeDir();
	//drawPoint();
	//Crop(fpSet[0].lefteye,fpSet[0].righteye,fpSet[0].nose,fpSet[0].mouth);


}
void FaceCrop::Initial(IplImage *inputImg, vector<FP> fp){

	srcImg=inputImg;
	faceCounter=0;

	for(vector<FP>::iterator iter=fp.begin() ; iter!=fp.end() ; iter++ ){
		fpSet.push_back(*iter);
	
	}

	//getFaceInfo();
	//drawPoint();
	//Crop(fpSet[0].lefteye,fpSet[0].righteye,fpSet[0].nose,fpSet[0].mouth);


}
void FaceCrop::makeDir(){
	system("md face");
	system("md face\\man");
	system("md face\\man\\left");
	system("md face\\man\\center");
	system("md face\\man\\right");
	system("md face\\man\\special");
	system("md face\\woman\\left");
	system("md face\\woman\\center");
	system("md face\\woman\\right");
	system("md face\\woman\\special");

}
void FaceCrop::setParameter(char* configFilename){
	ifstream configFile(configFilename);
	string tempString;

	configFile>>a>>tempString;
	configFile>>b>>tempString;
	configFile>>c>>tempString;
	configFile>>normalizeWidth>>tempString;
	configFile>>normalizeHeight>>tempString;
	configFile>>maxRotationAngle>>tempString;

	configFile.close();

	//printf("%f %f %f %d %d %f",a,b,c,normalizeWidth,normalizeHeight,maxRotationAngle);
	//getchar();
};

void FaceCrop::getFaceInfo(char *filename){
	//string tempString;
	char tempString[100];
	

	int temp,temp1,temp2,temp3,temp4,temp5,temp6,tempSwap;
	int sex,pose,normalOrSpecial;
	FP tempFP;

	ifstream faceInfoFile(filename);

	faceInfoFile>>numOfFace;
	for(int i=0 ; i<numOfFace ; i++){
		//faceInfoFile>>tempString;
		faceInfoFile.getline(tempString,100);
		faceInfoFile.getline(tempString,100);
	
		faceInfoFile>>temp1>>sex>>pose>>temp4>>temp5>>normalOrSpecial;
		for(int j=0 ; j<8 ;j++)
				faceInfoFile>>temp;

		faceInfoFile>>tempFP.righteye.x>>tempFP.righteye.y;
		faceInfoFile>>tempFP.lefteye.x>>tempFP.lefteye.y;
		//printf("%d %d %d %d ",tempFP.lefteye.x,tempFP.lefteye.y,tempFP.righteye.x,tempFP.righteye.y);
		if(tempFP.lefteye.x>tempFP.righteye.x){
			tempSwap=tempFP.lefteye.x;
			tempFP.lefteye.x=tempFP.righteye.x;
			tempFP.righteye.x=tempSwap;
			tempSwap=tempFP.lefteye.y;
			tempFP.lefteye.y=tempFP.righteye.y;
			tempFP.righteye.y=tempSwap;
		}
		//printf("%d %d %d %d",tempFP.lefteye.x,tempFP.lefteye.y,tempFP.righteye.x,tempFP.righteye.y);
		
		faceInfoFile>>tempFP.nose.x>>tempFP.nose.y;
		faceInfoFile>>tempFP.mouth.x>>tempFP.mouth.y;
		
		for(int j=0 ; j<136 ;j++)
			faceInfoFile>>temp;
		if(tempFP.lefteye.x!=-1){
			fpSet.push_back(tempFP);
			sexSet.push_back(sex);
			poseSet.push_back(pose);
			normalOrSpecialSet.push_back(normalOrSpecial);
		}
		
	}
	faceInfoFile.close();
}

void FaceCrop::CropAllFace(){

	//for(vector<FP>::iterator iter=fpSet.begin() ; iter!=fpSet.end() ; iter++ ){
	//	Crop(iter->lefteye,iter->righteye,iter->nose,iter->mouth);
	//	saveCropImage();
	//}	
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			saveCropImage(i);
		}
	}	
}
void FaceCrop::CropAllFace(vector<FaceInfo> *cropFaceSet){

	//for(vector<FP>::iterator iter=fpSet.begin() ; iter!=fpSet.end() ; iter++ ){
	//	Crop(iter->lefteye,iter->righteye,iter->nose,iter->mouth);
	//	saveCropImage();
	//}	
	FaceInfo temp;
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			temp.face=normalizeImg;
			temp.leftdown=leftdown;
			temp.leftTop=leftTop;
			temp.rightdown=rightdown;
			temp.rightTop=rightTop;
			cropFaceSet->push_back(temp);
		}
	}	
	//for(vector<IplImage*>::iterator iter=cropFaceSet.begin(); iter!=cropFaceSet.end()  ;iter++){
	//	cvNamedWindow("Img",1);
	//	cvShowImage("Img",*iter);
	//	cvWaitKey(0);
	//}

}
void FaceCrop::CropAllFace(vector<FaceInfo> *cropFaceSet,vector<IplImage*> *cropGrayFaceSet){

	//for(vector<FP>::iterator iter=fpSet.begin() ; iter!=fpSet.end() ; iter++ ){
	//	Crop(iter->lefteye,iter->righteye,iter->nose,iter->mouth);
	//	saveCropImage();
	//}	
	FaceInfo temp;
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			IplImage* grayImg=cvCreateImage(cvSize(normalizeImg->width,normalizeImg->height),IPL_DEPTH_8U,1);
			cvCvtColor(normalizeImg,grayImg,CV_BGR2GRAY);
			cropGrayFaceSet->push_back(grayImg);
			
			temp.face=normalizeImg;
			temp.leftdown=leftdown;
			temp.leftTop=leftTop;
			temp.rightdown=rightdown;
			temp.rightTop=rightTop;
			cropFaceSet->push_back(temp);
		}
	}	
	//for(vector<IplImage*>::iterator iter=cropFaceSet.begin(); iter!=cropFaceSet.end()  ;iter++){
	//	cvNamedWindow("Img",1);
	//	cvShowImage("Img",*iter);
	//	cvWaitKey(0);
	//}

}
void FaceCrop::CropAllFace(vector<IplImage*> *cropFaceSet){


	
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			cropFaceSet->push_back(normalizeImg);
		}
	}	


}
void FaceCrop::CropAllGrayLevelFace(vector<FaceInfo> *cropFaceSet){

	//for(vector<FP>::iterator iter=fpSet.begin() ; iter!=fpSet.end() ; iter++ ){
	//	Crop(iter->lefteye,iter->righteye,iter->nose,iter->mouth);
	//	saveCropImage();
	//}	
	FaceInfo temp;
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			IplImage* grayImg=cvCreateImage(cvSize(normalizeImg->width,normalizeImg->height),IPL_DEPTH_8U,1);
			cvCvtColor(normalizeImg,grayImg,CV_BGR2GRAY);
			cvReleaseImage(&normalizeImg);
			temp.face=grayImg;
			temp.leftdown=leftdown;
			temp.leftTop=leftTop;
			temp.rightdown=rightdown;
			temp.rightTop=rightTop;
			cropFaceSet->push_back(temp);
		}
	}	
	//for(vector<IplImage*>::iterator iter=cropFaceSet.begin(); iter!=cropFaceSet.end()  ;iter++){
	//	cvNamedWindow("Img",1);
	//	cvShowImage("Img",*iter);
	//	cvWaitKey(0);
	//}

}
void FaceCrop::CropAllGrayLevelFace(vector<IplImage*> *cropFaceSet){

	
	for(int i=0 ; i<fpSet.size() ; i++ ){
		if(Crop(fpSet[i].lefteye,fpSet[i].righteye,fpSet[i].nose,fpSet[i].mouth)==true){
			IplImage* grayImg=cvCreateImage(cvSize(normalizeImg->width,normalizeImg->height),IPL_DEPTH_8U,1);
			cvCvtColor(normalizeImg,grayImg,CV_BGR2GRAY);
			cropFaceSet->push_back(grayImg);
			cvReleaseImage(&normalizeImg);

		}
	}	


}
void  FaceCrop::saveCropImage(int index){
	char filename[128];
	char directory[128];
	char sexDir[128];
	char poseFir[128];
	if(sexSet[index]==MAN){
		sprintf(sexDir,"man\\");
	}
	else{
		sprintf(sexDir,"woman\\");
	}
	if(normalOrSpecialSet[index]==SPECIAL){
		sprintf(poseFir,"special\\");
	}
	else if(poseSet[index]==LEFT){
		sprintf(poseFir,"left\\");
	}
	else if(poseSet[index]==CENTER){
	
		sprintf(poseFir,"center\\");
	}
	else{
		sprintf(poseFir,"right\\");
	}

	sprintf(directory,"face\\%s%s%s_%d.jpg",sexDir,poseFir,outputFilename,faceCounter);
	faceCounter++;
	printf("%s\n",directory);
	//getchar();
	cvSaveImage(directory,normalizeImg);
	cvNamedWindow("Img",1);
	cvShowImage("Img",normalizeImg);
	cvWaitKey(5);
	cvReleaseImage(&normalizeImg);



};
bool  FaceCrop::Crop(CvPoint LE , CvPoint RE , CvPoint No , CvPoint Mo){
	
	CvPoint eyeVector;
	double rotation=0.0;
	IplImage* rotationImg=cvCreateImage(cvSize(srcImg->width,srcImg->height),IPL_DEPTH_8U,3);
	double eyeDistance;

	eyeVector.x=LE.x-RE.x;
	eyeVector.y=LE.y-RE.y;
	eyeDistance=sqrt((double)(eyeVector.x*eyeVector.x + eyeVector.y*eyeVector.y));
	rotation=atan2((double)eyeVector.y , (double)eyeVector.x) * 180 / CV_PI+180;
	

	CvMat *matrix=NULL;
	if(rotation > maxRotationAngle){
		matrix=cvCreateMat(2,3,CV_32FC1);
		matrix=cv2DRotationMatrix(cvPoint2D32f(LE.x,LE.y),rotation,1,matrix);
		cvWarpAffine( srcImg,rotationImg,matrix,CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );
	}
	else{
		cvCopy(srcImg,rotationImg);
	}
	

	eyeDistance=(int)eyeDistance;
	int x=LE.x-(int)(a*eyeDistance);
	int y=LE.y-(int)(b*eyeDistance);
	int width= (int)(a*eyeDistance)+(int)(a*eyeDistance)+eyeDistance+1;
	int height=(int)(b*eyeDistance)+(int)(c*eyeDistance)+1;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x+width>=rotationImg->width)width=rotationImg->width-x-1;
	if(y+height>=rotationImg->height)height=rotationImg->height-y-1;

	cvSetImageROI(rotationImg,cvRect(x , y , width , height));
	cropImg=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
	cvCopy(rotationImg,cropImg);
	cvResetImageROI(rotationImg);

	normalizeImg=cvCreateImage(cvSize(normalizeWidth,normalizeHeight),IPL_DEPTH_8U,3);
	cvResize(cropImg,normalizeImg);
	
	if(matrix!=NULL){

		matrix=cv2DRotationMatrix(cvPoint2D32f(LE.x,LE.y),-rotation,1,matrix);

		CvMat *pointMatrix=cvCreateMat(3,1,CV_32FC1);
		CvMat *rotatedPointMatrix=cvCreateMat(2,1,CV_32FC1);
		cvmSet(pointMatrix,0,0,x);
		cvmSet(pointMatrix,1,0,y);
		cvmSet(pointMatrix,2,0,1);
		cvmMul(matrix,pointMatrix,rotatedPointMatrix);
		leftTop.x=cvmGet(rotatedPointMatrix,0,0);
		leftTop.y=cvmGet(rotatedPointMatrix,1,0);
		
		cvmSet(pointMatrix,0,0,x+width);
		cvmSet(pointMatrix,1,0,y);
		cvmMul(matrix,pointMatrix,rotatedPointMatrix);
		rightTop.x=cvmGet(rotatedPointMatrix,0,0);
		rightTop.y=cvmGet(rotatedPointMatrix,1,0);
		
		cvmSet(pointMatrix,0,0,x);
		cvmSet(pointMatrix,1,0,y+height);
		cvmMul(matrix,pointMatrix,rotatedPointMatrix);
		leftdown.x=cvmGet(rotatedPointMatrix,0,0);
		leftdown.y=cvmGet(rotatedPointMatrix,1,0);
		
		cvmSet(pointMatrix,0,0,x+width);
		cvmSet(pointMatrix,1,0,y+height);
		cvmMul(matrix,pointMatrix,rotatedPointMatrix);
		rightdown.x=cvmGet(rotatedPointMatrix,0,0);
		rightdown.y=cvmGet(rotatedPointMatrix,1,0);
	}
	else{
		leftTop.x=x;
		leftTop.y=y; 
		rightTop.x=x+width;
		rightTop.y=y;
		leftdown.x=x;
		leftdown.y=y+height;
		rightdown.x=x+width;
		rightdown.y=y+height;
	}
	
	//cvSaveImage("result.jpg",cropImg);

	//cvNamedWindow("Img",1);
	//cvShowImage("Img",cropImg);
	//cvWaitKey(0);

	cvReleaseImage(&rotationImg);
	cvReleaseImage(&cropImg);

	return true;

	
}
void  FaceCrop::drawPoint(){

	for(int i=0 ; i<fpSet.size() ; i++){
		cvSet2D(srcImg,fpSet[i].lefteye.y  , fpSet[i].lefteye.x,CV_RGB(255,255,255));
		cvSet2D(srcImg,fpSet[i].righteye.y , fpSet[i].righteye.x,CV_RGB(255,255,255));
		//cvCircle( srcImg, cvPoint(fpSet[i].lefteye.x  , fpSet[i].lefteye.y  ), 1, CV_RGB(255,255,255),1,8, 0 );
		//cvCircle( srcImg, cvPoint(fpSet[i].righteye.x , fpSet[i].righteye.y ), 1, CV_RGB(255,255,255),1,8, 0 );
		//cvCircle( srcImg, cvPoint(fpSet[i].nose.x     , fpSet[i].nose.y     ), 1, CV_RGB(255,255,255),1,8, 0 );
		//cvCircle( srcImg, cvPoint(fpSet[i].mouth.x    , fpSet[i].mouth.y    ), 1, CV_RGB(255,255,255),1,8, 0 );
	
	}

	
	cvNamedWindow("Img",1);
	cvShowImage("Img",srcImg);
	cvWaitKey(0);
	
}