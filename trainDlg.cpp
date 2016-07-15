
// trainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "train.h"
#include "trainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtrainDlg 对话框



CtrainDlg::CtrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtrainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtrainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtrainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtrainDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CtrainDlg 消息处理程序

BOOL CtrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtrainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtrainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//粗匹配
void CtrainDlg::OnBnClickedButton1()
{
	//路径------------------------------------------------------------------
	char * chPath = "e:\\IMG_9061.MOV";

	//ROI区域参数-----------------------------------------------------------
    float fw = 1.0f/3.0f;              
	float fh = 1.8f/3.0f;
	int h_pos = -150;
	int w_pos = 0;

	//拼接偏移参数-----------------------------------------------------------
	int nPos_min = 10;           //静止最大偏移
	float fPosdifLimit = 5.0f;   //偏移误差范围
	m_nPosdifW = 20;
	m_nPosdifH = 20;

	//载入视频-------------------------------------------------------------
	VideoCapture capture(chPath);
	if(!capture.isOpened())
		AfxMessageBox(_T("Fail"));

	//设置开始帧,获取帧数和帧频--------------------------------------------
	bool stop = false;    //视频退出条件
	long frameToStart = 0;//初始帧频位置
	capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);
	double rate = capture.get(CV_CAP_PROP_FPS);
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);	
	int delay = 1000/rate;//两帧间的间隔时间:
		
	//利用while循环读取帧并进行拼接-----------------------------------------
	bool bActive = false;
	long currentFrame = frameToStart;
	Mat frame;            //当前帧图像
	Mat frame_t;          //拼接模板图像
	vector<int> vnPos;    //拼接偏移
	vector<bool> vbPos;   //拼接偏移标示
	while(!stop)
	{
		//读取下一帧
		if(!capture.read(frame))
		{
			AfxMessageBox(_T("Fail"));
		}
		
		//显示图像
		//imshow("train",frame);
		int w = frame.cols;
		int h = frame.rows;
		int w0 = int(w*(1.0f-fw)/2.0f)+w_pos;
		int h0 = int(h*(1.0f-fh)/2.0f)+h_pos;
		Point Loc;

		//step1.模板匹配----------------------------------------------------------------
		if(bActive)
		{
			//灰度化
			Mat frame_t_c1;
			frame_t_c1.create(frame_t.rows,frame_t.cols,CV_8UC1 );
			cvtColor(frame_t,frame_t_c1,CV_BGR2GRAY);
			Mat frame_c1;
			frame_c1.create(frame.rows,frame.cols,CV_8UC1 );
			cvtColor(frame,frame_c1,CV_BGR2GRAY);

			//梯度图
			/*
			Mat frame_t_g;
			frame_t_g.create(frame_t.rows,frame_t.cols,CV_8UC1 );
			Mat frame_g;
			frame_g.create(frame.rows,frame.cols,CV_8UC1 );
			for(int i=1;i<frame_t_g.rows-1;i++)
			{
				for(int j=1;j<frame_t_g.cols-1;j++)
				{
					int g1 = frame_t_c1.ptr<uchar>(i)[j-1] - frame_t_c1.ptr<uchar>(i)[j+1];
					int g2 = frame_t_c1.ptr<uchar>(i-1)[j] - frame_t_c1.ptr<uchar>(i+1)[j];
					int g = abs(g1)+abs(g2);
					if(g>255)
						g = 255;
					frame_t_g.ptr<uchar>(i)[j] =g;
				}
			}
			for(int i=1;i<frame_g.rows-1;i++)
			{
				for(int j=1;j<frame_g.cols-1;j++)
				{
					int g1 = frame_c1.ptr<uchar>(i)[j-1] - frame_c1.ptr<uchar>(i)[j+1];
					int g2 = frame_c1.ptr<uchar>(i-1)[j] - frame_c1.ptr<uchar>(i+1)[j];
					int g = abs(g1)+abs(g2);
					if(g>255)
						g = 255;
					frame_g.ptr<uchar>(i)[j] =g;
				}
			}*/


			//模板匹配
			int result_cols =  frame.cols - frame_t.cols + 1;
			int result_rows = frame.rows - frame_t.rows + 1;
			Mat result;
			result.create( result_rows, result_cols, CV_32FC1 );
			//matchTemplate(frame_t_g, frame_g, result, CV_TM_SQDIFF_NORMED);
			matchTemplate(frame_t_c1, frame_c1, result, CV_TM_SQDIFF_NORMED);

			//寻找极值
			double minVal,maxVal;
			Point minLoc,maxLoc;			
			minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
			//Loc = maxLoc;
			Loc = minLoc;
			int npos =  Loc.x - w0;
			vnPos.push_back(npos);
			vbPos.push_back(true);

			//显示位置
			Point p;
			p.x = Loc.x + frame_t.cols;
			p.y = Loc.y + frame_t.rows;
			Mat show;
			frame.copyTo(show);
			rectangle(show,Loc,p,Scalar(0,0,255),2,8,0); //在检测位置画红色框
			imshow("match",show);
		}		
	    

		//step2.提取模板----------------------------------------------------------------		
		Rect rect(w0, h0, int(w*fw), int(h*fh));    
		frame(rect).copyTo(frame_t);
		bActive = true;	
        //imshow("t",frame_t);		
		

		//按下ESC或者到达指定的结束帧后退出读取视频
		int c = waitKey(delay);
		if((char) c == 27 || currentFrame >= totalFrameNumber-1)
		//if((char) c == 27 || currentFrame >= 150)
		{
			stop = true;
		}
		//按下按键后会停留在当前帧，等待下一次按键
		if( c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;
	
	}
	
	//统计偏移位置-------------------------------------------------------------------------
	//int n = vnPos.size();
	////剔除静止位置
	//for(int i=0;i<n;i++)
	//{
	//	if(abs(vnPos[i])<=nPos_min)
	//		vbPos[i] = false;
	//}
	////迭代剔除跳变点
	//float fAve = 0.0f;
	//float fDif = 0.0f;
	//int num = 0;
	//while(1)
	//{
	//	//计算均值
	//	fAve = 0.0f;
	//	num = 0;
	//	for(int i=0;i<n;i++)
	//	{
	//		if(vbPos[i])
	//		{
	//			fAve += vnPos[i];
	//			num++;
	//		}
	//	}
	//	fAve = fAve/float(num);

	//	//剔除最大跳变点
	//	float fMaxDif = 0.0f;
	//	int nMaxDifIndex = 0;
	//	for(int i=0;i<n;i++)
	//	{
	//		if(vbPos[i])
	//		{
	//			float fTmp = abs(vnPos[i]-fAve);
	//			if(fTmp>fMaxDif)
	//			{
	//				fMaxDif = fTmp;
	//				nMaxDifIndex = i;
	//			}
	//		}
	//	}
	//	vbPos[nMaxDifIndex] = false;

	//	//计算偏移误差
	//	fDif = 0.0f;
	//	num = 0;
	//	for(int i=0;i<n;i++)
	//	{
	//		if(vbPos[i])
	//		{
	//			fDif += abs(vnPos[i]-fAve);
	//			num++;
	//		}
	//	}
	//	fDif = fDif/float(num);

	//	//跳出条件
	//	if(fDif<fPosdifLimit)
	//	{
	//		break;
	//	}
	//}
	
	int nAve = FindPos(&vnPos,nPos_min,fPosdifLimit,&vbPos);

	//二次剔除跳变点
	int n = vnPos.size();
	float fAveEx = 0.0f;
	int num = 0;
	for(int i=0;i<n;i++)
	{
		if(vbPos[i])
		{
			float fTmp = abs(vnPos[i]-nAve);
			if(fTmp>m_nPosdifW)
			{
				vbPos[i] = false;
			}
			else
			{
				fAveEx += vnPos[i];
				num++;
			}
		}
	}
	fAveEx = fAveEx/float(num);
	m_nPosBasic = int(fAveEx);

	//确定前后背景点-------------------------------------(设计的比较粗糙，以后可以加强增加鲁棒性)
	bool b0 = false;
	bool b1 = false;
	for(int i=0;i<n;i++)
	{
		if(!b0)
		{
			if(abs(vnPos[i])>nPos_min)
			{
				m_nStart = i;
				b0 = true;
			}
		}
		if(b0 && !b1)
		{
			if(abs(vnPos[i])<nPos_min)
			{
				m_nEnd = i-1;
				b1 = true;
			}
		}
	}

	//
	for(int i=0;i<n;i++)
		m_vnPos.push_back(vnPos[i]);

	
	int w = frame.cols;
	int h = frame.rows;
	int w0 = int(w*(1.0f-fw)/2.0f)+w_pos;
	int h0 = int(h*(1.0f-fh)/2.0f)+h_pos;
	Point p;
	p.x = w0 + frame_t.cols;
	p.y = h0 + frame_t.rows;
	Point p0;
	p0.x = w0;
	p0.y = h0;

	capture.set( CV_CAP_PROP_POS_FRAMES,m_nStart-1);
	capture.read(frame);
	rectangle(frame,p0,p,Scalar(0,0,255),2,8,0); //在检测位置画红色框
	imshow("frame1",frame);

	capture.set( CV_CAP_PROP_POS_FRAMES,m_nStart);
	capture.read(frame);
	rectangle(frame,p0,p,Scalar(0,0,255),2,8,0); //在检测位置画红色框
	imshow("frame2",frame);

	capture.set( CV_CAP_PROP_POS_FRAMES,m_nEnd);
	capture.read(frame);
	rectangle(frame,p0,p,Scalar(0,0,255),2,8,0); //在检测位置画红色框
	imshow("frame3",frame);

	capture.set( CV_CAP_PROP_POS_FRAMES,m_nEnd+1);
	capture.read(frame);
	rectangle(frame,p0,p,Scalar(0,0,255),2,8,0); //在检测位置画红色框
	imshow("frame4",frame);
	
	waitKey(0);

	//速度
	m_fLpp = 25.0f/5282.0f;     //每节车厢25m
	m_fV = m_fLpp*m_nPosBasic/float(delay/1000.0f);
	m_fV = m_fV*3.6f;

	//关闭视频文件
	capture.release();
	CString strV;
	strV.Format(_T("%f km/h"),m_fV);
	AfxMessageBox(strV);
}

