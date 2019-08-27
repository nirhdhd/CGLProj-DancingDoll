
// CGLProjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGLProj.h"
#include "CGLProjDlg.h"
#include "afxdialogex.h"
#include <windows.h> //trying
#include "Mmsystem.h";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCGLProjDlg dialog



CCGLProjDlg::CCGLProjDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGLPROJ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCGLProjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCGLProjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CCGLProjDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CCGLProjDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CCGLProjDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CCGLProjDlg::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CCGLProjDlg::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CCGLProjDlg::OnDeltaposSpin6)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CCGLProjDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CCGLProjDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CCGLProjDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &CCGLProjDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCGLProjDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCGLProjDlg::OnBnClickedButton3)
	
END_MESSAGE_MAP()


// CCGLProjDlg message handlers

BOOL CCGLProjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CWnd* panel1 = GetDlgItem(IDC_PICTURE);
	ptrView = new CGlView(panel1);
	playSound = true;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCGLProjDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCGLProjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		//ptrView->Draw();
		ptrView->vDraw();

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCGLProjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Translate buttons //////////////////////////////////////////

void CCGLProjDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	ptrView->setXShift(ptrView->getXShift() + pNMUpDown->iDelta*-0.1f);
	ptrView->vDraw();

	*pResult = 0;
}


void CCGLProjDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ptrView->setYShift(ptrView->getYShift() + pNMUpDown->iDelta*-0.1f);
	ptrView->vDraw();
	*pResult = 0;
}


void CCGLProjDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ptrView->setZShift(ptrView->getZShift() + pNMUpDown->iDelta*-0.1f);
	ptrView->vDraw();
	*pResult = 0;
}

// Rotate buttons //////////////////////////////////////////


void CCGLProjDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ptrView->setXAngle(ptrView->getXAngle() + pNMUpDown->iDelta);
	ptrView->vDraw();
	*pResult = 0;
}


void CCGLProjDlg::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ptrView->setYAngle(ptrView->getYAngle() + pNMUpDown->iDelta);
	ptrView->vDraw();
	*pResult = 0;
}


void CCGLProjDlg::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	ptrView->setZAngle(ptrView->getZAngle() + pNMUpDown->iDelta);
	ptrView->vDraw();
	*pResult = 0;
}


void CCGLProjDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1) {

		//ptrView->setYAngle(ptrView->getYAngle() + 1);

		// hands animation
		static bool isMaxAngle = false;
		if (!isMaxAngle) {
			ptrView->setShoulderAngle(ptrView->getShoulderAngle() + 2);
			if (ptrView->getShoulderAngle() >= 80)
				isMaxAngle = true;
		}
		else {
			ptrView->setShoulderAngle(ptrView->getShoulderAngle() - 2);
			if (ptrView->getShoulderAngle() <= -10)
				isMaxAngle = false;
		}


	}
	else if (nIDEvent == 2) {
		// skirt animation
		ptrView->setSkirtAngle(ptrView->getSkirtAngle() + 1);

		static bool isMaxRadius = false;
		if (!isMaxRadius) {
			ptrView->setHeightSkirt(ptrView->getHeightSkirt() - 0.01);
			ptrView->setRadiusBaseSkirt(ptrView->getRadiusBaseSkirt() + 0.1);
			if (ptrView->getRadiusBaseSkirt() >= 5)
				isMaxRadius = true;
		}
		else {
			ptrView->setHeightSkirt(ptrView->getHeightSkirt() + 0.01);
			ptrView->setRadiusBaseSkirt(ptrView->getRadiusBaseSkirt() - 0.1);
			if (ptrView->getRadiusBaseSkirt() <= 2)
				isMaxRadius = false;
		}
	}
	else if (nIDEvent == 3) {
		ptrView->setDancerAngle(ptrView->getDancerAngle() + 1);
	}


	ptrView->vDraw();

	CDialogEx::OnTimer(nIDEvent);
}


void CCGLProjDlg::OnBnClickedCheck1()
{
	// ANIMATION 
	static bool flag = true;

	if (flag)
		SetTimer(1, 10, NULL);

	// stop timer and sound
	else {
		KillTimer(1);
	}
	flag = !flag;
}

void CCGLProjDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	static bool flag = true;
	if (flag)
		SetTimer(2, 10, NULL);
	else {
		KillTimer(2);
	}
	flag = !flag;
}


void CCGLProjDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	static bool flag = true;
	if (flag)
		SetTimer(3, 10, NULL);
	else {
		KillTimer(3);
	}
	flag = !flag;
}


void CCGLProjDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// TOGGLE
	ptrView->setOffsetTexture(ptrView->getOffsetTexture() + 1);
	ptrView->vDraw();

}

void CCGLProjDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	ptrView->setXShift(0);
	ptrView->setYShift(0);
	ptrView->setZShift(0);

	ptrView->setXAngle(0);
	ptrView->setYAngle(0);
	ptrView->setZAngle(0);

	ptrView->setShoulderAngle(20.0f);
	ptrView->setRadiusBaseSkirt(2.0f);
	ptrView->setHeightSkirt(1.7f);
	ptrView->setSkirtAngle(0);
	ptrView->setDancerAngle(0);
	ptrView->setOffsetTexture(0);
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	PlaySoundW(NULL, NULL, NULL);
	playSound = true;
	ptrView->vDraw();

}


void CCGLProjDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	
	if (playSound) {
		PlaySoundW(L"res/song.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
		playSound = false;
	}
	else {
		PlaySoundW(NULL, NULL, NULL);
		playSound = true;
	}

}











