#include <iostream>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

   
int main( int argc, char** argv )  
{  
    //声明IplImage指针  
    IplImage* pFrame = NULL;  
    IplImage* img;  
  
    //获取摄像头  
    CvCapture* pCapture = cvCreateCameraCapture(-1);  
  
    //创建窗口  
    cvNamedWindow("video", 1);  
    CvVideoWriter *writer = NULL;  
    int isColor = 1;  
    int fps = 25; // or 30  
    int frameW = 640; //   
    int frameH = 480;  
      
    writer=cvCreateVideoWriter(argv[1],CV_FOURCC('X','V','I','D'),fps,cvSize(frameW,frameH),isColor);  
  
    //显示视屏  
    while(1)  
    {  
        pFrame=cvQueryFrame( pCapture );  
        if(!pFrame)break;  
        cvShowImage("video",pFrame);  
        cvGrabFrame(pCapture); // capture a frame  
        img = cvRetrieveFrame(pCapture); // retrieve the captured frame  
        cvWriteFrame(writer,img); // add the frame to the file  
        char c=cvWaitKey(10);  
        if(c==27)break;  
    }  
        cvReleaseImage(&pFrame);  
        cvReleaseImage(&img);  
        cvReleaseVideoWriter(&writer);  
        cvReleaseCapture(&pCapture);  
        cvDestroyWindow("video");  
}  
