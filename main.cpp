#include<iostream>
#include"train_mosaic.hpp"
#include"get_info.hpp"
#include"video_resize.hpp"

using namespace std;
int main()
{
	char video_path[]="test.avi";
	//get_info(video_path);
	//video_resize();
	Train_Mosaic train_mosaic(video_path);
	cout<<"beginning mosaic..."<<endl;
	train_mosaic.rough_mosaic();
	train_mosaic.finetuned_mosaic();

	return 0;	
}
