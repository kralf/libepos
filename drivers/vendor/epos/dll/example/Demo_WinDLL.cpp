// Demo_WinDLL.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "Demo_WinDLL.h"
#include "Demo_WinDLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLApp

BEGIN_MESSAGE_MAP(CDemo_WinDLLApp, CWinApp)
	//{{AFX_MSG_MAP(CDemo_WinDLLApp)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLApp Konstruktion

CDemo_WinDLLApp::CDemo_WinDLLApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CDemo_WinDLLApp-Objekt

CDemo_WinDLLApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDemo_WinDLLApp Initialisierung

BOOL CDemo_WinDLLApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	CDemo_WinDLLDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// ZU ERLEDIGEN: Fügen Sie hier Code ein, um ein Schließen des
		//  Dialogfelds über OK zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// ZU ERLEDIGEN: Fügen Sie hier Code ein, um ein Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
