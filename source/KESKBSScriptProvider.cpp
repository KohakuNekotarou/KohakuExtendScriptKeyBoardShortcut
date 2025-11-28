//========================================================================================
//  
//  $File: $
//  
//  Owner: 
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2012 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IActionManager.h"
#include "IApplication.h"
#include "IIdleTask.h"
#include "IIdleTaskMgr.h"
#include "IKBSCSetsManager.h"
#include "IMenuUtils.h"
#include "IScript.h"
#include "IScriptRequestData.h"
#include "IShortcutManager.h"
#include "IWorkspace.h"

// General includes:
#include "CAlert.h" // CAlert::InformationAlert(Msg);
#include "CScriptProvider.h"
#include "KBSCEditorDlgID.h" // for kKBSCEditorDlgActionID
#include "PMLocaleId.h"
#include "Utils.h"

// Project includes:
#include "KESKBSScriptingDefs.h"
#include "KESKBSID.h"

class KESKBSScriptProvider : public CScriptProvider
{
public:
	// Constructor.
	KESKBSScriptProvider(IPMUnknown* boss) : CScriptProvider(boss) {};

	// HandleMethod
	virtual ErrorCode HandleMethod(ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript);

	// AccessProperty
	virtual ErrorCode AccessProperty(ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent);

private:
	// GetBeforeTranslationActionNameOrArea
	ErrorCode GetBeforeTranslationActionNameOrArea(
		ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent, PMString pMString_target);

	// IsUserShortcutKBSCArea
	ErrorCode IsUserShortcutKBSCArea(
		ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript);

	// RemoveAllShortcutsForAction
	ErrorCode RemoveAllShortcutsForAction(IScriptRequestData* iScriptRequestData, IScript* iScript);

	// SaveCurrentShortcutSetFile
	ErrorCode SaveCurrentShortcutSetFile();

	// IsOpenedShortcutDialog
	ErrorCode IsOpenedShortcutDialog();

	// GetNumSets
	ErrorCode GetNumSets();

	// Flag indicating whether the shortcut dialog was opened.
	static bool16 openedShortcutDialogFlg;
};

// CREATE_PMINTERFACE
// Binds the C++ implementation class onto its ImplementationID making the C++ code callable by the application.
CREATE_PMINTERFACE(KESKBSScriptProvider, kKESKBSScriptProviderImpl)

bool16 KESKBSScriptProvider::openedShortcutDialogFlg = kFalse;

// HandleMethod
ErrorCode KESKBSScriptProvider::HandleMethod(ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID.Get())
	{
	case e_KESKBSIsUserShortcutKBSCArea:
		result = this->IsUserShortcutKBSCArea(scriptID, iScriptRequestData, iScript);
		break;

	case e_KESKBSRemoveAllShortcutsForAction:
		result = this->RemoveAllShortcutsForAction(iScriptRequestData, iScript);
		break;

	case e_KESKBSSaveCurrentShortcutSetFile:
		result = this->SaveCurrentShortcutSetFile();
		break;

	case e_KESKBSGetNumSets:
		result = this->GetNumSets();
		break;

	default:
		return CScriptProvider::HandleMethod(scriptID, iScriptRequestData, iScript);
	}

	return result;
}

// AccessProperty
ErrorCode KESKBSScriptProvider::AccessProperty(
	ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent)
{
	ErrorCode status = kFailure;

	switch (scriptID_prop.Get())
	{
	case p_KESKBSBeforeTranslationActionName:
		status = this->GetBeforeTranslationActionNameOrArea(scriptID_prop, iScriptRequestData, iScript_parent, "Name");
		break;

	case p_KESKBSBeforeTranslationActionArea:
		status = this->GetBeforeTranslationActionNameOrArea(scriptID_prop, iScriptRequestData, iScript_parent, "Area");
		break;

    default:
		status = CScriptProvider::AccessProperty(scriptID_prop, iScriptRequestData, iScript_parent);
    }

    return status;
}

