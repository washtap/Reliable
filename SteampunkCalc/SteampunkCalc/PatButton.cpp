// PatButton.cpp : implementation file
//   Extends CButton class. Displays transparent PNG files using GDI+ in the button control.

#include "stdafx.h"
#include "PatButton.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatButton

/////////////////////////////////////////////////////////////////////////////
// Construction

CPatButton::CPatButton() : 
	COwnerDrawBaseButton(), 
	m_bPressed(false),
	m_normalID(-1),
	m_pressedID(-1)
{
	// Setup a bitmap and DC to put background in (easy transparency)
	m_hBGbmp = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BUTTON_NORMAL));
	m_hBGdc = ::CreateCompatibleDC(NULL);
	SelectObject(m_hBGdc, m_hBGbmp);
}

/////////////////////////////////////////////////////////////////////////////
// Destruction

CPatButton::~CPatButton() 
{
	::DeleteDC(m_hBGdc);
	::DeleteObject(m_hBGbmp);
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CPatButton, COwnerDrawBaseButton)
	//{{AFX_MSG_MAP(CPatButton)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatButton message handlers

void CPatButton::PreSubclassWindow() 
{
	// Determine what type of button this is
	COwnerDrawBaseButton::PreSubclassWindow ();

	// Grab a font!
	if (NULL == (HFONT)m_font) 
	{
		// If a font isn't already set, get the default font
		CFont* pFont = GetFont();
		if (!pFont) 
		{
			HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			if (NULL == hFont) 
			{
				hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
			}
			if (hFont) 
			{
				pFont = CFont::FromHandle(hFont);
			}
		}
		ASSERT (pFont->GetSafeHandle());
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		m_font.CreateFontIndirect(&lf);
	}

}


HBRUSH CPatButton::CtlColor (CDC* /*pDC*/, UINT /*nCtlColor*/) 
{
	return (HBRUSH)::GetStockObject (NULL_BRUSH); 
}

void CPatButton::SetImageResourceIDs(int normalID, int pressedID)
{
	m_normalID = normalID;	
	m_pressedID = pressedID;
}

void CPatButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC cdcMem;
	CBitmap bmp;

	CRect btnRect(lpDrawItemStruct->rcItem);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	DrawBackground(pDC);

	CGdiPlusBitmapResource bitmapResoure;

	// Draw the PNG for the right state.
	int resID = (GetState() & BST_PUSHED) ? m_pressedID : m_normalID;
	if (resID == -1) // perhaps no pressed id?
		resID = m_normalID;

	if ((resID != -1) && bitmapResoure.Load(resID, _T("PNG")))
	{
		Gdiplus::Graphics grphx(lpDrawItemStruct->hDC);
		grphx.DrawImage(bitmapResoure.m_pBitmap, 0, 0, btnRect.right - btnRect.left, btnRect.bottom - btnRect.top);
	}
	
	// Draw out text
	pDC->SelectObject(&m_font);

	CString strCaption;
	GetWindowText(strCaption);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));

	pDC->DrawText (strCaption, btnRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);


}

void CPatButton::DrawBackground(CDC *pDC)
{
	// Get the parents window area behind our control and copy it for our background.

	CClientDC dc(GetParent());
	CRect cltRect, wndRect;

	GetClientRect(&cltRect);
	GetWindowRect(&wndRect);
	GetParent()->ScreenToClient(&wndRect);

	::BitBlt(pDC->m_hDC, 0, 0, cltRect.Width(), cltRect.Height(), m_hBGdc, wndRect.left, wndRect.top, SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
// CPatButton implementation

