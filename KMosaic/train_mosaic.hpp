#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include<fstream>
using namespace cv;
using namespace std;

//////////////////////////////////////////////////////// Train_Mosaic class declaration   /////////////////////////////////////////////////////////
class Train_Mosaic
{

private:
	char *m_video_path; // video path
	VideoCapture capture;
	
	//video information---------------------------------------------------
	int frame_w;
	int frame_h;
	
	double rate;
	long totalFrameNumber;
	
	//template区域参数,注：template在x轴上始终位于中心位置-----------------------------------------------------------
	float temp_aspect_ratio; //template宽高比        
	int temp_h,temp_w; //template的宽和高     
	int temp_x0,temp_y0;
	
	//------------------------------------------------------
	Mat frame;            //当前帧图像
	Mat frame_c1;	      //单色帧图像
	Mat temp;          //模板
	Mat temp_c1;       //单色模板
	Mat result;           //匹配结果map
	
	//-----------------配准结果---------------------------
	int *match_result;


	
public:
	Train_Mosaic(char* video_path,int h_min,int h_max)  // construction
		{
			//初始化视频文件路径
			m_video_path=video_path;
			
			//初始化模板参数
			temp_aspect_ratio=1.0;
			temp_h  = h_max-h_min;
			temp_w  = temp_h*temp_aspect_ratio; 		
			
			//load video----------------------------------------------------
			capture=VideoCapture(m_video_path);
			if(!capture.isOpened())
				{
					cout<<"Fail to load video!"<<endl;
					return ;
				}
			else cout<<"Success to load video!"<<endl;	
			
			rate = capture.get(CV_CAP_PROP_FPS);
			rate=30.0;
			
			totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
			match_result=(int*)malloc(sizeof(int)*totalFrameNumber);//用以存储配准结果的整型数组
			
			capture.read(frame);
			frame_w = frame.cols;
			frame_h = frame.rows;
			
			temp_x0=(frame_w-temp_w)/2;
			temp_y0=h_min;
						
			capture.release();//释放视频
		}
		
	void rough_mosaic();
	void finetuned_mosaic();
	void print_match_result(char*file_path)
		{
			ofstream ofile;
			ofile.open(file_path);
			for(int i=0;i<totalFrameNumber;i++)
				ofile<<match_result[i]<<endl;
			ofile.close();
		}

};

///////////////////////////////////////////////////////////  rough mosaic  //////////////////////////////////////////////////////////

void Train_Mosaic::rough_mosaic()
{

	
	
	capture=VideoCapture(m_video_path);
	int currentFrame=0;
		
	//利用while循环读取帧并进行拼接-----------------------------------------
	Rect rect(temp_x0,temp_y0, temp_w, temp_h); 
		                       //class Rect(x,y,w,h),(x,y)is the upperleft point and w,h are width & height
	frame(rect).copyTo(temp);
	
	while(true)
	{
		if(!capture.read(frame))
		{
			cout<<"Fail to read frame！"<<endl;
		}
		
		
		
		//图像帧灰度化
		frame_c1.create(frame.rows,frame.cols,CV_8UC1 );
		cvtColor(frame,frame_c1,CV_BGR2GRAY);
			
		temp_c1.create(temp.rows,temp.cols,CV_8UC1 );
		cvtColor(temp,temp_c1,CV_BGR2GRAY);
	
		//模板匹配
		result.create(frame.cols - temp.cols + 1, frame.rows - temp.rows + 1, CV_32FC1 );
		matchTemplate(temp_c1, frame_c1, result, CV_TM_SQDIFF_NORMED);

			//寻找极值
		double minVal,maxVal;
		Point minLoc,maxLoc;			
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	    match_result[currentFrame]=minLoc.x;
	    
	    //提取模板
		Rect rect(temp_x0,temp_y0, temp_w, temp_h); 
		                       //class Rect(x,y,w,h),(x,y)is the upperleft point and w,h are width & height
		frame(rect).copyTo(temp);
		
		currentFrame++;
		if(currentFrame >= totalFrameNumber)
			break;
	
	}

	//对配准结果中值滤波
	mid_filter(match_result,totalFrameNumber,11);
	
	//关闭视频文件
	capture.release();
	
	cout<<"Rough mosaic done!"<<endl;
}

