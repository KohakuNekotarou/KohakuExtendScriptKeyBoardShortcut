#include "VCPlugInHeaders.h"

// Implementation includes:
#include "ScriptData.h"
#include "CScriptProvider.h"

// Interface includes:
#include "IApplication.h"
#include "IActionManager.h"
#include "IIdleTask.h"
#include "IIdleTaskMgr.h"
#include "IScript.h"
#include "IScriptRequestData.h"

// General includes:
#include "CAlert.h" // CAlert::InformationAlert(Msg);
#include "KBSCEditorDlgID.h" // for kKBSCEditorDlgActionID

// Project includes:
#include "KESKBSID.h"
#include "KESKBSScriptingDefs.h"

// RepresentScriptProvider
class KESKBSKeyBoardShortcutSetScriptProvider : public RepresentScriptProvider
{
public:
	// Constructor
	KESKBSKeyBoardShortcutSetScriptProvider(IPMUnknown* boss);

	// HandleMethod
	virtual ErrorCode HandleMethod(ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript);

	// AccessProperty
	virtual ErrorCode AccessProperty(ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript);

protected:
	// Get the number of child objects in a collection on the given parent.
	virtual int32 GetNumObjects(const IScriptRequestData* iScriptRequestData, IScript* iScript);

	// Append the nth child object from a collection on the given parent.
	virtual ErrorCode AppendNthObject(const IScriptRequestData* iScriptRequestData, IScript* iScript, int32 int32_n, ScriptList& scriptList);

private:
	// IsOpenedShortcutDialog
	void IsOpenedShortcutDialog();

	static bool16 openedShortcutDialogFlg;
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESKBSKeyBoardShortcutSetScriptProvider, kKESKBSKeyBoardShortcutSetScriptProviderImpl)

// openedShortcutDialogFlg
bool16 KESKBSKeyBoardShortcutSetScriptProvider::openedShortcutDialogFlg = kFalse;

// Constructor
KESKBSKeyBoardShortcutSetScriptProvider::KESKBSKeyBoardShortcutSetScriptProvider(IPMUnknown* boss)
	: RepresentScriptProvider(boss)
{
}

// HandleMethod
ErrorCode KESKBSKeyBoardShortcutSetScriptProvider::HandleMethod(
	ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID.Get())
	{
	case e_KESKBSAdd:
	{
		//result = this->AddKeyBoardShortcut(scriptID, iScriptRequestData, iScript);
		break;
	}
	default:
		return RepresentScriptProvider::HandleMethod(scriptID, iScriptRequestData, iScript);
	}

	return result;
}

// AccessProperty
ErrorCode KESKBSKeyBoardShortcutSetScriptProvider::AccessProperty(
	ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID_prop.Get())
	{
	case p_KESKBSKeyBoardShortcutContextString:
	{
		// result = this->GetSetContextStringOrShortcutString(scriptID_prop, iScriptRequestData, iScript, "ContextString");
		break;
	}
	default:
		return RepresentScriptProvider::AccessProperty(scriptID_prop, iScriptRequestData, iScript);
	}

	return result;
}

// GetNumObjects
int32 KESKBSKeyBoardShortcutSetScriptProvider::GetNumObjects(const IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	int32 int32_numObjects = 0;
	do
	{
		








	} while (false);

	return int32_numObjects;
}

// AppendNthObject
ErrorCode KESKBSKeyBoardShortcutSetScriptProvider::AppendNthObject(const IScriptRequestData* iScriptRequestData, IScript* iScript, int32 int32_n, ScriptList& scriptList)
{
	ErrorCode result = kFailure;

	do
	{
		

		result = kSuccess;

	} while (false);

	return result;
}

// IsOpenedShortcutDialog
void KESKBSKeyBoardShortcutSetScriptProvider::IsOpenedShortcutDialog()
{
	do
	{
		// Have you ever opened the shortcut dialog.
		if (KESKBSKeyBoardShortcutSetScriptProvider::openedShortcutDialogFlg == kFalse)
		{
			// ---------------------------------------------------------------------------------------
			// InstallTask
			// Insert an idle task before opening modal dialog to prevent the program from stopping.
			InterfacePtr<IIdleTask> iIdleTask(::GetExecutionContextSession(), IID_IKESKBSIDLETASK);
			if (iIdleTask == nil) break;

			InterfacePtr<IIdleTaskMgr> idleTaskMgr(::GetExecutionContextSession(), ::UseDefaultIID());
			if (idleTaskMgr == nil) break;

			// ---------------------------------------------------------------------------------------
			// RemoveTask
			idleTaskMgr->RemoveTask(iIdleTask);

			// ---------------------------------------------------------------------------------------
			// AddTask
			idleTaskMgr->AddTask(iIdleTask, 0);

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

			KESKBSKeyBoardShortcutSetScriptProvider::openedShortcutDialogFlg = kTrue;
		}
	} while (false);
}