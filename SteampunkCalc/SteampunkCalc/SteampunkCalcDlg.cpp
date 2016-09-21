
// SteampunkCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SteampunkCalc.h"
#include "SteampunkCalcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )

#define TRANSPARENT_BITMAP_COLOR RGB(1, 11, 21)

#define DIALOG_SIZE_X 900
#define DIALOG_SIZE_Y 900

#define LCD_BORDER_SIZE 10

#define BUTTON_SIZE_X 52
#define BUTTON_SIZE_Y 52

// size X & Y of the knobs  
#define BIN_STATIC_SIZE 32

// Size of the array containing control IDs of calculator buttons
#define BUTTON_ARRAY_X 5
#define BUTTON_ARRAY_Y 5

// 2D array of calculator buttons ... used to arrange the buttons in an even grid.
int button_array[BUTTON_ARRAY_Y][BUTTON_ARRAY_X] =
{ 
	{ IDC_SC_MEMORY_ADD, IDC_SC_MEMORY_SUBTRACT, IDC_SC_MEMORY_RECALL, IDC_SC_MEMORY_CLEAR, IDC_SC_SIGN },
	{ IDC_SC_SQUARE_ROOT, IDC_SC_7, IDC_SC_8, IDC_SC_9, IDC_SC_DIVIDE },
	{ IDC_SC_PERCENT, IDC_SC_4, IDC_SC_5, IDC_SC_6, IDC_SC_MULTIPLY },
	{ IDC_SC_FRACTION, IDC_SC_1, IDC_SC_2, IDC_SC_3, IDC_SC_SUBTRACT },
	{ IDC_SC_CLEAR, IDC_SC_0, IDC_SC_DECIMAL, IDC_SC_EQUALS, IDC_SC_ADD }
};

// array of static control IDs  ... used to arrange the controls evenly
int static_bin_array[8] =
{ 
	IDC_BIN0, IDC_BIN1, IDC_BIN2, IDC_BIN3, IDC_BIN4, IDC_BIN5, IDC_BIN6, IDC_BIN7 
};

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


// CSteampunkCalcDlg dialog
CSteampunkCalcDlg::CSteampunkCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STEAMPUNKCALC_DIALOG, pParent),
	m_strCurrentEntry(""),
	m_fRunningTotal(0.0),
	m_cLastOp(0),
	m_nClearBtnStatus(0),
	m_fMemory(0.0),
	m_nLastCmd(0),
	m_strNumFormat("%g"),
	m_fResultsWndValue(0.0),
	m_bResetCurrentEntry(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSteampunkCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SC_CLOSE, m_buttonClose);
	DDX_Control(pDX, IDC_SC_0, m_button0);
	DDX_Control(pDX, IDC_SC_1, m_button1);
	DDX_Control(pDX, IDC_SC_2, m_button2);
	DDX_Control(pDX, IDC_SC_3, m_button3);
	DDX_Control(pDX, IDC_SC_4, m_button4);
	DDX_Control(pDX, IDC_SC_5, m_button5);
	DDX_Control(pDX, IDC_SC_6, m_button6);
	DDX_Control(pDX, IDC_SC_7, m_button7);
	DDX_Control(pDX, IDC_SC_8, m_button8);
	DDX_Control(pDX, IDC_SC_9, m_button9);
	DDX_Control(pDX, IDC_SC_MEMORY_ADD, m_buttonMemoryAdd);
	DDX_Control(pDX, IDC_SC_MEMORY_SUBTRACT, m_buttonMemorySubtract);
	DDX_Control(pDX, IDC_SC_MEMORY_RECALL, m_buttonMemoryRecall);
	DDX_Control(pDX, IDC_SC_MEMORY_CLEAR, m_buttonMemoryClear);
	DDX_Control(pDX, IDC_SC_SIGN, m_buttonSign);
	DDX_Control(pDX, IDC_SC_SQUARE_ROOT, m_buttonSquareRoot);
	DDX_Control(pDX, IDC_SC_DIVIDE, m_buttonDivide);
	DDX_Control(pDX, IDC_SC_PERCENT, m_buttonPercent);
	DDX_Control(pDX, IDC_SC_MULTIPLY, m_buttonMultiply);
	DDX_Control(pDX, IDC_SC_FRACTION, m_buttonFraction);
	DDX_Control(pDX, IDC_SC_SUBTRACT, m_buttonSubtract);
	DDX_Control(pDX, IDC_SC_CLEAR, m_buttonClear);
	DDX_Control(pDX, IDC_SC_DECIMAL, m_buttonDecimal);
	DDX_Control(pDX, IDC_SC_EQUALS, m_buttonEquals);
	DDX_Control(pDX, IDC_SC_ADD, m_buttonAdd);

	DDX_Control(pDX, IDC_LCD_DISPLAY, m_lcdDisplay);

	DDX_Control(pDX, IDC_BIN0, m_staticBin0);
	DDX_Control(pDX, IDC_BIN1, m_staticBin1);
	DDX_Control(pDX, IDC_BIN2, m_staticBin2);
	DDX_Control(pDX, IDC_BIN3, m_staticBin3);
	DDX_Control(pDX, IDC_BIN4, m_staticBin4);
	DDX_Control(pDX, IDC_BIN5, m_staticBin5);
	DDX_Control(pDX, IDC_BIN6, m_staticBin6);
	DDX_Control(pDX, IDC_BIN7, m_staticBin7);
}

