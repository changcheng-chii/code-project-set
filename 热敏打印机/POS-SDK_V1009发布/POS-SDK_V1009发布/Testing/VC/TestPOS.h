// TestPOS.h : main header file for the TESTPOS application
//

#if !defined(AFX_TESTPOS_H__92D738EE_C6C9_4D4C_909D_9A09EA67660C__INCLUDED_)
#define AFX_TESTPOS_H__92D738EE_C6C9_4D4C_909D_9A09EA67660C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestPOSApp:
// See TestPOS.cpp for the implementation of this class
//

class CTestPOSApp : public CWinApp
{
public:
	CTestPOSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPOSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestPOSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPOS_H__92D738EE_C6C9_4D4C_909D_9A09EA67660C__INCLUDED_)
