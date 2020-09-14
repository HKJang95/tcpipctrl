// CaneraControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TCPIP.h"
#include "CaneraControl.h"
#include "VirtualFG40.h"
#include "TCPIPDlg.h"

// CaneraControl

IMPLEMENT_DYNAMIC(CaneraControl, CWnd)

CaneraControl::CaneraControl()
{
	m_status = MCAM_ERR_SUCCESS;
	m_camNum = 0;
	m_camWidth = 0;
	m_camHeight = 0;
	m_bufferSize = 0;
	m_pImage = NULL;
	m_hDevice = -1;
	m_strErr = _T("");
	m_IsOpen = FALSE;
	m_IsAcq = FALSE;

	::InitializeCriticalSection(&mSc);
}

CaneraControl::~CaneraControl()
{
}

BOOL CaneraControl::CameraInit()
{
	m_status = ST_InitSystem();
	if (m_status != MCAM_ERR_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CaneraControl::CameraOpen()
{
	
	m_status = ST_UpdateDevice();
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Device Update Fail! : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	m_status = ST_GetAvailableCameraNum(&m_camNum);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("No Camera Available! Check connection."));
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	m_status = ST_OpenDevice(0, &m_hDevice);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Device Open Fail! : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	m_status = ST_GetIntReg(m_hDevice, MCAM_WIDTH, &m_camWidth);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Read Register failed : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	m_status = ST_GetIntReg(m_hDevice, MCAM_HEIGHT, &m_camHeight);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Read Register failed : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	m_bufferSize = m_camWidth * m_camHeight;
	m_pImage = (BYTE *)malloc(m_bufferSize);
	::ZeroMemory(m_pImage, m_bufferSize);

	m_status = ST_AcqStart(m_hDevice);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Acquisition start failed : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		m_IsAcq = FALSE;
		return FALSE;
	}
	else { m_IsAcq = TRUE; }
	SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
	m_IsOpen = TRUE;
	return TRUE;
}

BOOL CaneraControl::setTriggerFeature()
{
	m_status = ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_MODE, TRIGGER_MODE_ON);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Write Register failed : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	m_status = ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, TRIGGER_SOURCE_SOFTWARE);
	if (m_status != MCAM_ERR_SUCCESS)
	{
		m_strErr.Format(_T("Write Register failed : %d"), m_status);
		AfxMessageBox(m_strErr, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	return TRUE;
	// software trigger set
}

DOUBLE CaneraControl::GetExposeTime()
{
	DOUBLE exposetime = 0;
	m_status = ST_GetFloatReg(m_hDevice, MCAM_EXPOSURE_TIME, &exposetime);
	if (m_status == MCAM_ERR_SUCCESS)
	{
		return exposetime;
	}
	else
	{
		return -1.0;
	}
}

BOOL CaneraControl::SetExposeTime(DOUBLE exposetime)
{
	m_status = ST_SetFloatReg(m_hDevice, MCAM_EXPOSURE_TIME, exposetime);
	if (m_status == MCAM_ERR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CaneraControl::GrabImage()
{
	CTCPIPDlg* pMain = (CTCPIPDlg*)AfxGetMainWnd();
	if (m_IsOpen && pMain->m_IsLightOn)
	{
		::EnterCriticalSection(&mSc);
		m_status = ST_SetCmdReg(m_hDevice, MCAM_TRIGGER_SOFTWARE);
		if (m_status != MCAM_ERR_SUCCESS)
		{
			::LeaveCriticalSection(&mSc);
			return FALSE;
		}
		
		m_status = ST_GrabImageAsync(m_hDevice, m_pImage, m_bufferSize, -1);
		if (m_status != MCAM_ERR_SUCCESS)
		{
			::LeaveCriticalSection(&mSc);
			return FALSE;
		}
		::LeaveCriticalSection(&mSc);
		return TRUE;
	}
	else
	{
		::LeaveCriticalSection(&mSc);
		return FALSE;
	}
	
	
}

void CaneraControl::CameraClose()
{
	::LeaveCriticalSection(&mSc);
	if (m_IsAcq)
	{
		m_status = ST_AcqStop(m_hDevice);
		if (m_status != MCAM_ERR_SUCCESS)
		{
			AfxMessageBox(_T("Acquisition Stop 실패!"));
		}
		m_IsAcq = FALSE;
	}
	if (m_pImage != NULL) 
	{
		free(m_pImage);
		m_pImage = NULL;
	}
	
	ST_CloseDevice(m_hDevice);
	
	SetThreadExecutionState(ES_CONTINUOUS);
}

void CaneraControl::CameraDestroy()
{
	::LeaveCriticalSection(&mSc);
	::DeleteCriticalSection(&mSc);
	ST_FreeSystem();
	SetThreadExecutionState(ES_CONTINUOUS);
}


BEGIN_MESSAGE_MAP(CaneraControl, CWnd)
END_MESSAGE_MAP()



// CaneraControl 메시지 처리기입니다.


