#include<cstdlib>
#include <cv.h>    
#include <highgui.h>    
  
int get_roi(char *video_path)    
{    
	VideoCapture capture;
    capture=VideoCapture(video_path);
    Mat frame_1,frame_2,frame_absdiff;
    
    char window_name[20]="frame_absdiff";
    
  
	capture.set( CV_CAP_PROP_POS_FRAMES,350);	
	capture.read(frame_1);
	capture.read(frame_2);
	absdiff(frame_1,frame_2,frame_absdiff);
	cvtColor(frame_absdiff,frame_absdiff,CV_BGR2GRAY);		
	//threshold(frame_absdiff, frame_absdiff, 20, 255, THRESH_BINARY);
	
	int h_min=0,h_max=0;
	for(int x=0;x<270;x++)
		for(int y=235;y<=245;y+=5)
		{
			cout<<frame_absdiff.at<int>(x,y)<<endl;
		/*	if(frame_absdiff.at<short>(x,y)==0)
					;
			else 
			{
				h_max=x;
				if(h_min==0)
					h_min=x;
			}
			*/

		}
	cout<<h_min<<endl;
	cout<<h_max<<endl;
	/*		
		
	Point p;
	p.x = h_min;
	p.y = 240-(h_max-h_min)/2;

	Point p0;
	p0.x = h_max;
	p0.y = 240+(h_max-h_min)/2;

	rectangle(frame_absdiff,p,p0,Scalar(0,0,255),2,8,0); 
  */
	imshow(window_name,frame_absdiff);

 	waitKey(0);
  
    return 0;    
}    