BEGIN_MESSAGE_MAP(CSteampunkCalcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_SC_DECIMAL, &CSteampunkCalcDlg::OnBnClickedScDecimal)
	ON_BN_CLICKED(IDC_SC_0, &CSteampunkCalcDlg::OnBnClickedSc0)
	ON_BN_CLICKED(IDC_SC_CLOSE, &CSteampunkCalcDlg::OnBnClickedScClose)
	ON_BN_CLICKED(IDC_SC_1, &CSteampunkCalcDlg::OnBnClickedSc1)
	ON_BN_CLICKED(IDC_SC_2, &CSteampunkCalcDlg::OnBnClickedSc2)
	ON_BN_CLICKED(IDC_SC_3, &CSteampunkCalcDlg::OnBnClickedSc3)
	ON_BN_CLICKED(IDC_SC_4, &CSteampunkCalcDlg::OnBnClickedSc4)
	ON_BN_CLICKED(IDC_SC_5, &CSteampunkCalcDlg::OnBnClickedSc5)
	ON_BN_CLICKED(IDC_SC_6, &CSteampunkCalcDlg::OnBnClickedSc6)
	ON_BN_CLICKED(IDC_SC_7, &CSteampunkCalcDlg::OnBnClickedSc7)
	ON_BN_CLICKED(IDC_SC_8, &CSteampunkCalcDlg::OnBnClickedSc8)
	ON_BN_CLICKED(IDC_SC_9, &CSteampunkCalcDlg::OnBnClickedSc9)
	ON_BN_CLICKED(IDC_SC_EQUALS, &CSteampunkCalcDlg::OnBnClickedScEquals)
	ON_BN_CLICKED(IDC_SC_ADD, &CSteampunkCalcDlg::OnBnClickedScAdd)
	ON_BN_CLICKED(IDC_SC_SUBTRACT, &CSteampunkCalcDlg::OnBnClickedScSubtract)
	ON_BN_CLICKED(IDC_SC_MULTIPLY, &CSteampunkCalcDlg::OnBnClickedScMultiply)
	ON_BN_CLICKED(IDC_SC_DIVIDE, &CSteampunkCalcDlg::OnBnClickedScDivide)
	ON_BN_CLICKED(IDC_SC_SIGN, &CSteampunkCalcDlg::OnBnClickedScSign)
	ON_BN_CLICKED(IDC_SC_MEMORY_ADD, &CSteampunkCalcDlg::OnBnClickedScMemoryAdd)
	ON_BN_CLICKED(IDC_SC_MEMORY_SUBTRACT, &CSteampunkCalcDlg::OnBnClickedScMemorySubtract)
	ON_BN_CLICKED(IDC_SC_MEMORY_RECALL, &CSteampunkCalcDlg::OnBnClickedScMemoryRecall)
	ON_BN_CLICKED(IDC_SC_MEMORY_CLEAR, &CSteampunkCalcDlg::OnBnClickedScMemoryClear)
	ON_BN_CLICKED(IDC_SC_SQUARE_ROOT, &CSteampunkCalcDlg::OnBnClickedScSquareRoot)
	ON_BN_CLICKED(IDC_SC_PERCENT, &CSteampunkCalcDlg::OnBnClickedScPercent)
	ON_BN_CLICKED(IDC_SC_FRACTION, &CSteampunkCalcDlg::OnBnClickedScFraction)
	ON_BN_CLICKED(IDC_SC_CLEAR, &CSteampunkCalcDlg::OnBnClickedScClear)
