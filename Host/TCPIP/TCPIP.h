
// TCPIP.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTCPIPApp:
// �� Ŭ������ ������ ���ؼ��� TCPIP.cpp�� �����Ͻʽÿ�.
//

class CTCPIPApp : public CWinApp
{
public:
	CTCPIPApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTCPIPApp theApp;