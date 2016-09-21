// ---------------------------------------------------------------------------
//
// Description
//		CPatButton Class Header file
//
// ---------------------------------------------------------------------------
#if !defined(_PATBUTTON_)
#define _PATBUTTON_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "OwnerDrawBaseButton.h"
#include "CGdiPlusBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CPatButton window
// 
// A simple extension to the CButton class to handle the drawing of transparent
// PNG files as a button background.
//
class CPatButton : public COwnerDrawBaseButton 
{
public:
// Construction
	CPatButton();
// Destruction
	virtual ~CPatButton();

// Operations
public:
	void SetImageResourceIDs(int normalID, int pressedID = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	CFont m_font;		// font for text (could make this changeable)
	HDC m_hBGdc;		// Background which is a copy of the parents window behind the button.
	HBITMAP m_hBGbmp;	// The DC for the background.
	bool m_bPressed;	// Is the button pressed?
	int m_normalID;		// PNG ID for Normal state
	int m_pressedID;	// PNG ID for Pressed state

	// Generated message map functions
protected:
	//{{AFX_MSG(CPatButton)
	//}}AFX_MSG
	afx_msg HBRUSH CtlColor (CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
private:
	void DrawBackground(CDC *pDC);

};

#endif // !defined(_PATBUTTON_)

