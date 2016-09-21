#pragma once


// CPatStatic

class CPatStatic : public CStatic
{
	DECLARE_DYNAMIC(CPatStatic)

public:
	CPatStatic();
	virtual ~CPatStatic();

	// Operations
public:
	void SetImageResourceIDs(int enabledID, int disabledID = -1);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	HDC m_hBGdc;		// Background which is a copy of the parents window behind the button.
	HBITMAP m_hBGbmp;	// The DC for the background.
	int m_enabledID;		// PNG ID for Normal state
	int m_disabledID;	// PNG ID for Disabled state

						// Generated message map functions
protected:
	//{{AFX_MSG(CPatButton)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DrawBackground(CDC *pDC);
};


