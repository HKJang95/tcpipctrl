
// TCPIPDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TCPIP.h"
#include "TCPIPDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#include "crtdbg.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

#define LIGHT_OFF 0

#define ACTION_SUCCESS	0
#define SERIAL_NOT_OPEN -20000
#define CAMERA_NOT_OPEN -20001
#define GRAB_FAIL		-20002
#define LIGHT_FAIL		-20003

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


// CTCPIPDlg ��ȭ ����


CTCPIPDlg::CTCPIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPIPDlg::IDD, pParent)
	, m_ReceiveMsg(_T(""))
	, m_SerialSend(_T(""))
	, m_BaudRate(_T(""))
	, m_Port(_T(""))
	, m_Bright(_T(""))
	, m_ReceiveIP(_T(""))
	, m_ReceivePort(0)
	, m_IsSocketRcv(FALSE)
	, m_IsSerialOpen(FALSE)
	, m_pCamera(NULL)
	, m_PathName(_T(""))
	, m_IsServerOpen(FALSE)
	, m_IsCameraOpen(FALSE)
	, m_pBitmap(NULL)
	, m_IsLightOn(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

	::InitializeCriticalSection(&mSc);
}

void CTCPIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_PATHEDIT, m_PathEdit);
	DDX_Control(pDX, IDC_PORTCOMBO, m_PortCombo);
	DDX_Control(pDX, IDC_BAUDCOMBO, m_BaudCombo);
	DDX_Control(pDX, IDC_PORTEDIT, m_ServerPortEdit);
	DDX_Control(pDX, IDC_CAPIMG, m_CapImg);
	DDX_Control(pDX, IDC_EXPOSEEDIT, m_ExposeEdit);
}

BEGIN_MESSAGE_MAP(CTCPIPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_IMGPATH, &CTCPIPDlg::OnBnClickedImgpath)
	ON_BN_CLICKED(IDC_CAMBTN, &CTCPIPDlg::OnBnClickedCambtn)
	ON_BN_CLICKED(IDC_LIGHTBTN, &CTCPIPDlg::OnBnClickedLightbtn)
	ON_BN_CLICKED(IDC_SERVERBTN, &CTCPIPDlg::OnBnClickedServerbtn)
	ON_BN_CLICKED(IDC_EXPOSESET, &CTCPIPDlg::OnBnClickedExposeset)
END_MESSAGE_MAP()


// CTCPIPDlg �޽��� ó����

