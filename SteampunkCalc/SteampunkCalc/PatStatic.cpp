// PatStatic.cpp : implementation file
//   Extends CStatic class. Displays transparent PNG files using GDI+ in the static control.

#include "stdafx.h"
#include "SteampunkCalc.h"
#include "PatStatic.h"
#include "CGdiPlusBitmap.h"

// CPatStatic

IMPLEMENT_DYNAMIC(CPatStatic, CStatic)

CPatStatic::CPatStatic() :
m_enabledID(-1),
m_disabledID(-1)
{
	// Setup a bitmap and DC to put background in (easy transparency)
	m_hBGbmp = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BUTTON_NORMAL));
	m_hBGdc = ::CreateCompatibleDC(NULL);
	SelectObject(m_hBGdc, m_hBGbmp);
}

CPatStatic::~CPatStatic()
{
	::DeleteDC(m_hBGdc);
	::DeleteObject(m_hBGbmp);
}


BEGIN_MESSAGE_MAP(CPatStatic, CStatic)
END_MESSAGE_MAP()



// CPatStatic message handlers

void CPatStatic::PreSubclassWindow()
{

	ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW, SWP_FRAMECHANGED);
	
	// Determine what type of button this is
	CStatic::PreSubclassWindow();



}


void CPatStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC cdcMem;
	CBitmap bmp;

	CRect btnRect(lpDrawItemStruct->rcItem);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);


	DrawBackground(pDC);

	CGdiPlusBitmapResource bitmapResoure;

	// Draw the PNG for the right state.
	int resID = IsWindowEnabled() ? m_disabledID : m_enabledID;
	if (resID == -1) // perhaps no disabled id?
		resID = m_enabledID;

	if ((resID != -1) && bitmapResoure.Load(resID, _T("PNG")))
	{
		Gdiplus::Graphics grphx(lpDrawItemStruct->hDC);
		grphx.DrawImage(bitmapResoure.m_pBitmap, 0, 0, btnRect.right - btnRect.left, btnRect.bottom - btnRect.top);
	}
}


void CPatStatic::DrawBackground(CDC *pDC)
{
	// Get the parents window area behind our control and copy it for our background.
	CClientDC dc(GetParent());
	CRect cltRect, wndRect;

	GetClientRect(&cltRect);
	GetWindowRect(&wndRect);
	GetParent()->ScreenToClient(&wndRect);

	::BitBlt(pDC->m_hDC, 0, 0, cltRect.Width(), cltRect.Height(), m_hBGdc, wndRect.left, wndRect.top, SRCCOPY);
}

void CPatStatic::SetImageResourceIDs(int enabledID, int disabledID)
{
	m_enabledID = enabledID;
	m_disabledID = disabledID;
}

