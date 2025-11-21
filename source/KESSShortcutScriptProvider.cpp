#include "VCPlugInHeaders.h"

// Implementation includes:
#include "ScriptData.h"
#include "CScriptProvider.h"

// Interface includes:
#include "IActionManager.h"
#include "IApplication.h"
#include "IIntData.h"
#include "IScript.h"
#include "IScriptErrorUtils.h"
#include "IScriptRequestData.h"
#include "IShortcutContext.h"
#include "IShortcutManager.h"
#include "IShortcutUtils.h"



#include "LocaleSetting.h"



// General includes:
#include "CAlert.h" // CAlert::InformationAlert(Msg);
#include "keyboarddefs.h" // for kVirtualNullKey.

// Project includes:
#include "KESSID.h"
#include "KESSScriptingDefs.h"

// RepresentScriptProvider
class KESSShortcutScriptProvider : public RepresentScriptProvider
{
public:
	KESSShortcutScriptProvider(IPMUnknown* boss);

	virtual ErrorCode HandleMethod(ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript);

protected:
	// Get the number of child objects in a collection on the given parent.
	virtual int32 GetNumObjects(const IScriptRequestData* iScriptRequestData, IScript* iScript);

	// Append the nth child object from a collection on the given parent.
	virtual ErrorCode AppendNthObject(const IScriptRequestData* iScriptRequestData, IScript* iScript, int32 int32_n, ScriptList& scriptList);

private:
	// Add shortcut.
	ErrorCode AddShortcut(ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript);
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESSShortcutScriptProvider, kKESSShortcutScriptProviderImpl)

KESSShortcutScriptProvider::KESSShortcutScriptProvider(IPMUnknown* boss) : RepresentScriptProvider(boss){}

ErrorCode KESSShortcutScriptProvider::HandleMethod(ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID.Get())
	{
	case e_KESSAddShortcut: // Add shortcut.
	{
		result = this->AddShortcut(scriptID, iScriptRequestData, iScript);
		break;
	}
	default:
		return RepresentScriptProvider::HandleMethod(scriptID, iScriptRequestData, iScript);
	}

	return result;
}

int32 KESSShortcutScriptProvider::GetNumObjects(const IScriptRequestData* iScriptRequestData, IScript* iScript)
{

	int32 int32_numObjects = 0;
	do
	{
		// ---------------------------------------------------------------------------------------
		// Get actionID.
		ScriptObject scriptObject = iScript->GetScriptObject(iScriptRequestData->GetRequestContext());

		ScriptData scriptData = scriptObject.specifierData;

		int32 int32_id;
		scriptData.GetInt32(&int32_id);

		// ---------------------------------------------------------------------------------------
		// Get num shortcut for action.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		int32_numObjects = iShortcutManager->GetNumShortcutsForAction(int32_id);
	} while (false);

	return int32_numObjects;
}

ErrorCode KESSShortcutScriptProvider::AppendNthObject(const IScriptRequestData* iScriptRequestData, IScript* iScript, int32 int32_n, ScriptList& scriptList)
{
	ErrorCode result = kFailure;

	do
	{
		// check input parameters
		if (iScriptRequestData == nil || iScript == nil) break;

		// create a proxy script object
		InterfacePtr<IScript> iScript_proxyScriptObject(Utils<IScriptUtils>()->CreateProxyScriptObject(
			iScriptRequestData->GetRequestContext(),
			kKESSShortcutScriptObjectBoss,
			c_KESSShortcut,
			iScript
		));
		if (iScript == nil) break;

		// query for data interfaces
		InterfacePtr<IIntData> iIntData(iScript_proxyScriptObject, ::UseDefaultIID());
		if (iIntData == nil) break;

		// set the name
		iIntData->Set(int32_n);

		// add the object to the list
		scriptList.push_back(iScript_proxyScriptObject);

		result = kSuccess;

	} while (false);

	return result;
}

// Add shortcut.
ErrorCode KESSShortcutScriptProvider::AddShortcut(
	ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Query shortcut context.
		ScriptData scriptData;
		PMString pMString_shortcutContext;
		if (iScriptRequestData->ExtractRequestData(p_KESSShortcutContextString, scriptData) == kFailure) break;

		if (scriptData.GetPMString(pMString_shortcutContext) == kFailure) break;

		// validate.
		if (
			pMString_shortcutContext != "DialogContext" &&
			pMString_shortcutContext != "DefaultContext" &&
			pMString_shortcutContext != "TableContext" &&
			pMString_shortcutContext != "TableObjectContext" &&
			pMString_shortcutContext != "KBSCContext_XMLStructureContext" &&
			pMString_shortcutContext != "FullScreenContext" &&
			pMString_shortcutContext != "TextContext"
			)
		{
			return Utils<IScriptErrorUtils>()->
				SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESSShortcutContextString);
		}

		// ---------------------------------------------------------------------------------------
		// Query shortcut string.
		PMString pMString_shortcutString;
		if (iScriptRequestData->ExtractRequestData(p_KESSShortcutString, scriptData) != kSuccess) break;

		if (scriptData.GetPMString(pMString_shortcutString) != kSuccess) break;

		// ---------------------------------------------------------------------------------------
		// Parse shortcut string.
		VirtualKey virtualKey_keyOut;
		int16 int16_modsOut;
		Utils<IShortcutUtils>()->ParseShortcutString(pMString_shortcutString, &virtualKey_keyOut, &int16_modsOut);

		if (virtualKey_keyOut == kVirtualNullKey)
		{
			return Utils<IScriptErrorUtils>()->
				SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESSShortcutString);
		}

		// ---------------------------------------------------------------------------------------
		// Get actionID.
		ScriptObject scriptObject = iScript->GetScriptObject(iScriptRequestData->GetRequestContext());

		scriptData = scriptObject.specifierData;

		int32 int32_actionID;
		scriptData.GetInt32(&int32_actionID);

		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(::GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// Translation
		IShortcutContext* iShortcutContext = iShortcutManager->QueryShortcutContextByName(pMString_shortcutContext);
		if (iShortcutContext == nil) break;

		PMString pMString_shortcutContextLocalString = iShortcutContext->GetShortcutContextString();

		// Add shortcut.
		iShortcutManager->AddShortcut(int32_actionID, pMString_shortcutContextLocalString, virtualKey_keyOut, int16_modsOut);

		// Save current shortcut.
		//result = iShortcutManager->SaveCurrentShortcutSetFile();

		result = kSuccess;

	} while (false);

	return result;
}