BOOL CTCPIPDlg::OnInitDialog()
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

	m_BaudCombo.InsertString(0, _T("4800"));
	m_BaudCombo.InsertString(1, _T("9600"));
	m_BaudCombo.InsertString(2, _T("14400"));
	m_BaudCombo.InsertString(3, _T("19200"));
	m_BaudCombo.InsertString(4, _T("38400"));
	m_BaudCombo.InsertString(5, _T("56000"));
	m_BaudCombo.InsertString(6, _T("57600"));
	m_BaudCombo.InsertString(7, _T("115200"));
	m_BaudCombo.SetCurSel(1);

	m_PortCombo.InsertString(0, _T("COM1"));
	m_PortCombo.InsertString(1, _T("COM2"));
	m_PortCombo.InsertString(2, _T("COM3"));
	m_PortCombo.InsertString(3, _T("COM4"));
	m_PortCombo.InsertString(4, _T("COM5"));
	m_PortCombo.InsertString(5, _T("COM6"));
	m_PortCombo.InsertString(6, _T("COM7"));
	m_PortCombo.InsertString(7, _T("COM8"));
	m_PortCombo.InsertString(8, _T("COM9"));
	m_PortCombo.InsertString(9, _T("COM10"));
	m_PortCombo.InsertString(10, _T("COM11"));
	m_PortCombo.SetCurSel(2);
	m_ServerPortEdit.SetWindowTextW(_T("21000"));
	m_pCamera = new CaneraControl();

	if (!m_pCamera->CameraInit())
	{
		AfxMessageBox(_T("���! ī�޶� ���̺귯�� �ε忡 �����߽��ϴ�. ī�޶� Open�� �Ұ��մϴ�."));
		delete m_pCamera;
		GetDlgItem(IDC_CAMBTN)->EnableWindow(FALSE);
	}

	m_PathEdit.SetWindowTextW(_T("C:\\photo"));

	static CClientDC dispDC(GetDlgItem(IDC_CAPIMG));
	m_hDC = dispDC.GetSafeHdc();
	GetDlgItem(IDC_CAPIMG)->GetWindowRect(m_rcDisp);
	m_pGraphics = Graphics::FromHDC(m_hDC);

	GetDlgItem(IDC_EXPOSESET)->EnableWindow(FALSE);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CTCPIPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPIPDlg::OnPaint()
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
		if (m_IsCameraOpen)
		{
			m_pGraphics->DrawImage(m_pBitmap, 0, 0, m_pCamera->m_camWidth, m_pCamera->m_camHeight);
		}
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTCPIPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCPIPDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	::LeaveCriticalSection(&mSc);
	::DeleteCriticalSection(&mSc);
	POSITION pos;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL)
	{
		pClient = (CClientSocket *)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();
			delete pClient;
		}
	}

	if (m_ListenSocket != NULL)
	{
		m_ListenSocket.ShutDown();
		m_ListenSocket.Close();
	}

	if (m_Comm != NULL)
	{
		m_Comm->Close();
		if (!m_Comm->m_bIsOpenned)
		{
			delete m_Comm;
		}
		else
		{
			m_Comm = NULL;
		}
	}
	if (m_pCamera != NULL)
	{
		m_pCamera->CameraClose();
		m_pCamera->CameraDestroy();
		delete m_pCamera;
	}
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
	}
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CTCPIPDlg::ActionControl()
{
	int bright = 0;
	BOOL complete = TRUE;
	CString msg = _T("");
	CString innerLogMsg = _T("");

	::EnterCriticalSection(&mSc);
	bright = _ttoi(m_ReceiveMsg);
	
	if (m_IsSocketRcv)
	{
		if (m_IsSerialOpen)
		{
			if (!LightControl(bright))
			{
				opLog(_T("����Ʈ �ѱ� ����!"));
				AddCompleteMsg(LIGHT_FAIL, &msg);
				m_IsLightOn = FALSE;
			}
			else
			{
				innerLogMsg.Format(_T("����Ʈ �ѱ�. ��� : %d"), bright);
				m_IsLightOn = TRUE;
				opLog(innerLogMsg);
			}
		}
		if (m_IsCameraOpen)
		{
			if (m_pCamera->GrabImage())
			{
				RawToBmp();
				InvalidateRect(m_rcDisp, NULL);
				BmpSave();
				opLog(_T("�̹��� ���� �Ϸ�."));
			}
			else
			{
				AddCompleteMsg(GRAB_FAIL, &msg);
				opLog(_T("�̹��� Grab ����."));
				complete = FALSE;
			}
		}
		else
		{
			Sleep(15);
			AddCompleteMsg(CAMERA_NOT_OPEN, &msg);
			opLog(_T("ī�޶� Open���� ����. ī�޶� ���� �Ұ�."));
			complete = FALSE;
		}

		if (m_IsSerialOpen)
		{ 
			LightControl(LIGHT_OFF);
			m_IsLightOn = FALSE;
			opLog(_T("����Ʈ OFF"));
		}
		else
		{
			AddCompleteMsg(SERIAL_NOT_OPEN, &msg);
			opLog(_T("�ø��� ��Ʈ Open���� ����. ����Ʈ ���� �Ұ�."));
		}
		if (complete)
		{
			AddCompleteMsg(ACTION_SUCCESS, &msg);
			opLog(_T("���� �Կ� �Ϸ�."));
		}
		sendCompleteMsg(msg);
		opLog(_T("���� ���� �Ϸ�."));
		m_IsSocketRcv = FALSE;
	}
	::LeaveCriticalSection(&mSc);
}

BOOL CTCPIPDlg::LightControl(int brightness)
{
	CString CErr = _T("");
	CString sendASCII = _T("");
	CString sendMsg = _T("");
	char m_stx = 0x02;
	char m_etx = 0x03;

	if (brightness > 255 || brightness < 0)
	{
		m_List.AddString(CErr);
		m_IsSocketRcv = FALSE;
		return FALSE;
	}
	else
	{
		if (brightness > 99)
		{

		}
		else if (brightness < 100 && brightness > 9)
		{
			m_Bright.Format(_T("0%d"), brightness);
		}
		else
		{
			m_Bright.Format(_T("00%d"), brightness);
		}
	}
	sendASCII = (CString)(_T("CH1S") + m_Bright);
	sendMsg = (CString)(m_stx + sendASCII + m_etx);
	m_Comm->Send(sendMsg);
	return TRUE;
}

