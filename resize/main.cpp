#include<iostream>

#include <cv.h>    
#include <highgui.h>   
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;

#include"video_resize.hpp"

int main(int argc,char* argv[])
{	
	video_resize(argv[1]);
	
	VideoCapture capture("test.avi");  
    if(!capture.isOpened())  
        cout<<"Fail to open!"<<endl;  
        
    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
	cout<<"the total number of frames is "<<totalFrameNumber<<endl;

    Mat frame_middle;    
    char window_name[20]="frame_middle";
    
	capture.set( CV_CAP_PROP_POS_FRAMES,totalFrameNumber/2);
	capture.read(frame_middle);
	
	//-----------------ºáÖá£¨col£©Îªx, ×ÝÖá£¨row£©Îªy -----------------------------------------------------
	
	Point pt0,pt1;
	pt0.x=0;
	pt1.x=480;
	char text[10];
	

	for(int y=30;y<270;y+=30)
	{
		pt0.y=pt1.y=y;	 
		line(frame_middle, pt0, pt1, Scalar(0,0,255),2);
		sprintf(text,"%d",y);
		putText(frame_middle,text, pt0,CV_FONT_HERSHEY_DUPLEX,0.8f,Scalar( 0, 255, 0));
	}
	imshow(window_name,frame_middle);  
	waitKey(0);   
	
	return 0;	
}
