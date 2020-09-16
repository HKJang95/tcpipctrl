
// SendClientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "ConnectSocket.h"
#include "afxcmn.h"
#include "queue"

// CSendClientDlg 대화 상자
class CSendClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CSendClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SENDCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString			m_PortStr;
	CString			m_IpStr;
	CListBox		m_List;
	BOOL			m_receiveFlag;
	CString			m_strMessage;
	CString			m_strBright;
	CString			m_strExpose;
	ConnectSocket	m_Socket;
	BOOL			m_IsConnected;

public:
	CEdit			m_PortEdit;
	CIPAddressCtrl	m_IPEdit;
	CEdit			m_IsConnectEdit;
	
public:
	HANDLE			h_LogThread;
	HANDLE			h_LogTerminate;
	// Threads for log

	CRITICAL_SECTION mSc;

	std::queue<CString> m_rcvqueue;

public:
	afx_msg void OnBnClickedConnectbtn();
	afx_msg void OnBnClickedShoot();
	

private:
	void nowtime(CString* write);
	BOOL IsWrongInput(LPCTSTR input, UINT32 utype);
	

public:
	void RcvLogWrite();
	void InLog(LPCTSTR content);
	void OutLog(LPCTSTR content, INT32 errcode);
	afx_msg void OnDestroy();
	CEdit m_ExposureEdit;
};
