
// SendClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SendClient.h"
#include "SendClientDlg.h"
#include "afxdialogex.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define SOCKET_SEND 1
#define SOCKET_TIMEOUT 2

#define BRIGHT 10
#define EXPOSE 11



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CSendClientDlg ��ȭ ����



CSendClientDlg::CSendClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendClientDlg::IDD, pParent)
	, m_receiveFlag(FALSE)
	, m_strMessage(_T(""))
	, m_IpStr(_T(""))
	, m_PortStr(_T(""))
	, m_IsConnected(FALSE)
	, m_rcvMessage(_T(""))
	, m_strBright(_T(""))
	, m_strExpose(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PORTEDIT, m_PortEdit);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPEdit);
	DDX_Control(pDX, IDC_EXPOSUREEDIT, m_ExposureEdit);
}

BEGIN_MESSAGE_MAP(CSendClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHOOT, &CSendClientDlg::OnBnClickedShoot)
	ON_BN_CLICKED(IDC_CONNECTBTN, &CSendClientDlg::OnBnClickedConnectbtn)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSendClientDlg �޽��� ó����

BOOL CSendClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_PortEdit.SetWindowTextW(_T("21000"));
	m_IPEdit.SetWindowTextW(_T("192.168.20.34"));

	SetDlgItemTextW(IDC_BRIGHTEDIT, _T("5"));
	SetDlgItemTextW(IDC_EXPOSUREEDIT, _T("60000"));

	GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
	::InitializeCriticalSection(&mSc);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSendClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSendClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSendClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendClientDlg::OnBnClickedShoot()
{
	CString strErr = _T("");
	m_strMessage = _T("");
	m_strBright = _T("");
	m_strExpose = _T("");
	
	UpdateData(TRUE);
	
	GetDlgItemTextW(IDC_BRIGHTEDIT, m_strBright);
	GetDlgItemTextW(IDC_EXPOSUREEDIT, m_strExpose);

	if (!IsWrongInput(m_strBright, BRIGHT))
	{
		strErr.Format(_T("%s�� �߸��� ���Դϴ�. 0~255 ������ ���ڸ� �Է����ּ���."), m_strMessage);
		AfxMessageBox(strErr);
		return;
	}
	else if (!IsWrongInput(m_strExpose, EXPOSE))
	{
		strErr.Format(_T("%s�� �߸��� ���Դϴ�. 0~255 ������ ���ڸ� �Է����ּ���."), m_strMessage);
		AfxMessageBox(strErr);
		return;
	}

	m_strMessage.Format(_T("%s#%s"), m_strBright, m_strExpose);

	SetTimer(10, 10000, NULL);
	OutLog(m_strMessage, SOCKET_SEND);

	
	for (int i = 0; i < 5; i++)
	{
		Sleep(5);
		if (!m_Socket.Send(m_strMessage, m_strMessage.GetLength() * 2))
		{
			KillTimer(10);
			OutLog(m_strMessage, SOCKET_TIMEOUT);
		}
	}
	

	

	GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSendClientDlg::InLog(LPCTSTR content)
{
	CString time = _T("");
	CString logstr = _T("");
	CString brightstr = _T("");
	CString exposestr = _T("");
	CString in = content;
	CString temp = _T("");
	int successStart = 0;
	int successEnd = 0;
	
	EnterCriticalSection(&mSc);
	nowtime(&time);

	if (in.Find(_T("[SUCCESS]")) >= 0)
	{
		successStart = in.Find(_T("[SUCCESS]"));
		successEnd = in.Find(_T("\n"));
		successEnd -= successStart;
		temp = in.Mid(successStart, successEnd);
		
		AfxExtractSubString(brightstr, temp, 1, '#');
		AfxExtractSubString(exposestr, temp, 2, '#');

		logstr.Format(_T("%s ��� %s ����ð� %s ���� �Կ� �Ϸ�."), time, brightstr, exposestr);
		m_List.InsertString(-1, logstr);
	}

	if (in.Find(_T("[SERIAL_NOT_OPEN]")) >= 0)
	{
		logstr.Format(_T("%s �ø��� Open���� ����. ������ ���� �� �������ּ���."), time);
		m_List.InsertString(-1, logstr);
		
	}

	if (in.Find(_T("[GRAB_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s ���� Grab ����. �ٽ� �õ����ֽðų� ī�޶� ������ Ȯ�����ּ���. Ȥ�� ����Ʈ ������ Ȯ�����ּ���."), time);
		m_List.InsertString(-1, logstr);
		
	}

	if (in.Find(_T("[CAMERA_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s ī�޶� �̻�. ���� ���� Ȥ�� Camera Open ���� ���˿��."), time);
		m_List.InsertString(-1, logstr);

	}
	if (in.Find(_T("[LIGHT_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s ���� ��Ʈ�ѷ� �̻�. ���� �ѱ� ����"), time);
		m_List.InsertString(-1, logstr);
	}
	
	LeaveCriticalSection(&mSc);

}

void CSendClientDlg::OutLog(LPCTSTR content, INT32 errcode)
{
	CString time = _T("");
	CString temp = _T("");
	nowtime(&time);
	switch (errcode)
	{
	case SOCKET_SEND:
		temp.Format(_T("%s %s ���� �Ϸ�"), time, content);
		break;
	case SOCKET_TIMEOUT:
		temp.Format(_T("%s %s Ÿ�� �ƿ�"), time, content);
		break;
	}
	m_List.InsertString(-1, temp);
}


BOOL CSendClientDlg::IsWrongInput(LPCTSTR input, UINT32 utype)
{
	CString in = input;
	if (in.GetLength() <= 0) 
	{
		m_strMessage = _T("");
		SetDlgItemTextW(IDC_BRIGHTEDIT, _T(""));
		SetDlgItemTextW(IDC_EXPOSUREEDIT, _T(""));
		return FALSE;
	}

	for (int i = 0; i < in.GetLength(); i++)
	{
		if (!iswdigit(in.GetAt(i)))
		{
			m_strMessage = _T("");
			SetDlgItemTextW(IDC_BRIGHTEDIT, _T(""));
			SetDlgItemTextW(IDC_EXPOSUREEDIT, _T(""));
			return FALSE;
		}
	}

	if (utype == BRIGHT)
	{
		if (_ttoi(in) > 255 || _ttoi(in) < 0)
		{
			m_strMessage = _T("");
			SetDlgItemTextW(IDC_BRIGHTEDIT, _T(""));
			SetDlgItemTextW(IDC_EXPOSUREEDIT, _T(""));
			return FALSE;
		}
	}
	else if (utype == EXPOSE)
	{
		if (_ttoi(in) > 100000 || _ttoi(in) < 0)
		{
			m_strMessage = _T("");
			SetDlgItemTextW(IDC_BRIGHTEDIT, _T(""));
			SetDlgItemTextW(IDC_EXPOSUREEDIT, _T(""));
			return FALSE;
		}
	}
	return TRUE;
}


void CSendClientDlg::OnBnClickedConnectbtn()
{
	m_IPEdit.GetWindowTextW(m_IpStr);
	m_PortEdit.GetWindowTextW(m_PortStr);
	GetDlgItem(IDC_CONNECTBTN)->EnableWindow(FALSE);
	UINT uport = _ttoi(m_PortStr);

	if (m_IsConnected)
	{
		m_Socket.ShutDown();
		m_Socket.Close();
		GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("����"));
		m_IsConnected = FALSE;
	}
	else
	{
		SetTimer(10, 10000, NULL);
		m_Socket.Create();
		if (m_Socket.Connect(m_IpStr, uport) == FALSE)
		{
			KillTimer(10);
			m_Socket.Close();
			AfxMessageBox(_T("���� ���� ����!"));
			m_IsConnected = FALSE;
			GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("����"));
			return;
		}
		else
		{
			m_IsConnected = TRUE;
			GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOOT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("���� ����"));
		}
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CSendClientDlg::nowtime(CString* write)
{
	CString nowtime = _T("");
	time_t timer;
	struct tm *t = new tm;

	timer = time(NULL);
	localtime_s(t, &timer);
	nowtime.Format(_T("[%d-%d-%d %d:%d:%d]"), t->tm_year + 1900, t->tm_mon + 1,
		t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	*write = nowtime;
	delete t;
}


void CSendClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	::DeleteCriticalSection(&mSc);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
