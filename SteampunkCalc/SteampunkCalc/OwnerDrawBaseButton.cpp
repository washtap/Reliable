// OwnerDrawBaseButton.cpp : implementation file
//

#include "stdafx.h"
#include "OwnerDrawBaseButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBaseButton

// mask for control's type
#define BS_SS_TYPEMASK SS_TYPEMASK

COwnerDrawBaseButton::COwnerDrawBaseButton()
{
	m_bDefault = FALSE;
	m_bCanBeDefault = TRUE;
	
	// invalid value, since type still unknown
	m_nTypeStyle = BS_SS_TYPEMASK;
}

COwnerDrawBaseButton::~COwnerDrawBaseButton()
{
}


BEGIN_MESSAGE_MAP(COwnerDrawBaseButton, CButton)
	//{{AFX_MSG_MAP(COwnerDrawBaseButton)
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBaseButton message handlers

void COwnerDrawBaseButton::PreSubclassWindow() 
{
	// set initial control type
	m_nTypeStyle = GetButtonStyle() & BS_SS_TYPEMASK;

	// set initial default state flag
	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		// enable default state handling for push buttons
		m_bCanBeDefault = TRUE;

		// set default state for a default button
		m_bDefault = TRUE;

		// adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	}
	else /*if (m_nTypeStyle == BS_PUSHBUTTON)*/
	{
		// enable default state handling for push buttons
		m_bCanBeDefault = TRUE;
	}

	// you should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// switch to owner-draw
	ModifyStyle(BS_SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

UINT COwnerDrawBaseButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	// tell the system if we want default state handling
	// (losing default state always allowed)
    UINT nType = GetControlType ();
	if (BS_RADIOBUTTON == (BS_RADIOBUTTON & nType) ||
		BS_AUTORADIOBUTTON == (BS_AUTORADIOBUTTON & nType)) 
	{
		nCode |= DLGC_RADIOBUTTON;
	}
	else 
	{
	    if (m_bCanBeDefault || m_bDefault)
		    nCode |= (m_bDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);
    }

	return nCode;
}

LRESULT COwnerDrawBaseButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	UINT nNewType = (wParam & BS_SS_TYPEMASK);

	// update default state flag
	if (nNewType == BS_DEFPUSHBUTTON)
	{
		// we must like default state at this point
		ASSERT(m_bCanBeDefault);

		m_bDefault = TRUE;
	}
	else if (nNewType == BS_PUSHBUTTON)
	{
		// losing default state always allowed
		m_bDefault = FALSE;
	}

	// can't change control type after owner-draw is set.
	// let the system process changes to other style bits
	// and redrawing, while keeping owner-draw style
	return DefWindowProc(BM_SETSTYLE,
		(wParam & ~BS_SS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawBaseButton methods

UINT COwnerDrawBaseButton::GetControlType()
{
	return m_nTypeStyle;
}

BOOL COwnerDrawBaseButton::IsDefault()
{
	// if we have default state, we must like it!
	ASSERT((m_bCanBeDefault && m_bDefault) == m_bDefault);

	return m_bDefault;
}

void COwnerDrawBaseButton::EnableDefault(BOOL bEnable)
{
	m_bCanBeDefault = bEnable;

	// disabling default when control has default state
	// needs removing the default state
	if (!bEnable && m_bDefault)
	{
		// remove default state
		SendMessage(BM_SETSTYLE, 
			(GetStyle() & ~BS_SS_TYPEMASK) | BS_PUSHBUTTON, TRUE);
		ASSERT(m_bDefault == FALSE);

		// update default button
		LRESULT lRes = GetParent()->SendMessage(DM_GETDEFID);
		if (HIWORD(lRes) == DC_HASDEFID)
			GetParent()->SendMessage(DM_SETDEFID, LOWORD(lRes));
	}
}
