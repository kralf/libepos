// Demo_WinDLLDlg.h : Header-Datei
//

#if !defined(AFX_DEMO_WINDLLDLG_H__04D04B9B_472D_48F0_82B0_F0674CA8BFE9__INCLUDED_)
#define AFX_DEMO_WINDLLDLG_H__04D04B9B_472D_48F0_82B0_F0674CA8BFE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLDlg Dialogfeld

class CDeviceSettings;

class CDemo_WinDLLDlg : public CDialog
{
// Konstruktion
public:
	CDemo_WinDLLDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDemo_WinDLLDlg)
	enum { IDD = IDD_DEMO_WINDLL_DIALOG };
	CButton	m_DeviceSettings;
	CButton	m_Halt;
	CButton	m_Move;
	CButton	m_Enable;
	CButton	m_Disable;
	long	m_lStartPosition;
	long	m_lTargetPosition;
	int		m_oRadio;
	CString	m_strActiveMode;
	long	m_lActualValue;
	CString	m_strNodeId;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CDemo_WinDLLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CDemo_WinDLLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonDisable();
	afx_msg void OnButtonEnable();
	afx_msg void OnButtonMove();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButtonHalt();
	afx_msg void OnChangeEditTargetPosition();
	afx_msg void OnRadioRelative();
	afx_msg void OnRadioAbsolute();
	afx_msg void OnButtonDeviceSettings();
	afx_msg void OnChangeEditNodeId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void StopTimer();
	BOOL ShowErrorInformation(DWORD dErrorCode);
	BOOL OpenDevice();
	BOOL RestoreEPOS();
	BOOL UpdateStatus();
	void UpdateNodeIdString();

private:
	BOOL m_oImmediately;
	BOOL m_oInitialisation;
	BOOL m_oUpdateActive;
	__int8 m_uMode;
	DWORD m_dErrorCode;
	DWORD m_dProfileAcceleration;
	DWORD m_dProfileDeceleration;
	DWORD m_dProfileVelocity;
	HANDLE m_KeyHandle;
	WORD m_wNodeId;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_DEMO_WINDLLDLG_H__04D04B9B_472D_48F0_82B0_F0674CA8BFE9__INCLUDED_)
