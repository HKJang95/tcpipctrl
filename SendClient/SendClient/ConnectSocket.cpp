// ConnectSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SendClient.h"
#include "ConnectSocket.h"
#include "SendClientDlg.h"
#include "ctime"
// ConnectSocket

ConnectSocket::ConnectSocket()
{
}

ConnectSocket::~ConnectSocket()
{
}


// ConnectSocket 멤버 함수


void ConnectSocket::OnClose(int nErrorCode)
{
	CSendClientDlg* pMain = (CSendClientDlg *)AfxGetMainWnd();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox(_T("서버와의 연결이 끊어졌습니다."));
	pMain->m_IsConnected = FALSE;
	pMain->GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
	pMain->GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
	pMain->GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("연결"));
}

void ConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	if (Receive(szBuffer, sizeof(szBuffer)) > 0)
	{
		CSendClientDlg* pMain = (CSendClientDlg *)AfxGetMainWnd();
		pMain->InLog(szBuffer);
		pMain->GetDlgItem(IDC_SHOOT)->EnableWindow(TRUE);
		pMain->m_rcvMessage = szBuffer;
		pMain->m_strMessage = _T("");
	}

	CSocket::OnReceive(nErrorCode);
}

BOOL ConnectSocket::OnMessagePending()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	MSG Message;
	if (::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (Message.wParam == 10)
		{
			::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			Close();
		}
	}
	return CSocket::OnMessagePending();
}
