#include<iostream>
#include"train_mosaic.hpp"

#include"get_info.hpp"
#include"video_resize.hpp"
#include"get_roi.hpp"

using namespace std;
int main(int argc,char* argv[])
{
	//char video_path[]="test.avi";
	//get_info(video_path);
	
	//if(argv[2]) 
	//	video_resize(argv[1],argv[2]);
	
	//get_roi(argv[1]);
	
	
	
	int h_min,h_max;
	cout<<"input two integer:h_min,h_max..."<<endl;
	cin>>h_min>>h_max;
	
	Train_Mosaic train_mosaic("test.avi",h_min,h_max);
	
	cout<<"beginning mosaic..."<<endl;
	train_mosaic.rough_mosaic();
	train_mosaic.finetuned_mosaic();
	
	return 0;	
}