void CTCPIPDlg::AddCompleteMsg(INT32 errcode, CString* errMsg)
{
	CString exposetimeStr = _T("");
	CString strCompleteMsg = _T("");
	CString temp = *errMsg;

	if (m_IsCameraOpen)
	{
		exposetimeStr.Format(_T("%f"), m_pCamera->GetExposeTime());
	}

	switch (errcode)
	{
	case ACTION_SUCCESS:
		strCompleteMsg.Format(_T("[SUCCESS]#") + m_ReceiveMsg + _T("#") + exposetimeStr + _T("\n"));
		break;

	case SERIAL_NOT_OPEN:
		strCompleteMsg.Format(_T("[SERIAL_NOT_OPEN]"));
		break;

	case GRAB_FAIL:
		strCompleteMsg.Format(_T("[GRAB_FAIL]"));
		break;

	case CAMERA_NOT_OPEN:
		strCompleteMsg.Format(_T("[CAMERA_FAIL]"));
		break;

	case LIGHT_FAIL:
		strCompleteMsg.Format(_T("[LIGHT_FAIL]"));
		break;
	}
	temp.Format(_T("%s%s"), temp, strCompleteMsg);
	*errMsg = temp;
}

void CTCPIPDlg::sendCompleteMsg(LPCTSTR errMsg)
{
	POSITION pos = NULL;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;
	CString tempIP = _T("");
	UINT utempPort = 0;
	while (pos != NULL)
	{
		pClient = (CClientSocket *)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		pClient->GetPeerName(tempIP, utempPort);

		if (pClient != NULL)
		{
			if (tempIP == m_ReceiveIP && utempPort == m_ReceivePort)
			{
				pClient->Send(errMsg, lstrlen(errMsg) * 2);
			}
		}
	}
}

BOOL CTCPIPDlg::RawToBmp()
{
	BitmapData bitmapdata;
	Rect rc;
	ColorPalette* pPalette;
	int size = 0;
	if (!m_IsCameraOpen || m_pBitmap == NULL)
	{
		return FALSE;
	}
	else
	{
		rc = Rect(0, 0, m_pCamera->m_camWidth, m_pCamera->m_camHeight);
		m_pBitmap->LockBits(&rc, 0, PixelFormat8bppIndexed, &bitmapdata);
		memcpy(bitmapdata.Scan0, m_pCamera->m_pImage, m_pCamera->m_bufferSize);
		m_pBitmap->UnlockBits(&bitmapdata);
		int size = m_pBitmap->GetPaletteSize();
		pPalette = new ColorPalette[size];
		m_pBitmap->GetPalette(pPalette, size);
		for (unsigned int i = 0; i<pPalette->Count; i++)
		{
			pPalette->Entries[i] = Color::MakeARGB(255, i, i, i);
		}
		m_pBitmap->SetPalette(pPalette);
		delete[] pPalette;
	}
	return TRUE;
}


BOOL CTCPIPDlg::BmpSave()
{
	time_t timer;
	struct tm *t = new tm;
	
	CString cfname;
	TCHAR* _pfname;
	CString temp;
	timer = time(NULL);
	localtime_s(t, &timer);

	if (m_PathName == _T(""))
	{
		m_PathName = _T("C:\\photo");
	}
	
	cfname.Format(_T("%s\\[%d-%d-%d_%dh%dm%ds][ip_%s_port_%d].bmp"), m_PathName,t->tm_year + 1900, t->tm_mon + 1,
		t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, m_ReceiveIP, m_ReceivePort);
	_pfname = (TCHAR *)(LPCTSTR)cfname;

	CLSID clsid;
	CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &clsid);
	m_pBitmap->Save(_pfname, &clsid, NULL);
	delete t;
	
	return TRUE;
}

