// Demo_WinDLLDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "Definitions.h"
#include "Demo_WinDLL.h"

#include "Demo_WinDLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLDlg Dialogfeld

CDemo_WinDLLDlg::CDemo_WinDLLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo_WinDLLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemo_WinDLLDlg)
	m_lStartPosition = 0;
	m_lTargetPosition = 2000;
	m_oRadio = 0;
	m_strActiveMode = _T("");
	m_lActualValue = 0;
	m_strNodeId = _T("1");
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_WinDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemo_WinDLLDlg)
	DDX_Control(pDX, IDC_ButtonDeviceSettings, m_DeviceSettings);
	DDX_Control(pDX, IDC_ButtonHalt, m_Halt);
	DDX_Control(pDX, IDC_ButtonMove, m_Move);
	DDX_Control(pDX, IDC_ButtonEnable, m_Enable);
	DDX_Control(pDX, IDC_ButtonDisable, m_Disable);
	DDX_Text(pDX, IDC_EditStartPosition, m_lStartPosition);
	DDX_Text(pDX, IDC_EditTargetPosition, m_lTargetPosition);
	DDX_Radio(pDX, IDC_RadioRelative, m_oRadio);
	DDX_Text(pDX, IDC_StaticActiveMode, m_strActiveMode);
	DDX_Text(pDX, IDC_EditActualValue, m_lActualValue);
	DDX_Text(pDX, IDC_EditNodeId, m_strNodeId);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemo_WinDLLDlg, CDialog)
	//{{AFX_MSG_MAP(CDemo_WinDLLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ButtonDisable, OnButtonDisable)
	ON_BN_CLICKED(IDC_ButtonEnable, OnButtonEnable)
	ON_BN_CLICKED(IDC_ButtonMove, OnButtonMove)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ButtonHalt, OnButtonHalt)
	ON_EN_CHANGE(IDC_EditTargetPosition, OnChangeEditTargetPosition)
	ON_BN_CLICKED(IDC_RadioRelative, OnRadioRelative)
	ON_BN_CLICKED(IDC_RadioAbsolute, OnRadioAbsolute)
	ON_BN_CLICKED(IDC_ButtonDeviceSettings, OnButtonDeviceSettings)
	ON_EN_CHANGE(IDC_EditNodeId, OnChangeEditNodeId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLDlg Nachrichten-Handler

BOOL CDemo_WinDLLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if(!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	//Settings
	m_oImmediately = TRUE;
	m_oUpdateActive = FALSE;
	m_wNodeId = 1;

	//OpenDevice
	if(OpenDevice())
	{
		m_oInitialisation = TRUE;
		if(SetTimer(1,100,NULL)) m_oUpdateActive = TRUE;
	}
	else
	{
		m_oInitialisation = FALSE;

		m_Enable.EnableWindow(false);
		m_Disable.EnableWindow(false);
		m_Move.EnableWindow(false);
		m_Halt.EnableWindow(false);
	}

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CDemo_WinDLLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CDemo_WinDLLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR CDemo_WinDLLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/************************************************************************
Sets device to disable state
************************************************************************/
void CDemo_WinDLLDlg::OnButtonDisable() 
{
	UpdateNodeIdString();

	if(!VCS_SetDisableState(m_KeyHandle,m_wNodeId,&m_dErrorCode))
	{
		ShowErrorInformation(m_dErrorCode);
	}
}

/************************************************************************
Sets device to enable state
************************************************************************/
void CDemo_WinDLLDlg::OnButtonEnable() 
{
	UpdateNodeIdString();

	if(!VCS_SetEnableState(m_KeyHandle,m_wNodeId,&m_dErrorCode))
	{
		ShowErrorInformation(m_dErrorCode);
	}
}

/************************************************************************
Starts the movement
************************************************************************/
void CDemo_WinDLLDlg::OnButtonMove() 
{
	UpdateNodeIdString();

	if(VCS_GetPositionIs(m_KeyHandle,m_wNodeId,&m_lStartPosition,&m_dErrorCode))
	{
		if(!VCS_MoveToPosition(m_KeyHandle,m_wNodeId,m_lTargetPosition,m_oRadio,m_oImmediately,&m_dErrorCode))
		{
			ShowErrorInformation(m_dErrorCode);
		}
	}

	UpdateStatus();
}

/************************************************************************
Stops the movement
************************************************************************/
void CDemo_WinDLLDlg::OnButtonHalt() 
{
	UpdateNodeIdString();

	if(!VCS_HaltPositionMovement(m_KeyHandle,m_wNodeId,&m_dErrorCode))
	{
		ShowErrorInformation(m_dErrorCode);
	}
}

/************************************************************************
If timer was started all 100ms this function will be called
************************************************************************/
void CDemo_WinDLLDlg::OnTimer(UINT nIDEvent) 
{
	if(m_oUpdateActive && !UpdateStatus())
	{
		StopTimer();
	}

	CDialog::OnTimer(nIDEvent);
}

/************************************************************************
Updates the display
************************************************************************/
BOOL CDemo_WinDLLDlg::UpdateStatus()
{
	BOOL oEnable = TRUE;
	BOOL oResult = m_oUpdateActive;

	if(m_oRadio == 0)
	{
		m_Move.SetWindowText("&Move Relative");
	}
	else
	{
		m_Move.SetWindowText("&Move Absolute");
	}

	if(oResult)
	{
		oResult = VCS_GetOperationMode(m_KeyHandle,m_wNodeId,&m_uMode,&m_dErrorCode);
		if(oResult)
		{
			switch(m_uMode)
			{
				case -6: m_strActiveMode = "Step/Direction Mode"; break;
				case -5: m_strActiveMode = "Master Encoder Mode"; break;
				case -3: m_strActiveMode = "Current Mode"; break;
				case -2: m_strActiveMode = "Velocity Mode"; break;
				case -1: m_strActiveMode = "Position Mode"; break;
				case 1: m_strActiveMode = "Profile Position Mode"; break;
				case 3: m_strActiveMode = "Profile Velocity Mode"; break;
				case 6: m_strActiveMode = "Homing Mode"; break;
				default: m_strActiveMode = "Unknown Mode";
			}
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_dErrorCode);

			m_strActiveMode = "Unknown Mode";
		}
	}
	else
	{
		m_strActiveMode = "Unknown Mode";
	}

	if(oResult)
	{
		oResult = VCS_GetEnableState(m_KeyHandle,m_wNodeId,&oEnable,&m_dErrorCode);

		if(oResult)
		{
			m_DeviceSettings.EnableWindow(!oEnable);
			m_Enable.EnableWindow(!oEnable);
			m_Disable.EnableWindow(oEnable);
			m_Move.EnableWindow(oEnable);
			m_Halt.EnableWindow(oEnable);
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_dErrorCode);

			m_DeviceSettings.EnableWindow(oEnable);
			m_Enable.EnableWindow(oEnable);
			m_Disable.EnableWindow(!oEnable);
			m_Move.EnableWindow(!oEnable);
			m_Halt.EnableWindow(!oEnable);
		}
	}
	else
	{
		m_DeviceSettings.EnableWindow(oEnable);
		m_Enable.EnableWindow(!oEnable);
		m_Disable.EnableWindow(!oEnable);
		m_Move.EnableWindow(!oEnable);
		m_Halt.EnableWindow(!oEnable);
	}

	if(oResult)
	{
		oResult = VCS_GetPositionIs(m_KeyHandle,m_wNodeId,&m_lActualValue,&m_dErrorCode);
		if(!oResult)
		{
			StopTimer();
			ShowErrorInformation(m_dErrorCode);

			m_lActualValue = 0;
			m_lStartPosition = 0;
		}
	}
	else
	{
		m_lActualValue = 0;
		m_lStartPosition = 0;
	}

	if(m_hWnd) UpdateData(false);

	return oResult;
}

