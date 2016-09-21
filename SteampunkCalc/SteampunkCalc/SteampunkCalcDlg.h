
// SteampunkCalcDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "PatStatic.h"
#include "PatButton.h"
#include ".\IsiLcdLib\IsiLcdDisplay.h"

// CSteampunkCalcDlg dialog
class CSteampunkCalcDlg : public CDialogEx
{
// Construction
public:
	CSteampunkCalcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEAMPUNKCALC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Objects to hold current calculation
	CString m_strCurrentEntry;		// Holds number text as it's being added
	double m_fResultsWndValue;		// Full precision value of results window text
	double m_fRunningTotal;			// Current answer value
	char m_cLastOp;					// Last operator key pressed
	int m_nClearBtnStatus;			// Counter for managing Clear Entry / Clear button
	double m_fMemory;				// Holds the value in calculator's memory
	UINT m_nLastCmd;				// Holds last command (based on button IDs)
	CString m_strNumFormat;			// Formatting string used to display in results window
	bool m_bResetCurrentEntry;

	// Calculator methods
	void UpdateLcdWndChar(LPCTSTR szNewChr);
	void UpdateLcdWnd();
	void ProcessEquals(void);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
	CPatButton m_button0;
	CPatButton m_button1;
	CPatButton m_button2;
	CPatButton m_button3;
	CPatButton m_button4;
	CPatButton m_button5;
	CPatButton m_button6;
	CPatButton m_button7;
	CPatButton m_button8;
	CPatButton m_button9;
	CPatButton m_buttonMemoryAdd;
	CPatButton m_buttonMemorySubtract;
	CPatButton m_buttonMemoryRecall;
	CPatButton m_buttonMemoryClear;
	CPatButton m_buttonSign;
	CPatButton m_buttonSquareRoot;
	CPatButton m_buttonDivide;
	CPatButton m_buttonPercent;
	CPatButton m_buttonMultiply;
	CPatButton m_buttonFraction;
	CPatButton m_buttonSubtract;
	CPatButton m_buttonClear;
	CPatButton m_buttonDecimal;
	CPatButton m_buttonEquals;
	CPatButton m_buttonAdd;
	CPatButton m_buttonClose;
	CIsiLcdDisplay m_lcdDisplay;
	CPatStatic m_staticBin0;
	CPatStatic m_staticBin1;
	CPatStatic m_staticBin2;
	CPatStatic m_staticBin3;
	CPatStatic m_staticBin4;
	CPatStatic m_staticBin5;
	CPatStatic m_staticBin6;
	CPatStatic m_staticBin7;

	afx_msg void OnBnClickedScDecimal();
	afx_msg void OnBnClickedSc0();
	afx_msg void OnBnClickedScClose();
	afx_msg void OnBnClickedSc1();
	afx_msg void OnBnClickedSc2();
	afx_msg void OnBnClickedSc3();
	afx_msg void OnBnClickedSc4();
	afx_msg void OnBnClickedSc5();
	afx_msg void OnBnClickedSc6();
	afx_msg void OnBnClickedSc7();
	afx_msg void OnBnClickedSc8();
	afx_msg void OnBnClickedSc9();
	afx_msg void OnBnClickedScEquals();
	afx_msg void OnBnClickedScAdd();
	afx_msg void OnBnClickedScSubtract();
	afx_msg void OnBnClickedScMultiply();
	afx_msg void OnBnClickedScDivide();
	afx_msg void OnBnClickedScSign();
	afx_msg void OnBnClickedScMemoryAdd();
	afx_msg void OnBnClickedScMemorySubtract();
	afx_msg void OnBnClickedScMemoryRecall();
	afx_msg void OnBnClickedScMemoryClear();
	afx_msg void OnBnClickedScSquareRoot();
	afx_msg void OnBnClickedScPercent();
	afx_msg void OnBnClickedScFraction();
	afx_msg void OnBnClickedScClear();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