//细匹配
void CtrainDlg::OnBnClickedButton2()
{
	//m_nStart = 47;
	//m_nEnd = 576;
	//m_nPosBasic = 156;

	// TODO: 在此添加控件通知处理程序代码
	//路径------------------------------------------------------------------
	char * chPath = "e:\\IMG_9061.MOV";

	//ROI区域参数-----------------------------------------------------------
    float fw = 1.0f/3.0f;              
	float fh = 1.8f/3.0f;
	int h_pos = -150;
	int w_pos = 0;

	//参数-----------------------------------------------------------------
	m_fLpp = 25.0f/5282.0f;     //每节车厢25m
	int rols_gray_th = 20;      //剪裁模板参数1 （备用方案可以用背景做变化检测更可靠）
	float rols_num_th = 0.3f;   //剪裁模板参数2
	m_nPosdifW = 3;
	m_nPosdifH = 10;
	int nPos_min = 10;           //静止最大偏移
	float fPosdifLimit = 2.0f;   //偏移误差范围
	int nFrameNum = 50;          //稳定帧频的帧长

	//载入视频-------------------------------------------------------------
	VideoCapture capture(chPath);
	if(!capture.isOpened())
		AfxMessageBox(_T("Fail"));

	//设置开始帧,获取帧数和帧频--------------------------------------------
	bool stop = false;    //视频退出条件	
	capture.set( CV_CAP_PROP_POS_FRAMES,0); //初始帧频位置
	double rate = capture.get(CV_CAP_PROP_FPS);
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);	
	int delay = 1000/rate;//两帧间的间隔时间:
		
	//利用while循环读取帧并进行拼接-----------------------------------------
	bool bActive = false;
	bool bCutT = true;
	long currentFrame = 0;
	Mat frame;            //当前帧图像
	Mat frame_t;          //拼接模板图像

	//获取背景图像	
	Mat frame_bg_c1;      //背景图像
	capture.set(CV_CAP_PROP_POS_FRAMES,m_nStart-1); 
	capture.read(frame);
	int w = frame.cols;
	int h = frame.rows;
	int w0 = int(w*(1.0f-fw)/2.0f)+w_pos;
	int h0 = int(h*(1.0f-fh)/2.0f)+h_pos;
	int	rows = int(h*fh);
	Mat tmp;
	Rect rect_bg(w0, h0, int(w*fw), int(h*fh));
	frame(rect_bg).copyTo(tmp);
	cvtColor(tmp,frame_bg_c1,CV_BGR2GRAY);
	//imshow("frame_bg_c1",frame_bg_c1);
	//cvWaitKey(0);

	capture.set(CV_CAP_PROP_POS_FRAMES,0); //复位

	vector <int> vnPos;      //匹配位置
	vector <Mat> vFrameROI;  //roi区域

	//标示
	CvFont font;    
    double hScale=1;   
    double vScale=0.5;    
    int lineWidth=2;    
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//初始化字体，准备写到图片上的     
	USES_CONVERSION;

	while(!stop)
	{
		//读取下一帧
		if(!capture.read(frame))
		{
			AfxMessageBox(_T("Fail"));
		}
		
		//显示图像
		//imshow("train",frame);	
		Point Loc;

		//step1.模板匹配----------------------------------------------------------------
		if(bActive && currentFrame>=m_nStart+1 &&  currentFrame<=m_nEnd)
		{
			//灰度化
			Mat frame_t_c1;
			frame_t_c1.create(frame_t.rows,frame_t.cols,CV_8UC1 );
			cvtColor(frame_t,frame_t_c1,CV_BGR2GRAY);
			Mat frame_c1;
			frame_c1.create(frame.rows,frame.cols,CV_8UC1 );
			cvtColor(frame,frame_c1,CV_BGR2GRAY);

			//目标进入剪裁
			Mat frame_t_cut_c1;
			int w_LR = 0;
			int j_th_L = 0;
			int j_th_R = 0;
			//if(bCutT)
			{
				//剪裁模板				
				for(int j=0;j<frame_t.cols;j++)			
				{
					int nCount = 0;
					for(int i=0;i<frame_t.rows;i++)
					{
						float diff = abs(frame_t_c1.ptr<uchar>(i)[j] - frame_bg_c1.ptr<uchar>(i)[j]);
						if(diff>rols_gray_th)
							nCount++;
					}
					if(nCount>int(rols_num_th*frame_t.rows))
					{
						j_th_L = j;
						break;
					}
				}
				for(int j=frame_t.cols-1;j>=0;j--)			
				{
					int nCount = 0;
					for(int i=0;i<frame_t.rows;i++)
					{
						float diff = abs(frame_t_c1.ptr<uchar>(i)[j] - frame_bg_c1.ptr<uchar>(i)[j]);
						if(diff>rols_gray_th)
							nCount++;
					}
					if(nCount>int(rols_num_th*frame_t.rows))
					{
						j_th_R = j;
						break;
					}
				}
				w_LR = j_th_R-j_th_L+1;				
			}
			//else
			//{
			//	j_th_L = 0;
			//	j_th_R = frame_t.cols-1;
			//	w_LR = j_th_R-j_th_L+1;
			//}
			//if(w_LR==frame_t.cols)
			//	bCutT = false;
			frame_t_cut_c1.create(frame_t.rows,w_LR,CV_8UC1 );
			Rect rect_cut(j_th_L, 0, w_LR, frame_t.rows);
			frame_t_c1(rect_cut).copyTo(frame_t_cut_c1);
			//imshow("frame_t_cut_c1",frame_t_cut_c1);
			//waitKey(0);

			//保存车头
			if(currentFrame==m_nStart+1)
			{
				Mat head;
				frame_t(rect_cut).copyTo(head);
				vFrameROI.push_back(head);
			}

			//限定匹配范围
			vector <int> vnpos;
			vector <bool> vbpos;
			int n1 = currentFrame-nFrameNum/2;
			if(n1<m_nStart)
				n1 = m_nStart+1;
			int n2 = n1 + nFrameNum -1;
			if(n2>m_nEnd)
				n2 = m_nEnd-1;
			n1 = n2 - nFrameNum +1;
			for(int i=n1;i<=n2;i++)
			{
				vnpos.push_back(m_vnPos[i]);
				vbpos.push_back(true);
			}
			int nPosBasic = FindPos(&vnpos,nPos_min,fPosdifLimit,&vbpos);		
			int nw_pos = w0+nPosBasic-m_nPosdifW;
			//int nw_pos = w0+m_nPosBasic-m_nPosdifW;
			int nh_pos = h0-m_nPosdifH;
			Mat frame_limit_c1;
			Rect rect_limit(nw_pos, nh_pos, int(w*fw)+2*m_nPosdifW, int(h*fh)+2*m_nPosdifH);    
			frame_c1(rect_limit).copyTo(frame_limit_c1);
			//imshow("frame_limit_c1",frame_limit_c1);
			//cvWaitKey(0);

			//梯度
			/*Mat frame_t_g;
			frame_t_g.create(frame_t_cut_c1.rows,frame_t_cut_c1.cols,CV_8UC1 );
			Mat frame_g;
			frame_g.create(frame_limit_c1.rows,frame_limit_c1.cols,CV_8UC1 );
			for(int i=1;i<frame_t_g.rows-1;i++)
			{
				for(int j=1;j<frame_t_g.cols-1;j++)
				{
					int g1 = frame_t_cut_c1.ptr<uchar>(i)[j-1] - frame_t_cut_c1.ptr<uchar>(i)[j+1];
					int g2 = frame_t_cut_c1.ptr<uchar>(i-1)[j] - frame_t_cut_c1.ptr<uchar>(i+1)[j];
					int g = abs(g1)+abs(g2);
					if(g>255)
						g = 255;
					frame_t_g.ptr<uchar>(i)[j] =g;
				}
			}
			for(int i=1;i<frame_g.rows-1;i++)
			{
				for(int j=1;j<frame_g.cols-1;j++)
				{
					int g1 = frame_limit_c1.ptr<uchar>(i)[j-1] - frame_limit_c1.ptr<uchar>(i)[j+1];
					int g2 = frame_limit_c1.ptr<uchar>(i-1)[j] - frame_limit_c1.ptr<uchar>(i+1)[j];
					int g = abs(g1)+abs(g2);
					if(g>255)
						g = 255;
					frame_g.ptr<uchar>(i)[j] =g;
				}
			}*/
			
			//模板匹配
			int result_cols =  frame_limit_c1.cols - frame_t_cut_c1.cols + 1;
			int result_rows = frame_limit_c1.rows - frame_t_cut_c1.rows + 1;
			Mat result;
			result.create( result_rows, result_cols, CV_32FC1 );
			//matchTemplate(frame_t_g, frame_g, result, CV_TM_SQDIFF_NORMED);
			//matchTemplate(frame_t_cut_c1, frame_limit_c1, result, CV_TM_CCOEFF_NORMED);
			matchTemplate(frame_t_cut_c1, frame_limit_c1, result, CV_TM_SQDIFF_NORMED);

			//寻找极值
			double minVal,maxVal;
			Point minLoc,maxLoc;			
			minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
			//Loc = maxLoc;
			Loc = minLoc;

			//剪切帧频
			int npos =  nw_pos + Loc.x - (w0+j_th_L);
			vnPos.push_back(npos);

			Mat ROI;
			Rect rectROI;
			rectROI.y = nh_pos + Loc.y -1; //为何老是向下移动1个像素？
			rectROI.height = int(h*fh);	
			if(m_nPosBasic>0)   //左到右
			{
				rectROI.x = w0;
				rectROI.width = npos;
			}
			else                //右到左
			{
				rectROI.x = w0+frame_t.cols-npos-1;
				rectROI.width = npos;
			}
			frame(rectROI).copyTo(ROI);

			//标示
			IplImage * t_rs = cvCreateImage(ROI.size(),8,3);
			int hh = ROI.size().height;
			int ww = ROI.size().width;
			for(int i=0;i<hh;i++)
			{
				for(int j=0;j<ww;j++)
				{
					((uchar*)( t_rs->imageData + i * t_rs->widthStep))[j] = ROI.ptr<uchar>(i)[j];
				}
			}
			CString strw;
			strw.Format(_T("%d"),currentFrame);
			char * chw1 = T2A(strw);
			cvPutText(t_rs,chw1,cvPoint(0,50),&font,CV_RGB(255,0,0));//在图片中输出字符			
			strw.Format(_T("%d"),npos);
			char * chw2 = T2A(strw);
			cvPutText(t_rs,chw2,cvPoint(0,150),&font,CV_RGB(255,0,0));//在图片中输出字符
			
			for(int i=0;i<hh;i++)
			{
				for(int j=0;j<ww;j++)
				{
					ROI.ptr<uchar>(i)[j] = ((uchar*)( t_rs->imageData + i * t_rs->widthStep))[j];
				}
			}
			cvReleaseImage(&t_rs);

			//载入
			vFrameROI.push_back(ROI);	

			//显示位置
			Point p1,p2;
			p1.x = nw_pos + Loc.x;
			p1.y = nh_pos + Loc.y;
			p2.x = nw_pos + Loc.x + frame_t.cols;
			p2.y = nh_pos + Loc.y + frame_t.rows;
			Mat show;
			frame.copyTo(show);
			rectangle(show,p1,p2,Scalar(0,0,255),2,8,0); //在检测位置画红色框
			imshow("match",show);
			//waitKey(0);
		}		
	    

		//step2.提取模板----------------------------------------------------------------		
		Rect rect(w0, h0, int(w*fw), int(h*fh));    
		frame(rect).copyTo(frame_t);
		bActive = true;	
        //imshow("t",frame_t);		
		

		//按下ESC或者到达指定的结束帧后退出读取视频
		int c = waitKey(delay);
		if((char) c == 27 || currentFrame >= totalFrameNumber-1)
		//if((char) c == 27 || currentFrame >= 150)
		{
			stop = true;
		}
		//按下按键后会停留在当前帧，等待下一次按键
		if( c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;
	
	}
	

	//step3.拼合车厢--------------------------------------------------------------------
	Mat dst;                 //拼接的图像
	int ns = vFrameROI.size();	
	int clos = 0;
	for(int k=0;k<ns;k++)
	{
		clos += vFrameROI[k].cols;		
	}
	dst.create(rows,clos,CV_8UC3);

	if(m_nPosBasic>0)   //左到右
	{
		int nTmp = 0;
		for(int k=0;k<ns;k++)
		{
			int j_pos = 0;
			nTmp += vFrameROI[k].cols;
			j_pos = clos - nTmp;
			for(int i=0;i<vFrameROI[k].rows;i++)
			{
				for(int j=0;j<vFrameROI[k].cols;j++)
				{
					dst.ptr<uchar>(i)[(j+j_pos)*3] = vFrameROI[k].ptr<uchar>(i)[j*3];
					dst.ptr<uchar>(i)[(j+j_pos)*3+1] = vFrameROI[k].ptr<uchar>(i)[j*3+1];
					dst.ptr<uchar>(i)[(j+j_pos)*3+2] = vFrameROI[k].ptr<uchar>(i)[j*3+2];
				}
			}		
		}
	}
	else               //右到左
	{
		int j_pos = 0;
		for(int k=0;k<ns;k++)
		{
			for(int i=0;i<vFrameROI[k].rows;i++)
			{
				for(int j=0;j<vFrameROI[k].cols;j++)
				{
					dst.ptr<uchar>(i)[(j+j_pos)*3] = vFrameROI[k].ptr<uchar>(i)[j*3];
					dst.ptr<uchar>(i)[(j+j_pos)*3+1] = vFrameROI[k].ptr<uchar>(i)[j*3+1];
					dst.ptr<uchar>(i)[(j+j_pos)*3+2] = vFrameROI[k].ptr<uchar>(i)[j*3+2];
				}
			}
			j_pos += vFrameROI[k].cols;
		}
	}	
	imwrite("e:\\train.bmp",dst);	

	//关闭视频文件
	capture.release();
	AfxMessageBox(_T("ok!"));
}