/************************************************************************
Restores the old settings, stops the timer and delete the key handle
************************************************************************/
BOOL CDemo_WinDLLDlg::RestoreEPOS()
{
	//Restore Settings
	if(m_oInitialisation)
	{
		VCS_SetDisableState(m_KeyHandle,m_wNodeId,&m_dErrorCode);
		VCS_SetOperationMode(m_KeyHandle,m_wNodeId,m_uMode,&m_dErrorCode);
		VCS_SetPositionProfile(m_KeyHandle,m_wNodeId,m_dProfileVelocity,m_dProfileAcceleration,m_dProfileDeceleration,&m_dErrorCode);
	}
	
	//Stop Updating
	StopTimer();

	//Close Device
	if(m_KeyHandle) VCS_CloseDevice(m_KeyHandle,&m_dErrorCode);
	
	return TRUE;
}

/************************************************************************
Closes the dialog
************************************************************************/
void CDemo_WinDLLDlg::OnDestroy() 
{
	RestoreEPOS();
	CDialog::OnDestroy();
}

/************************************************************************
Updates the position value
************************************************************************/
void CDemo_WinDLLDlg::OnChangeEditTargetPosition() 
{
	if(m_hWnd) UpdateData(true);
}

/************************************************************************
Sets state to relative movement
************************************************************************/
void CDemo_WinDLLDlg::OnRadioRelative() 
{
	m_oRadio = 0;
}