END_MESSAGE_MAP()


// CSteampunkCalcDlg message handlers

BOOL CSteampunkCalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Tricky stuff to make the dialog transparent. 
	SetBackgroundColor(TRANSPARENT_BITMAP_COLOR);
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(GetSafeHwnd(), TRANSPARENT_BITMAP_COLOR, 255, LWA_COLORKEY);

	// Make ourself big ... same size as the IDB_PNG_FRAME resource.
	CRect Rect;
	GetClientRect(&Rect);
	Rect.right = Rect.left + DIALOG_SIZE_X;
	Rect.bottom = Rect.top + DIALOG_SIZE_Y;
	MoveWindow(&Rect, TRUE);
	CenterWindow();

	// Line all our buttons up iin a grid in the center and a little bit down.
	int posY = (DIALOG_SIZE_Y - (BUTTON_ARRAY_Y * BUTTON_SIZE_Y)) / 2 + 40;
	for (int y = 0; y < BUTTON_ARRAY_Y; y++)
	{
		int posX = (DIALOG_SIZE_X - (BUTTON_ARRAY_X * BUTTON_SIZE_X)) / 2;
		for (int x = 0; x < BUTTON_ARRAY_X; x++)
		{
			// dynamic_cast because we want to be sure it's a CPatButton
			CPatButton *pButton = dynamic_cast<CPatButton*>(GetDlgItem(button_array[y][x]));
			if (pButton != NULL)
			{
				pButton->MoveWindow(posX, posY, BUTTON_SIZE_X, BUTTON_SIZE_Y);
				pButton->SetImageResourceIDs(IDB_PNG_BUTTON_NORMAL, IDB_PNG_BUTTON_PRESSED);
			}
						
			posX += BUTTON_SIZE_X;
		}
		posY += BUTTON_SIZE_X;
	}

	// Now let's move our little binary indicator knobs below the keys
	posY += 10;
	
	int posX = (DIALOG_SIZE_X - (8 * BIN_STATIC_SIZE)) / 2;
	for (int x = 0; x < 8; x++)
	{
		// dynamic_cast because we want to be sure it's a CPatStatic
		CPatStatic *pStatic = dynamic_cast<CPatStatic*>(GetDlgItem(static_bin_array[x]));
		if (pStatic != NULL)
		{
			pStatic->MoveWindow(posX, posY, BIN_STATIC_SIZE, BIN_STATIC_SIZE);
			pStatic->SetImageResourceIDs(IDB_PNG_KNOB_ON, IDB_PNG_KNOB_OFF);
		}

		posX += BIN_STATIC_SIZE;
	}
	// Move the close button to the right location ... absolute coordinates base on the IDB_PNG_FRAME
	m_buttonClose.MoveWindow(603, 410, 80, 80);
	m_buttonClose.SetImageResourceIDs(IDB_PNG_EXIT, IDB_PNG_EXIT);

	// Calc the position and size for our LCD window directly above the keys.
	posY = (DIALOG_SIZE_Y - (BUTTON_ARRAY_Y * BUTTON_SIZE_Y)) / 2 - BUTTON_SIZE_Y- LCD_BORDER_SIZE*2;
	posX = (DIALOG_SIZE_X - (BUTTON_ARRAY_X * BUTTON_SIZE_X)) / 2;
	m_lcdDisplay.MoveWindow(posX,posY, BUTTON_ARRAY_X * BUTTON_SIZE_X, (int)((float)BUTTON_SIZE_Y*1.2));

	// Set the LCD in correct mode ... sadly there is not right justified ability in it.
	m_lcdDisplay.SetSegmentStyle(Segment16, FALSE);
	m_lcdDisplay.SetValueNbDigit(9, 8);
	m_lcdDisplay.ShowSign( FALSE );
	m_lcdDisplay.SetColor(RGB(150, 97, 0)); // Orange
	m_lcdDisplay.StringMode(TRUE, FALSE);
	m_lcdDisplay.GetBorder()->SetStyle(CGdiBorder::Chrome);
	m_lcdDisplay.GetBorder()->SetSize(LCD_BORDER_SIZE);
	m_lcdDisplay.InvalidateBorder();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSteampunkCalcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSteampunkCalcDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSteampunkCalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSteampunkCalcDlg::OnEraseBkgnd(CDC *pDC)
{
	CRect clientRect;

	// paint ourself in a transparent color.
	GetClientRect(&clientRect);
	pDC->FillSolidRect(clientRect, TRANSPARENT_BITMAP_COLOR);  // paint background in transparent color

	// Now display our fancy PNG background
	CGdiPlusBitmapResource bitmapResoure;
	if (bitmapResoure.Load(IDB_PNG_FRAME, _T("PNG")))
	{
		Gdiplus::Graphics grphx(pDC->GetSafeHdc());
		grphx.DrawImage(bitmapResoure.m_pBitmap, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	return FALSE;
}

// Someone pressed the big power button!
void CSteampunkCalcDlg::OnBnClickedScClose()
{
	PostMessage(WM_CLOSE, 0, 0);
}

void CSteampunkCalcDlg::OnBnClickedSc0()
{
	UpdateLcdWndChar(_T("0"));
}

void CSteampunkCalcDlg::OnBnClickedSc1()
{
	UpdateLcdWndChar(_T("1"));
}


void CSteampunkCalcDlg::OnBnClickedSc2()
{
	UpdateLcdWndChar(_T("2"));
}


void CSteampunkCalcDlg::OnBnClickedSc3()
{
	UpdateLcdWndChar(_T("3"));
}


void CSteampunkCalcDlg::OnBnClickedSc4()
{
	UpdateLcdWndChar(_T("4"));
}


void CSteampunkCalcDlg::OnBnClickedSc5()
{
	UpdateLcdWndChar(_T("5"));
}


void CSteampunkCalcDlg::OnBnClickedSc6()
{
	UpdateLcdWndChar(_T("6"));
}


void CSteampunkCalcDlg::OnBnClickedSc7()
{
	UpdateLcdWndChar(_T("7"));
}


void CSteampunkCalcDlg::OnBnClickedSc8()
{
	UpdateLcdWndChar(_T("8"));
}


void CSteampunkCalcDlg::OnBnClickedSc9()
{
	UpdateLcdWndChar(_T("9"));
}


void CSteampunkCalcDlg::OnBnClickedScDecimal()
{
	UpdateLcdWndChar(_T("."));
}

void CSteampunkCalcDlg::OnBnClickedScEquals()
{
	m_nClearBtnStatus = 0;
	if (m_nLastCmd != 0)
		ProcessEquals();
}


void CSteampunkCalcDlg::OnBnClickedScAdd()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fRunningTotal += m_fResultsWndValue;
	m_strCurrentEntry = "+";
	m_cLastOp = '+';
}


void CSteampunkCalcDlg::OnBnClickedScSubtract()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fRunningTotal += m_fResultsWndValue;
	m_strCurrentEntry = "-";
	m_cLastOp = '-';
}