//修正
int CtrainDlg::FindPos(vector <int>  * pvnPos,int nPos_min,float fPosdifLimit,vector <bool>  * pvbPos)

{
	int n = pvnPos->size();
	//剔除静止位置
	for(int i=0;i<n;i++)
	{
		if(abs((*pvnPos)[i])<=nPos_min)
		{
			if(pvbPos!=NULL)
				(*pvbPos)[i] = false;
		}
	}
	//迭代剔除跳变点
	float fAve = 0.0f;
	float fDif = 0.0f;
	int num = 0;
	while(1)
	{
		//计算均值
		fAve = 0.0f;
		num = 0;
		for(int i=0;i<n;i++)
		{
			if((*pvbPos)[i])
			{
				fAve += (*pvnPos)[i];
				num++;
			}
		}
		fAve = fAve/float(num);

		//剔除最大跳变点
		float fMaxDif = 0.0f;
		int nMaxDifIndex = 0;
		for(int i=0;i<n;i++)
		{
			if((*pvbPos)[i])
			{
				float fTmp = abs((*pvnPos)[i]-fAve);
				if(fTmp>fMaxDif)
				{
					fMaxDif = fTmp;
					nMaxDifIndex = i;
				}
			}
		}
		(*pvbPos)[nMaxDifIndex] = false;

		//计算偏移误差
		fDif = 0.0f;
		num = 0;
		for(int i=0;i<n;i++)
		{
			if((*pvbPos)[i])
			{
				fDif += abs((*pvnPos)[i]-fAve);
				num++;
			}
		}
		fDif = fDif/float(num);

		//跳出条件
		if(fDif<fPosdifLimit)
		{
			break;
		}
	}
	
	return int(fAve);
}