
// trainDlg.h : 头文件
//

#pragma once
#include "cv.h"
#include "highgui.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
using namespace cv;
using namespace std;

// CtrainDlg 对话框
class CtrainDlg : public CDialogEx
{
// 构造
public:
	CtrainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	//int FindPos(vector <int>  * pvnPos,int nPos_min,float fPosdifLimit);
	int FindPos(vector <int>  * pvnPos,int nPos_min,float fPosdifLimit,vector <bool>  * pvbPos = NULL);

public:
	int m_nStart;     //起始帧频数
	int m_nEnd;       //终止帧频数
	int m_nPosBasic;  //匹配偏移基数
	int m_nPosdifW;   //配准偏移幅度
	int m_nPosdifH;
	float m_fLpp;     //每个像素物理尺寸
	float m_fV;       //速度
	vector <int> m_vnPos; //偏移容器
};