void CSteampunkCalcDlg::OnBnClickedScMultiply()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fRunningTotal = m_fResultsWndValue;
	m_strCurrentEntry = "*";
	m_cLastOp = '*';
}


void CSteampunkCalcDlg::OnBnClickedScDivide()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fRunningTotal = m_fResultsWndValue;
	m_strCurrentEntry = "/";
	m_cLastOp = '/';
}


void CSteampunkCalcDlg::OnBnClickedScSign()
{
	m_fResultsWndValue = -m_fResultsWndValue;
	m_strCurrentEntry.Format(m_strNumFormat, m_fResultsWndValue);
}


void CSteampunkCalcDlg::OnBnClickedScMemoryAdd()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fMemory += m_fResultsWndValue;
	m_fRunningTotal = m_fResultsWndValue;
}


void CSteampunkCalcDlg::OnBnClickedScMemorySubtract()
{
	if (m_cLastOp != 0)
		ProcessEquals();
	m_fMemory -= m_fResultsWndValue;
	m_fRunningTotal = m_fResultsWndValue;
}


void CSteampunkCalcDlg::OnBnClickedScMemoryRecall()
{
	m_fResultsWndValue = m_fMemory;
	m_strCurrentEntry.Format(m_strNumFormat, m_fResultsWndValue);
}


