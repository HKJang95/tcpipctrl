#pragma once

// CClientSocket ���� ����Դϴ�.

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

	CAsyncSocket* m_pListenSocket;

public:
	void SetListenSocket(CAsyncSocket* pSocket);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

