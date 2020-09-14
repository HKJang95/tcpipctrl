
// TCPIPDlg.h : ��� ����
//

#pragma once
#include "ListenSocket.h"
#include "CaneraControl.h"
#include "SerialPort.h"
#include "afxwin.h"
#include "ctime"


// CTCPIPDlg ��ȭ ����
class CTCPIPDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTCPIPDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TCPIP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
	afx_msg void OnDestroy();
	void ActionControl();
	void opLog(CString ip, UINT uport, CString content);

private:
	BOOL RawToBmp();
	void DrawImage();
	BOOL BmpSave();
	void AddCompleteMsg(INT32 errcode, CString* errMsg);
	void sendCompleteMsg(LPCTSTR errMsg);
	BOOL LightControl(int brightness);
	void BtnToggle(BOOL toggle);
	void BtnToggle(BOOL LightToggle, BOOL CameraToggle, BOOL ServerToggle, BOOL ImgPathToggle, BOOL ExposeToggle);
	void opLog(CString content);
	void nowtime(CString* timestr);

public:
	CListenSocket m_ListenSocket;
	
	CString			m_ReceiveMsg;
	CString			m_ReceiveIP;
	UINT			m_ReceivePort;

	CString			m_SerialSend;
	CString			m_BaudRate;
	CString			m_Port;
	CString			m_Bright;
	CString			m_PathName;

	BOOL			m_IsSocketRcv;
	BOOL			m_IsSerialOpen;
	BOOL			m_IsServerOpen;
	BOOL			m_IsCameraOpen;
	BOOL			m_IsLightOn;

	Graphics*		m_pGraphics;
	HDC				m_hDC;
	CRect			m_rcDisp;

	Bitmap*			m_pBitmap;

	SerialPort*		m_Comm;

	CaneraControl*  m_pCamera;

	CRITICAL_SECTION mSc;
	afx_msg void OnBnClickedImgpath();
	CEdit m_PathEdit;
	afx_msg void OnBnClickedCambtn();
	afx_msg void OnBnClickedLightbtn();
	afx_msg void OnBnClickedServerbtn();
	CComboBox m_PortCombo;
	CComboBox m_BaudCombo;
	CEdit m_ServerPortEdit;
	CStatic m_CapImg;
	CEdit m_ExposeEdit;
	afx_msg void OnBnClickedExposeset();
};