void CSteampunkCalcDlg::OnBnClickedScMemoryClear()
{
	m_fMemory = 0.0;
}


void CSteampunkCalcDlg::OnBnClickedScSquareRoot()
{
	m_fResultsWndValue = sqrt(m_fResultsWndValue);
	m_strCurrentEntry.Format(m_strNumFormat, m_fResultsWndValue);
}


void CSteampunkCalcDlg::OnBnClickedScPercent()
{
	switch (m_cLastOp)
	{
	case '+':
		m_fRunningTotal *= ((100.0 + m_fResultsWndValue) / 100.0);
		break;
	case '-':
		m_fRunningTotal *= ((100.0 + m_fResultsWndValue) / 100.0);
		break;
	case '*':
		m_fRunningTotal *= (m_fResultsWndValue / 100.0);
		break;
	case '/':
		m_fRunningTotal /= (m_fResultsWndValue / 100.0);
		break;
	}
	m_fResultsWndValue = m_fRunningTotal;
	m_strCurrentEntry.Format(m_strNumFormat, m_fResultsWndValue);
	m_cLastOp = 0;
}


void CSteampunkCalcDlg::OnBnClickedScFraction()
{
	if (!m_strCurrentEntry.IsEmpty())
	{
		m_fResultsWndValue = 1.0 / m_fResultsWndValue;
		m_strCurrentEntry.Format(m_strNumFormat, m_fResultsWndValue);
		m_fRunningTotal = m_fResultsWndValue;
	}
}


void CSteampunkCalcDlg::OnBnClickedScClear()
{
	m_nClearBtnStatus++;
	if (m_nClearBtnStatus == 1)
	{
		m_strCurrentEntry = m_cLastOp;
		m_fResultsWndValue = 0.0;
	}
	else
	{
		m_fRunningTotal = 0.0;
		m_nClearBtnStatus = 0;
		m_cLastOp = 0;
	}
}

BOOL CSteampunkCalcDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL ret = CDialogEx::OnCommand(wParam, lParam);

	// This is needed to be sure the LCD is updated after every button press.
	UpdateLcdWnd();

	// Just need to know there was a command
	m_nLastCmd = (UINT)wParam;

	return ret;
}


