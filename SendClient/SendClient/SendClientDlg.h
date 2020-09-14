
// SendClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "ConnectSocket.h"
#include "afxcmn.h"

// CSendClientDlg ��ȭ ����
class CSendClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSendClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SENDCLIENT_DIALOG };

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
	CString			m_PortStr;
	CString			m_IpStr;
	CString			m_rcvMessage;
	CListBox		m_List;
	BOOL			m_receiveFlag;
	CString			m_strMessage;
	CString			m_strBright;
	CString			m_strExpose;
	ConnectSocket	m_Socket;
	CEdit			m_PortEdit;
	CIPAddressCtrl	m_IPEdit;
	CEdit			m_IsConnectEdit;
	BOOL			m_IsConnected;

	CRITICAL_SECTION mSc;

public:
	afx_msg void OnBnClickedConnectbtn();
	afx_msg void OnBnClickedShoot();

private:
	void nowtime(CString* write);
	BOOL IsWrongInput(LPCTSTR input, UINT32 utype);

public:
	void InLog(LPCTSTR content);
	void OutLog(LPCTSTR content, INT32 errcode);
	afx_msg void OnDestroy();
	CEdit m_ExposureEdit;
};
