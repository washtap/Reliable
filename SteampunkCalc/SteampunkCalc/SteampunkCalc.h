
// SteampunkCalc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CSteampunkCalcApp:
// See SteampunkCalc.cpp for the implementation of this class
//

class CSteampunkCalcApp : public CWinApp
{
public:
	CSteampunkCalcApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR m_gdiplusToken;
public:
	virtual int ExitInstance();
};

extern CSteampunkCalcApp theApp;