// ClientSocket.cpp : 구현 파일입니다.
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

// CClientSocket 멤버 함수


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	CSocket::OnReceive(nErrorCode);
}
