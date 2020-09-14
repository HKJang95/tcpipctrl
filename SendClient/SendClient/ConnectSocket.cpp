// ConnectSocket.cpp : ���� �����Դϴ�.
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


// ConnectSocket ��� �Լ�


void ConnectSocket::OnClose(int nErrorCode)
{
	CSendClientDlg* pMain = (CSendClientDlg *)AfxGetMainWnd();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox(_T("�������� ������ ���������ϴ�."));
	pMain->m_IsConnected = FALSE;
	pMain->GetDlgItem(IDC_CONNECTBTN)->EnableWindow(TRUE);
	pMain->GetDlgItem(IDC_SHOOT)->EnableWindow(FALSE);
	pMain->GetDlgItem(IDC_CONNECTBTN)->SetWindowTextW(_T("����"));
}

void ConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