/************************************************************************
Sets state to absolute movement
************************************************************************/
void CDemo_WinDLLDlg::OnRadioAbsolute() 
{
	m_oRadio = 1;
}

/************************************************************************
Sets new communication settings via button 'Device Settings'
************************************************************************/
void CDemo_WinDLLDlg::OnButtonDeviceSettings() 
{
	UpdateNodeIdString();

	if(OpenDevice())
	{
		m_oInitialisation = TRUE;
		if(SetTimer(1,100,NULL)) m_oUpdateActive = TRUE;
	}
	else
	{
		m_oInitialisation = FALSE;
		StopTimer();
	}
}

/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL CDemo_WinDLLDlg::OpenDevice()
{
	HANDLE hNewKeyHandle;

	//OpenDevice
	hNewKeyHandle = VCS_OpenDeviceDlg(&m_dErrorCode);
	if(hNewKeyHandle)
	{
		//Close Previous Device
		if(m_KeyHandle) VCS_CloseDevice(m_KeyHandle,&m_dErrorCode);
		m_KeyHandle = hNewKeyHandle;
		
		//Clear Error History
		if(VCS_ClearFault(m_KeyHandle,m_wNodeId,&m_dErrorCode))
		{
			//Read Operation Mode
			if(VCS_GetOperationMode(m_KeyHandle,m_wNodeId,&m_uMode,&m_dErrorCode))
			{
				//Read Position Profile Objects
				if(VCS_GetPositionProfile(m_KeyHandle,m_wNodeId,&m_dProfileVelocity,&m_dProfileAcceleration,&m_dProfileDeceleration,&m_dErrorCode))
				{
					//Write Profile Position Mode
					if(VCS_SetOperationMode(m_KeyHandle,m_wNodeId,0x01/*Profile Position Mode*/,&m_dErrorCode))
					{
						//Write Profile Position Objects
						if(VCS_SetPositionProfile(m_KeyHandle,m_wNodeId,100,1000,1000,&m_dErrorCode))
						{
							//Read Actual Position
							if(VCS_GetPositionIs(m_KeyHandle,m_wNodeId,&m_lStartPosition,&m_dErrorCode))
							{
								return TRUE;
							}
						}
					}
				}
			}
		}

		ShowErrorInformation(m_dErrorCode);
	}
	else
	{
		AfxMessageBox("Can't open device!",MB_ICONINFORMATION);
	}

	return FALSE;
}

/************************************************************************
Changes node id
************************************************************************/
void CDemo_WinDLLDlg::OnChangeEditNodeId() 
{
	if(m_hWnd) UpdateData(true);

	if(m_strNodeId != "")
	{
		m_wNodeId = atoi(m_strNodeId);
		if(m_wNodeId >= 1 && m_wNodeId <= 128)
		{
			if(SetTimer(1,100,NULL)) m_oUpdateActive = TRUE;
		}
		else
		{
			AfxMessageBox("Node ID from 1 to 127!",MB_ICONINFORMATION);
		}
	}
}

/************************************************************************
Converts node id to string
************************************************************************/
void CDemo_WinDLLDlg::UpdateNodeIdString()
{
	char strNodeId[3];

	itoa(m_wNodeId,strNodeId,10);
	m_strNodeId = strNodeId;
}

/************************************************************************
Stops timer. Status will be displayed as disconnected
************************************************************************/
void CDemo_WinDLLDlg::StopTimer()
{
	KillTimer(1);

	m_oUpdateActive = FALSE;

	UpdateStatus();
}

/************************************************************************
Shows a message box with error description of the input error code
************************************************************************/
BOOL CDemo_WinDLLDlg::ShowErrorInformation(DWORD dErrorCode)
{
	char* pStrErrorInfo;
	CString strDescription;

	if((pStrErrorInfo = (char*)malloc(100)) == NULL)
	{
		MessageBox("Not enough memory to allocate buffer for error information string\n","System Error");

		return FALSE;
	}

	if(VCS_GetErrorInfo(dErrorCode,pStrErrorInfo,100))
	{
		strDescription = pStrErrorInfo;
		AfxMessageBox(strDescription,MB_ICONINFORMATION);

		free(pStrErrorInfo);

		return TRUE;
	}
	else
	{
		free(pStrErrorInfo);
		AfxMessageBox("Error information can't be read!",MB_ICONINFORMATION);

		return FALSE;
	}
}
