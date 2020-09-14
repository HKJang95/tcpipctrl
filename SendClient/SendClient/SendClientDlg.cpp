
// SendClientDlg.cpp : 구현 파일
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



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSendClientDlg 대화 상자



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


// CSendClientDlg 메시지 처리기

BOOL CSendClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_PortEdit.SetWindowTextW(_T("21000"));
	m_IPEdit.SetWindowTextW(_T("192.168.20.34"));

	SetDlgItemTextW(IDC_BRIGHTEDIT, _T("5"));
	SetDlgItemTextW(IDC_EXPOSUREEDIT, _T("60000"));

	GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
	::InitializeCriticalSection(&mSc);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSendClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
		strErr.Format(_T("%s는 잘못된 값입니다. 0~255 사이의 숫자를 입력해주세요."), m_strMessage);
		AfxMessageBox(strErr);
		return;
	}
	else if (!IsWrongInput(m_strExpose, EXPOSE))
	{
		strErr.Format(_T("%s는 잘못된 값입니다. 0~255 사이의 숫자를 입력해주세요."), m_strMessage);
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

		logstr.Format(_T("%s 밝기 %s 노출시간 %s 영상 촬영 완료."), time, brightstr, exposestr);
		m_List.InsertString(-1, logstr);
	}

	if (in.Find(_T("[SERIAL_NOT_OPEN]")) >= 0)
	{
		logstr.Format(_T("%s 시리얼 Open되지 않음. 조명을 연결 및 설정해주세요."), time);
		m_List.InsertString(-1, logstr);
		
	}

	if (in.Find(_T("[GRAB_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s 영상 Grab 실패. 다시 시도해주시거나 카메라 설정을 확인해주세요. 혹은 라이트 설정을 확인해주세요."), time);
		m_List.InsertString(-1, logstr);
		
	}

	if (in.Find(_T("[CAMERA_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s 카메라 이상. 연결 상태 혹은 Camera Open 상태 점검요망."), time);
		m_List.InsertString(-1, logstr);

	}
	if (in.Find(_T("[LIGHT_FAIL]")) >= 0)
	{
		logstr.Format(_T("%s 조명 컨트롤러 이상. 조명 켜기 실패"), time);
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
		temp.Format(_T("%s %s 전송 완료"), time, content);
		break;
	case SOCKET_TIMEOUT:
		temp.Format(_T("%s %s 타임 아웃"), time, content);
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
		GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("연결"));
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
			AfxMessageBox(_T("서버 연결 실패!"));
			m_IsConnected = FALSE;
			GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("연결"));
			return;
		}
		else
		{
			m_IsConnected = TRUE;
			GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SHOOT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("연결 해제"));
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