void CTCPIPDlg::OnBnClickedImgpath()
{
	ITEMIDLIST *pidlBrowse;
	BOOL bSuccess = FALSE;
	BOOL BrowserOn = FALSE;
	BROWSEINFO BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = (LPWSTR)(LPCTSTR)m_PathName;
	BrInfo.lpszTitle = _T("�̹����� ����� ������ �������ּ���.");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	pidlBrowse = SHBrowseForFolder(&BrInfo);

	if (pidlBrowse != NULL)
	{
		bSuccess = ::SHGetPathFromIDList(pidlBrowse, (LPWSTR)(LPCTSTR)m_PathName);
		if (!bSuccess)
		{
			MessageBox(_T("�߸��� �������Դϴ�."), _T(""), MB_OK | MB_ICONASTERISK);
			return;
		}
	}
	
	m_PathEdit.SetWindowTextW(m_PathName);
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTCPIPDlg::OnBnClickedCambtn()
{
	CString exposeTimeCstr = _T("");
	
	BtnToggle(FALSE);
	GetDlgItem(IDC_CAMBTN)->SetWindowTextW(_T("WAIT"));

	if (m_IsCameraOpen)
	{
		if (m_pCamera != NULL)
		{
			m_pCamera->CameraClose();
			GetDlgItem(IDC_CAMBTN)->SetWindowTextW(_T("Camera Close"));
			BtnToggle(TRUE);

			GetDlgItem(IDC_EXPOSESET)->EnableWindow(FALSE);
			m_IsCameraOpen = FALSE;
		}
	}
	else
	{
		if (m_pCamera != NULL)
		{
			if (!m_pCamera->CameraOpen())
			{
				AfxMessageBox(_T("ī�޶� ���� ����!"));
				GetDlgItem(IDC_CAMBTN)->SetWindowTextW(_T("Camera Close"));
				BtnToggle(TRUE);
				m_IsCameraOpen = FALSE;
				return;
			}
			m_pCamera->setTriggerFeature();
			GetDlgItem(IDC_CAMBTN)->SetWindowTextW(_T("Camera Open"));
			BtnToggle(TRUE);

			exposeTimeCstr.Format(_T("%.2f"), m_pCamera->GetExposeTime());

			GetDlgItem(IDC_EXPOSEEDIT)->SetWindowTextW(exposeTimeCstr);
			GetDlgItem(IDC_EXPOSEEDIT)->EnableWindow(TRUE);
			m_IsCameraOpen = TRUE;
			m_pBitmap = new Bitmap(m_pCamera->m_camWidth, m_pCamera->m_camHeight, PixelFormat8bppIndexed);
			m_pGraphics->DrawImage(m_pBitmap, 0, 0, m_pCamera->m_camWidth, m_pCamera->m_camHeight);

		}
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void CTCPIPDlg::OnBnClickedLightbtn()
{
	int cursel = -1;
	BtnToggle(FALSE);
	GetDlgItem(IDC_LIGHTBTN)->SetWindowTextW(_T("WAIT"));

	if (m_IsSerialOpen)
	{
		if (m_Comm != NULL)
		{
			m_Comm->Close();
			if (!m_Comm->m_bIsOpenned)
			{
				delete m_Comm;
			}
			else
			{
				m_Comm = NULL;
			}
		}
		m_IsSerialOpen = FALSE;

		BtnToggle(TRUE);
		GetDlgItem(IDC_LIGHTBTN)->SetWindowTextW(_T("Light OFF"));
	}
	else
	{
		cursel = m_BaudCombo.GetCurSel();
		m_BaudCombo.GetLBText(cursel, m_BaudRate);
		cursel = -1;
		cursel = m_PortCombo.GetCurSel();
		m_PortCombo.GetLBText(cursel, m_Port);
		cursel = -1;

		m_Comm = new SerialPort(_T("\\\\.\\") + m_Port, m_BaudRate, _T("None"), _T("8 Bit"), _T("1 Bit"));

		if (m_Comm->Create(GetSafeHwnd()) != 0)
		{
			m_Comm->Clear();
		}
		else
		{
			AfxMessageBox(_T("Light Controller Connection FAIL!"));
			m_IsSerialOpen = FALSE;

			GetDlgItem(IDC_LIGHTBTN)->SetWindowTextW(_T("Light OFF"));
			BtnToggle(TRUE);
			return;
		}
		m_IsSerialOpen = TRUE;
		GetDlgItem(IDC_LIGHTBTN)->SetWindowTextW(_T("Light ON"));
		BtnToggle(TRUE);
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTCPIPDlg::OnBnClickedServerbtn()
{
	POSITION pos;
	UINT uport = 0;
	CClientSocket* pClient = NULL;

	BtnToggle(FALSE);
	GetDlgItem(IDC_SERVERBTN)->SetWindowTextW(_T("WAIT"));
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	GetDlgItem(IDC_PORTEDIT)->GetWindowTextW(m_Port);
	uport = _ttoi(m_Port);
	
	if (m_IsServerOpen)
	{
		while (pos != NULL)
		{
			pClient = (CClientSocket *)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
			if (pClient != NULL)
			{
				pClient->ShutDown();
				pClient->Close();
				delete pClient;
			}
		}
		if (m_ListenSocket != NULL)
		{
			m_ListenSocket.ShutDown();
			m_ListenSocket.Close();
			m_ListenSocket.m_ptrClientSocketList.RemoveAll();
		}
		GetDlgItem(IDC_SERVERBTN)->SetWindowTextW(_T("Server Off"));
		m_IsServerOpen = FALSE;
		if (m_IsCameraOpen) { BtnToggle(TRUE); } else { BtnToggle(TRUE, TRUE, TRUE, TRUE, FALSE); }
	}
	else
	{
		if (m_ListenSocket.Create(uport, SOCK_STREAM))
		{
			if (!m_ListenSocket.Listen())
			{
				AfxMessageBox(_T("Error : Listen() return FALSE!"));
				GetDlgItem(IDC_SERVERBTN)->SetWindowTextW(_T("Server Off"));
				m_IsServerOpen = FALSE;
				if (m_IsCameraOpen) { BtnToggle(TRUE); } else { BtnToggle(TRUE, TRUE, TRUE, TRUE, FALSE); }
				return;
			}
		}
		else
		{
			AfxMessageBox(_T("ERROR : Failed to create server socket!"));
			GetDlgItem(IDC_SERVERBTN)->SetWindowTextW(_T("Server Off"));
			m_IsServerOpen = FALSE;
			BtnToggle(TRUE);
			if (m_IsCameraOpen) { BtnToggle(TRUE); } else { BtnToggle(TRUE, TRUE, TRUE, TRUE, FALSE); }
			return;
		}
		GetDlgItem(IDC_SERVERBTN)->SetWindowTextW(_T("Server On"));
		m_IsServerOpen = TRUE;
		BtnToggle(FALSE, FALSE, TRUE, FALSE, FALSE);
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTCPIPDlg::BtnToggle(BOOL AllToggle)
{
	GetDlgItem(IDC_LIGHTBTN)->EnableWindow(AllToggle);
	GetDlgItem(IDC_SERVERBTN)->EnableWindow(AllToggle);
	GetDlgItem(IDC_CAMBTN)->EnableWindow(AllToggle);
	GetDlgItem(IDC_IMGPATH)->EnableWindow(AllToggle);
	GetDlgItem(IDC_EXPOSESET)->EnableWindow(AllToggle);
}

void CTCPIPDlg::BtnToggle(BOOL LightToggle, BOOL CameraToggle, BOOL ServerToggle, BOOL ImgPathToggle, BOOL ExposeToggle)
{
	GetDlgItem(IDC_LIGHTBTN)->EnableWindow(LightToggle);
	GetDlgItem(IDC_CAMBTN)->EnableWindow(CameraToggle);
	GetDlgItem(IDC_SERVERBTN)->EnableWindow(ServerToggle);
	GetDlgItem(IDC_IMGPATH)->EnableWindow(ImgPathToggle);
	GetDlgItem(IDC_EXPOSESET)->EnableWindow(ExposeToggle);
}

void CTCPIPDlg::OnBnClickedExposeset()
{
	CString exposeTimeStr = _T("");
	if (m_IsCameraOpen)
	{
		m_ExposeEdit.GetWindowTextW(exposeTimeStr);
		if (m_pCamera->SetExposeTime(_ttof(exposeTimeStr)))
		{
			exposeTimeStr = _T("");
			exposeTimeStr.Format(_T("%.2f"), m_pCamera->GetExposeTime());
			m_ExposeEdit.SetWindowTextW(exposeTimeStr);
		}
		else
		{
			AfxMessageBox(_T("Expose Time Set ����!"));
		}
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void CTCPIPDlg::opLog(CString content)
{
	CString time;
	CString result = _T("");
	
	nowtime(&time);

	result.Format(_T("%s %s"), time, content);
	m_List.InsertString(-1, result);
}

void CTCPIPDlg::opLog(CString ip, UINT uport, CString content)
{

	CString temp = _T("");
	CString timestr = _T("");
	nowtime(&timestr);

	temp.Format(_T("���ŵ�. %s IP : %s PORT : %d ��� : %s"),timestr, ip, uport, content);
	m_List.InsertString(-1, temp);
}

void CTCPIPDlg::nowtime(CString* timestr)
{
	CString strTemp = _T("");
	time_t timer;
	struct tm *t = new tm;

	timer = time(NULL);
	localtime_s(t, &timer);

	strTemp.Format(_T("[%d-%d-%d %d:%d:%d]"), t->tm_year + 1900, t->tm_mon + 1,
		t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	*timestr = strTemp;
	delete t;
}
