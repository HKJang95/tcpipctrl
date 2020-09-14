#pragma once


// CaneraControl

class CaneraControl : public CWnd
{
	DECLARE_DYNAMIC(CaneraControl)

public:
	CaneraControl();
	virtual ~CaneraControl();

	BOOL CameraInit();
	BOOL CameraOpen();
	BOOL setTriggerFeature();
	BOOL GrabImage();
	DOUBLE GetExposeTime();
	BOOL SetExposeTime(DOUBLE exposetime);
	void CameraClose();
	void CameraDestroy();

public:
	BYTE* m_pImage;
	INT32 m_camWidth;
	INT32 m_camHeight;
	int m_hDevice;
	BOOL m_IsOpen;
	INT32 m_bufferSize;

private:
	INT32 m_status;
	UINT32 m_camNum;
	CString m_strErr;
	BOOL m_IsAcq;
	
	CRITICAL_SECTION mSc;

protected:
	DECLARE_MESSAGE_MAP()
};