#pragma once

// ConnectSocket 명령 대상입니다.

class ConnectSocket : public CSocket
{
public:
	ConnectSocket();
	virtual ~ConnectSocket();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

public:
	virtual BOOL OnMessagePending();
};


