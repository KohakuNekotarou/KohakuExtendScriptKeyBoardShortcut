//========================================================================================
//  
//  $File$
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author$
//  
//  $DateTime$
//  
//  $Revision$
//  
//  $Change$
//  
//  Copyright 1997-2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// interface includes
#include "IApplication.h"
#include "IDialog.h"
#include "IDialogMgr.h"
#include "IIdleTaskMgr.h"
#include "IWindow.h"

// General includes:
#include "CIdleTask.h"
#include "CAlert.h" // CAlert::InformationAlert(Msg);

// Project includes
#include "KESKBSID.h"

class KESKBSIdleTask : public CIdleTask
{
public:

	// Constructor
	KESKBSIdleTask(IPMUnknown* boss);

	// RunTask
	virtual uint32 RunTask(uint32 appFlags, IdleTimer* timeCheck);
};

// CREATE_PMINTERFACE
// Binds the C++ implementation class onto its ImplementationID 
// making the C++ code callable by the application.
CREATE_PMINTERFACE(KESKBSIdleTask, kKESKBSIdleTaskImpl)

// Constructor
KESKBSIdleTask::KESKBSIdleTask(IPMUnknown *boss)
	:CIdleTask(boss)
{
}

// RunTask
uint32 KESKBSIdleTask::RunTask(uint32 appFlags, IdleTimer* timeCheck)
{
	if( appFlags == IIdleTaskMgr::kModalDialogUp)
	{
		do
		{
			InterfacePtr<IApplication> iApplication(::GetExecutionContextSession()->QueryApplication());
			if (iApplication == nil) break;

			InterfacePtr<IDialogMgr> iDialogMgr(iApplication, ::UseDefaultIID());
			if (iDialogMgr == nil) break;

			IWindow* iWindow = iDialogMgr->GetFrontmostDialogWindow();
			if (iWindow == nil) break;

			InterfacePtr<IDialog> iDialog(iWindow, ::UseDefaultIID());
			if (iDialog == nil) break;

			iDialog->PressDefaultButton();

			// Removes the task from its queues
			return IIdleTask::kEndOfTime;

		} while (false);
	}
	return 0;
}