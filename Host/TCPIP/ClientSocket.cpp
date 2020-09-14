// ClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TCPIP.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "TCPIPDlg.h"
#include "ctime"


// CClientSocket

CClientSocket::CClientSocket()
{
	m_pListenSocket = NULL;

}

CClientSocket::~CClientSocket()
{

}


void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

// CClientSocket ��� �Լ�


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnClose(nErrorCode);

	CListenSocket * pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CTCPIPDlg* pMain = (CTCPIPDlg*)AfxGetMainWnd();
	
	CString  strIpAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	GetPeerName(strIpAddress, uPortNumber);
	
	if (Receive(szBuffer, sizeof(szBuffer)) > 0 && pMain->m_IsSocketRcv != TRUE)
	{
		pMain->m_ReceiveIP = strIpAddress;
		pMain->m_ReceivePort = uPortNumber;
		pMain->m_ReceiveMsg = szBuffer;
		pMain->opLog(strIpAddress, uPortNumber, szBuffer);
		pMain->m_IsSocketRcv = TRUE;
		pMain->ActionControl();
	}
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	CSocket::OnReceive(nErrorCode);
}
