
// PS_ServApp.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "PS_ServApp.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "PS_ServAppDoc.h"
#include "PS_ServAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPS_ServAppApp

BEGIN_MESSAGE_MAP(CPS_ServAppApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CPS_ServAppApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CPS_ServAppApp 생성

CPS_ServAppApp::CPS_ServAppApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CPS_ServAppApp 개체입니다.

CPS_ServAppApp theApp;


// CPS_ServAppApp 초기화

BOOL CPS_ServAppApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("PS_ServApp"));
	LoadStdProfileSettings(0);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	CleanState ();

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_PS_ServAppTYPE,
		RUNTIME_CLASS(CPS_ServAppDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CPS_ServAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;	// 처음 실행되는 view 를 막음

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	// 메인 도면을 맨 위에 띄움[S]
	INFO_JUST_MADE_VIEW *pIJMV = &glInfoGlobal.unGVA.iGVA.iJMV;
	
	pIJMV->bInfoCopyOK = FALSE;
	pIJMV->idxMainBDI = 0;
	pIJMV->idxSubBDI = -1;
	DoFileNew ();
	while (pIJMV->bInfoCopyOK == FALSE)
	{
		Sleep (0);
	}
	
	//pIJMV->bInfoCopyOK = FALSE;
	//pIJMV->idxMainBDI = 1;
	//pIJMV->idxSubBDI = -1;
	//DoFileNew ();
	//while (pIJMV->bInfoCopyOK == FALSE)
	//{
	//	Sleep (0);
	//}

	//pIJMV->bInfoCopyOK = FALSE;
	//pIJMV->idxMainBDI = 2;
	//pIJMV->idxSubBDI = -1;
	//DoFileNew ();
	//while (pIJMV->bInfoCopyOK == FALSE)
	//{
	//	Sleep (0);
	//}

	//pIJMV->bInfoCopyOK = FALSE;
	//pIJMV->idxMainBDI = 3;
	//pIJMV->idxSubBDI = -1;
	//DoFileNew ();
	//while (pIJMV->bInfoCopyOK == FALSE)
	//{
	//	Sleep (0);
	//}
	//glInfoGlobal.iBDA.bufBDI[0].iVS.pView->GetParent ()->ShowWindow (SW_SHOWNORMAL);
	//glInfoGlobal.iBDA.bufBDI[1].iVS.pView->GetParent ()->ShowWindow (SW_SHOWNORMAL);
//	pMainFrame->OnWindowLeftRight ();
	// 메인 도면을 맨 위에 띄움[E]

	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CPS_ServAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CPS_ServAppApp 사용자 지정 로드/저장 메서드

void CPS_ServAppApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CPS_ServAppApp::LoadCustomState()
{
}

void CPS_ServAppApp::SaveCustomState()
{
}

// CPS_ServAppApp 메시지 처리기

void CPS_ServAppApp::DoFileNew ()
{
	OnFileNew ();
}