BOOL CSteampunkCalcDlg::PreTranslateMessage(MSG* pMsg)
{
	// Added this for keyboard support. Needs to be "PreTranslateMessage" because "OnKeyDown"
	// doesn't work with CDialog.

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			// Number keys
		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			OnBnClickedScDecimal();
			break;
		case VK_NUMPAD0:
		case 0x30:
			OnBnClickedSc0();
			break;
		case VK_NUMPAD1:
		case 0x31:
			OnBnClickedSc1();
			break;
		case VK_NUMPAD2:
		case 0x32:
			OnBnClickedSc2();
			break;
		case VK_NUMPAD3:
		case 0x33:
			OnBnClickedSc3();
			break;
		case VK_NUMPAD4:
		case 0x34:
			OnBnClickedSc4();
			break;
		case VK_NUMPAD5:
			OnBnClickedSc5();
			break;
		case VK_NUMPAD6:
		case 0x36:
			OnBnClickedSc6();
			break;
		case VK_NUMPAD7:
		case 0x37:
			OnBnClickedSc7();
			break;
		case VK_NUMPAD8:
		case 0x38:
			if (!IsSHIFTpressed())
				OnBnClickedSc8();
			else
				OnBnClickedScMultiply();
			break;
		case VK_NUMPAD9:
		case 0x39:
			OnBnClickedSc8();
			break;
			// Operator keys
		case VK_MULTIPLY:
			OnBnClickedScMultiply();
			break;
		case VK_DIVIDE:
		case 0xDC:
			OnBnClickedScDivide();
			break;
		case VK_ADD:
			OnBnClickedScAdd();
			break;
		case VK_SUBTRACT:
		case 0xBD:
			OnBnClickedScSubtract();
			break;
			// Equals
		case VK_RETURN:
		case 0xBB:
			if (IsSHIFTpressed())
				OnBnClickedScAdd();
			else
				OnBnClickedScEquals();
			break;
			// CE/C
		case VK_ESCAPE:
			OnBnClickedScClear();
			break;
		case 0x35:		// 'Shift+%' or '5'
			if (IsSHIFTpressed())
				OnBnClickedScPercent();
			else
				OnBnClickedSc5();
			break;
		case 0x8:		// Backspace
			if (!m_strCurrentEntry.IsEmpty())
			{
				m_strCurrentEntry = m_strCurrentEntry.Left(m_strCurrentEntry.GetLength() - 1);
				m_fResultsWndValue = _ttof(m_strCurrentEntry);
				m_lcdDisplay.SetWindowText(m_strCurrentEntry);
			}
			break;
		}

		// This is needed to be sure the LCD is updated after every button press.
		UpdateLcdWnd();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// Update results window
void CSteampunkCalcDlg::UpdateLcdWnd()
{
	m_lcdDisplay.SetWindowText(m_strCurrentEntry);

	// Let's set up our knobs using the first 8 bits of our calculator current result.
	BYTE mask = 0x1;
	for (int i = 0; i < 8; i++)
	{
		// dynamic_cast because we want to be sure it's a CPatStatic
		CPatStatic *pStatic = dynamic_cast<CPatStatic*>(GetDlgItem(static_bin_array[i]));
		if (pStatic != NULL)
		{
			BOOL bEnable = ((BYTE)m_fResultsWndValue & mask) ? TRUE : FALSE;
			pStatic->EnableWindow(bEnable);
		}
		mask <<= 1;
	}

	// Set focus here to capture keystrokes
	SetFocus();
}

// Updates results window variables with the character passed as an argument
void CSteampunkCalcDlg::UpdateLcdWndChar(LPCTSTR szNewChr)
{
	// Ensure we are not trying to add a second decimal point!
	if (szNewChr == _T(".") && m_strCurrentEntry.Find(_T(".")) != -1)
		return;

	if (m_bResetCurrentEntry)
	{
		m_strCurrentEntry = _T("");
		m_bResetCurrentEntry = false;
	}
	// Update the private member variables
	m_strCurrentEntry += szNewChr;
	CString strCurrentEntry(m_strCurrentEntry);
	strCurrentEntry.Remove('*');
	strCurrentEntry.Remove('/');
	m_fResultsWndValue = _ttof(strCurrentEntry);
	m_nClearBtnStatus = 0;
}


// Process an 'Equals' button press
void CSteampunkCalcDlg::ProcessEquals(void)
{
	if (m_strCurrentEntry.GetLength() > 1)
	{
		switch (m_cLastOp)
		{
		case '+':
			m_fRunningTotal += m_fResultsWndValue;
			break;
		case '-':
			m_fRunningTotal += m_fResultsWndValue;
			break;
		case '*':
			if (m_strCurrentEntry.GetLength() > 1)
				m_fRunningTotal *= m_fResultsWndValue;
			break;
		case '/':
			if (m_fResultsWndValue != 0.0)
				m_fRunningTotal /= m_fResultsWndValue;
			else
			{
				m_fRunningTotal = 0.0;
				m_strCurrentEntry = "0.0";
			}
			break;
		default: // Assume an equals
			m_bResetCurrentEntry = true;
		}
	}
	m_strCurrentEntry.Format(m_strNumFormat, m_fRunningTotal);
	m_fResultsWndValue = m_fRunningTotal;
	m_fRunningTotal = 0.0;
	m_cLastOp = 0;
}