// GetBeforeTranslationArea
ErrorCode KESKBSScriptProvider::GetBeforeTranslationActionNameOrArea(
	ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent, PMString pMString_target)
{
	ErrorCode status = kFailure;

	do
	{
		if (iScriptRequestData->IsPropertyGet())
		{
			// ---------------------------------------------------------------------------------------
			// Query IActionManager.
			InterfacePtr<IApplication> iApplication(::GetExecutionContextSession()->QueryApplication());
			if (iApplication == nil) break;

			InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
			if (iActionManager == nil) break;

			// ---------------------------------------------------------------------------------------
			// Get actionID.
			ScriptObject scriptObject = iScript_parent->GetScriptObject(iScriptRequestData->GetRequestContext());

			ScriptData scriptData = scriptObject.specifierData;

			int32 int32_actionID;
			scriptData.GetInt32(&int32_actionID);

			// ---------------------------------------------------------------------------------------
			// Get action name or area.
			PMString pMString;

			if (pMString_target == "Name")
			{
				pMString = iActionManager->GetActionName(int32_actionID);
			}
			else if (pMString_target == "Area")
			{
				pMString = iActionManager->GetActionArea(int32_actionID);
			}
			
			// Don't translate
			pMString.SetTranslatable(kFalse);

			// ---------------------------------------------------------------------------------------
			// Append return data
			iScriptRequestData->AppendReturnData(iScript_parent, scriptID_prop, ScriptData(pMString));

			status = kSuccess;
		}
	} while (false);

	return status;
}

// IsUserShortcutKBSCArea
ErrorCode KESKBSScriptProvider::IsUserShortcutKBSCArea(
	ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode status = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// GetShortcutKBSCArea
		IKBSCSetsManager::KBSCArea kBSCArea = iShortcutManager->GetShortcutKBSCArea();
		int32 int32_flg;
		if (kBSCArea == IKBSCSetsManager::kUserKBSCArea) int32_flg = 1;
		else int32_flg = 0;

		// ---------------------------------------------------------------------------------------
		// Append return data
		iScriptRequestData->AppendReturnData(iScript, scriptID, ScriptData(int32_flg));

		status = kSuccess;
	} while (false);

	return status;
}

// RemoveAllShortcutsForAction
ErrorCode KESKBSScriptProvider::RemoveAllShortcutsForAction(IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode status = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// Get actionID.
		ScriptObject scriptObject = iScript->GetScriptObject(iScriptRequestData->GetRequestContext());

		ScriptData scriptData = scriptObject.specifierData;

		int32 int32_actionID;
		scriptData.GetInt32(&int32_actionID);

		// ---------------------------------------------------------------------------------------
		// RemoveAllShortcutsForAction
		iShortcutManager->RemoveAllShortcutsForAction(int32_actionID);

		status = kSuccess;

	} while (false);

	return status;
}

// SaveCurrentShortcutSetFile
ErrorCode KESKBSScriptProvider::SaveCurrentShortcutSetFile()
{
	ErrorCode status = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// Save current shortcut set file.
		iShortcutManager->SaveCurrentShortcutSetFile();

		status = kSuccess;

	} while (false);

	return status;
}

// IsOpenedShortcutDialog
ErrorCode KESKBSScriptProvider::IsOpenedShortcutDialog()
{
	ErrorCode status = kFailure;

	do
	{
		// Have you ever opened the shortcut dialog.
		if (KESKBSScriptProvider::openedShortcutDialogFlg == kFalse)
		{
			// ---------------------------------------------------------------------------------------
			// InstallTask
			// Insert an idle task before opening modal dialog to prevent the program from stopping.
			InterfacePtr<IIdleTask> iIdleTask(::GetExecutionContextSession(), IID_IKESKBSIDLETASK);
			if (iIdleTask == nil) break;

			InterfacePtr<IIdleTaskMgr> idleTaskMgr(::GetExecutionContextSession(), ::UseDefaultIID());
			if (idleTaskMgr == nil) break;

			// If the task wasn't installed or it is currently running, returns IIdleTask::kEndOfTime.
			if (idleTaskMgr->RemoveTask(iIdleTask) == IIdleTask::kEndOfTime)
			{
				// AddTask
				idleTaskMgr->AddTask(iIdleTask, 0);
			}

			// ---------------------------------------------------------------------------------------
			// Open edit shortcut dialog
			InterfacePtr<IApplication> iApplication(::GetExecutionContextSession()->QueryApplication());
			if (iApplication == nil) break;

			InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
			if (iActionManager == nil) break;

			iActionManager->PerformAction(::GetExecutionContextSession()->GetActiveContext(), kKBSCEditorDlgActionID);

			// ---------------------------------------------------------------------------------------
			// RemoveTask
			// When modal dialog closes, the program resumes.
			idleTaskMgr->RemoveTask(iIdleTask);

			KESKBSScriptProvider::openedShortcutDialogFlg = kTrue;
		}

		status = kSuccess;

	} while (false);

	return status;
}

// GetNumSets
ErrorCode KESKBSScriptProvider::GetNumSets()
{
	ErrorCode status = kFailure;

	do
	{
		// IsOpenedShortcutDialog
		if (this->IsOpenedShortcutDialog()) break;

		status = kSuccess;

	} while (false);

	return status;
}