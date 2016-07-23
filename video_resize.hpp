  #include <cv.h>    
#include <highgui.h>    
  
int resize()    
{    
    CvCapture* capture = cvCaptureFromAVI("test.MOV");    
  
    IplImage* frame = 0;    
  
    int isColor = 1;      
    int fps     = 30; // or 25  
    CvVideoWriter *writer = cvCreateVideoWriter("result.avi",CV_FOURCC('X','V','I','D'),fps,cvSize(480,270),isColor);      
  
    cvNamedWindow( "vivi");    
    //读取和显示    
    while(1)    
    {    
        frame = cvQueryFrame(capture); //获取一帧图片    
        if(frame == NULL)    
            break;    
  
        CvSize dstSize = cvSize(480, 270);  
        IplImage *dst = cvCreateImage(dstSize, frame->depth, frame->nChannels);  
  
        cvResize( frame, dst, CV_INTER_LINEAR );   
        cvShowImage( "vivi", dst ); //将其显示    
        char key = cvWaitKey(10);    
          
        cvWriteFrame(writer, dst);  
        frame = NULL;  
        cvReleaseImage(&dst);  
    }    
  
    cvReleaseVideoWriter(&writer);      
    cvDestroyWindow("vivi");    
  
    return 0;    
}    
