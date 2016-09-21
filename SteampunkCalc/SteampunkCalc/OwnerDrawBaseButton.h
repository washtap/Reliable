// Description
//		COwnerDrawBaseButton Class Header file

#if !defined(_OWNERDRAWBASEBUTTON_)
#define _OWNERDRAWBASEBUTTON_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBaseButton window 
//
//	Allows Owner Draw buttons to handle its default state as a normal button 
//	does
//
class COwnerDrawBaseButton : public CButton
{
// Construction
public:
	COwnerDrawBaseButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawBaseButton)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwnerDrawBaseButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnerDrawBaseButton)
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bCanBeDefault;
	BOOL m_bDefault;
	UINT m_nTypeStyle;

protected:
	// used to enable default state handling
	void EnableDefault(BOOL bEnable);

	// use to know when the control has default state
	BOOL IsDefault();

	// use to know the type of control to draw
	UINT GetControlType();
};

#endif // !defined(_OWNERDRAWBASEBUTTON_)

