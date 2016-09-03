
int get_info(char*path)  
{  
    //open a video and check   
    VideoCapture capture(path);  
    if(!capture.isOpened())  
        cout<<"Fail to open!"<<endl;  
        
        
    //get frame number   
    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
    cout<<"Total frame number:"<<totalFrameNumber<<endl;  
  
    //set the starting and ending frame   
    long frameToStart = 0;
    long frameToEnd=400;  
    
    capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);  
    cout<<"Start frame:"<<frameToStart<<endl;    
  
    if(frameToEnd < frameToStart)  
    {  
        cout<<"ERROR:ending frame is smaller than starting frame!"<<endl;  
        return -1;  
    }  
    else  
    {  
        cout<<"End frame:"<<frameToEnd<<endl;  
    }  
  
  
    //get the FPS  
    double rate = capture.get(CV_CAP_PROP_FPS);  
    rate=30.0;   // seems rate is set to -nan, here we set it 30.0 manually.
    cout<<"FPS:"<<rate<<endl;  
  	
  	
  	//get the cols(weight) and rows(height) of each frame
	Mat frame;// to store each frame
	capture.read(frame);  
	cout<<"cols："<<frame.cols<<endl;
	cout<<"rows："<<frame.rows<<endl;
  
    //a bool to control the processing   
    bool stop = false;  
    
    //window to show the frame  
    namedWindow("Extracted frame");
      
    //time interval between two neighboring frames      
    int delay = 6000/rate;  
  
	//which frame we are processing   
    long currentFrame = frameToStart;  
    
    
    while(!stop)  
    {     
        if(!capture.read(frame))  
        {  
            cout<<"Failed to read the frame!"<<endl;  
            return -1;    
        }  
           
        imshow("Extracted frame",frame);  
      
        cout<<"currentFrame:"<<currentFrame<<endl;  
 
   
        int c = waitKey(delay);  
        
        //press esc to end or reach the end frame   
        if((char) c == 27 || currentFrame > frameToEnd)  
        {  
            stop = true;  
        }  
        
        //other key makes the proc pause   
        if( c >= 0)  
        {  
            waitKey(0);  
        } 
         
        currentFrame++;  
      
    } 
     
    //close the video file   
    capture.release();  
    waitKey(0);  
    return 0;  
}  
