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
#include "KESKBSID.h"
#include "KESKBSScriptingDefs.h"

// RepresentScriptProvider
class KESKBSKeyBoardShortcutScriptProvider : public RepresentScriptProvider
{
public:
	// Constructor
	KESKBSKeyBoardShortcutScriptProvider(IPMUnknown* boss);

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
	// AddKeyBoardShortcut
	ErrorCode AddKeyBoardShortcut(ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript);

	// RemoveKeyBoardShortcut
	ErrorCode RemoveKeyBoardShortcut(ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript);

	// GetSetContextStrOrShortcutStr
	ErrorCode GetSetContextStringOrShortcutString(
		ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript, PMString pMString_target);
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESKBSKeyBoardShortcutScriptProvider, kKESKBSKeyBoardShortcutScriptProviderImpl)

// Constructor
KESKBSKeyBoardShortcutScriptProvider::KESKBSKeyBoardShortcutScriptProvider(IPMUnknown* boss) : RepresentScriptProvider(boss){}

// HandleMethod
ErrorCode KESKBSKeyBoardShortcutScriptProvider::HandleMethod(
	ScriptID scriptID, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID.Get())
	{
	case e_KESKBSAdd:
	{
		result = this->AddKeyBoardShortcut(scriptID, iScriptRequestData, iScript);
		break;
	}
	case e_KESKBSRemove:
	{
		result = this->RemoveKeyBoardShortcut(scriptID, iScriptRequestData, iScript);
		break;
	}
	default:
		return RepresentScriptProvider::HandleMethod(scriptID, iScriptRequestData, iScript);
	}

	return result;
}

// AccessProperty
ErrorCode KESKBSKeyBoardShortcutScriptProvider::AccessProperty(
	ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	switch (scriptID_prop.Get())
	{
	case p_KESKBSKeyBoardShortcutContextString:
	{
		result = this->GetSetContextStringOrShortcutString(scriptID_prop, iScriptRequestData, iScript, "ContextString");
		break;
	}
	case p_KESKBSKeyBoardShortcutString:
	{
		result = this->GetSetContextStringOrShortcutString(scriptID_prop, iScriptRequestData, iScript, "ShortcutString");
		break;
	}
	default:
		return RepresentScriptProvider::AccessProperty(scriptID_prop, iScriptRequestData, iScript);
	}

	return result;
}

// GetNumObjects
int32 KESKBSKeyBoardShortcutScriptProvider::GetNumObjects(const IScriptRequestData* iScriptRequestData, IScript* iScript)
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

// AppendNthObject
ErrorCode KESKBSKeyBoardShortcutScriptProvider::AppendNthObject(const IScriptRequestData* iScriptRequestData, IScript* iScript, int32 int32_n, ScriptList& scriptList)
{
	ErrorCode result = kFailure;

	do
	{
		// check input parameters
		if (iScriptRequestData == nil || iScript == nil) break;

		// create a proxy script object
		InterfacePtr<IScript> iScript_proxyScriptObject(Utils<IScriptUtils>()->CreateProxyScriptObject(
			iScriptRequestData->GetRequestContext(),
			kKESKBSKeyBoardShortcutScriptObjectBoss,
			c_KESKBSKeyBoardShortcut,
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

// AddKeyBoardShortcut
ErrorCode KESKBSKeyBoardShortcutScriptProvider::AddKeyBoardShortcut(
	ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Query shortcut context.
		ScriptData scriptData;
		PMString pMString_shortcutContext;
		if (iScriptRequestData->ExtractRequestData(p_KESKBSKeyBoardShortcutContextString, scriptData) == kFailure) break;

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
				SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESKBSKeyBoardShortcutContextString);
		}

		// ---------------------------------------------------------------------------------------
		// Query shortcut string.
		PMString pMString_shortcutString;
		if (iScriptRequestData->ExtractRequestData(p_KESKBSKeyBoardShortcutString, scriptData) != kSuccess) break;

		if (scriptData.GetPMString(pMString_shortcutString) != kSuccess) break;

		// ---------------------------------------------------------------------------------------
		// Parse shortcut string.
		VirtualKey virtualKey_keyOut;
		int16 int16_modsOut;
		Utils<IShortcutUtils>()->ParseShortcutString(pMString_shortcutString, &virtualKey_keyOut, &int16_modsOut);

		if (virtualKey_keyOut == kVirtualNullKey)
		{
			return Utils<IScriptErrorUtils>()->
				SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESKBSKeyBoardShortcutString);
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

		result = kSuccess;

	} while (false);

	return result;
}

// RemoveKeyBoardShortcut
ErrorCode KESKBSKeyBoardShortcutScriptProvider::RemoveKeyBoardShortcut(
	ScriptID scriptID_method, IScriptRequestData* iScriptRequestData, IScript* iScript)
{
	ErrorCode result = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Get actionID.
		InterfacePtr<IScript> iScript_parent((IScript*)iScript->QueryParent(
			IScript::kDefaultIID, iScriptRequestData->GetRequestContext()));
		if (iScript_parent == nil) break;

		ScriptObject scriptObject = iScript_parent->GetScriptObject(iScriptRequestData->GetRequestContext());

		ScriptData scriptData = scriptObject.specifierData;

		int32 int32_actionID;
		scriptData.GetInt32(&int32_actionID);

		// ---------------------------------------------------------------------------------------
		// Get index
		InterfacePtr<IIntData> iIntData(iScript, ::UseDefaultIID());
		if (iIntData == nil) break;

		int32 int32_index = iIntData->Get();

		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// Get shortcuts
		int32 int32_num = iShortcutManager->GetNumShortcutsForAction(int32_actionID);

		std::vector<PMString> vector_contextStrOut;
		std::vector<VirtualKey> vector_keyOut;
		std::vector<int16> vector_modsOut;
		for (int32 i = 0; i < int32_num; i++)
		{
			if (i == int32_index) continue;

			// Retrieve information about the nth shortcut associated with a particular action.
			PMString pMString_contextStrOut;
			VirtualKey virtualKey_keyOut;
			int16 int16_modsOut;
			iShortcutManager->GetNthShortcutForAction(
				int32_actionID,
				i,
				&pMString_contextStrOut,
				&virtualKey_keyOut,
				&int16_modsOut
			);
			vector_contextStrOut.emplace_back(pMString_contextStrOut);
			vector_keyOut.emplace_back(virtualKey_keyOut);
			vector_modsOut.emplace_back(int16_modsOut);
		}

		// ---------------------------------------------------------------------------------------
		// RemoveAllShortcutsForAction
		iShortcutManager->RemoveAllShortcutsForAction(int32_actionID);

		// ---------------------------------------------------------------------------------------
		// Add shortcut.
		for (int32 i = 0; i < vector_contextStrOut.size(); i++)
		{
			iShortcutManager->AddShortcut(int32_actionID, vector_contextStrOut[i], vector_keyOut[i], vector_modsOut[i]);
		}

		result = kSuccess;

	} while (false); // only do once.

	return result;
}

// GetSetContextStrOrShortcutString
ErrorCode KESKBSKeyBoardShortcutScriptProvider::GetSetContextStringOrShortcutString(
	ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript, PMString pMString_target)
{
	ErrorCode result = kFailure;

	do
	{
		// ---------------------------------------------------------------------------------------
		// Get actionID.
		InterfacePtr<IScript> iScript_parent((IScript*)iScript->QueryParent(
			IScript::kDefaultIID, iScriptRequestData->GetRequestContext()));
		if (iScript_parent == nil) break;

		ScriptObject scriptObject = iScript_parent->GetScriptObject(iScriptRequestData->GetRequestContext());

		ScriptData scriptData = scriptObject.specifierData;

		int32 int32_actionID;
		scriptData.GetInt32(&int32_actionID);

		// ---------------------------------------------------------------------------------------
		// Get index
		InterfacePtr<IIntData> iIntData(iScript, ::UseDefaultIID());
		if (iIntData == nil) break;

		int32 int32_index = iIntData->Get();

		// ---------------------------------------------------------------------------------------
		// Query IShortcutManager.
		InterfacePtr<IApplication> iApplication(GetExecutionContextSession()->QueryApplication());
		if (iApplication == nil) break;

		InterfacePtr<IActionManager> iActionManager(iApplication->QueryActionManager());
		if (iActionManager == nil) break;

		InterfacePtr<IShortcutManager> iShortcutManager(iActionManager, ::UseDefaultIID());
		if (iShortcutManager == nil) break;

		// ---------------------------------------------------------------------------------------
		// Get shortcuts
		int32 int32_num = iShortcutManager->GetNumShortcutsForAction(int32_actionID);

		std::vector<PMString> vector_contextStrOut;
		std::vector<VirtualKey> vector_keyOut;
		std::vector<int16> vector_modsOut;
		for (int32 i = 0; i < int32_num; i++)
		{
			// Retrieve information about the nth shortcut associated with a particular action.
			PMString pMString_contextStrOut;
			VirtualKey virtualKey_keyOut;
			int16 int16_modsOut;
			iShortcutManager->GetNthShortcutForAction(
				int32_actionID,
				i,
				&pMString_contextStrOut,
				&virtualKey_keyOut,
				&int16_modsOut
			);
			vector_contextStrOut.emplace_back(pMString_contextStrOut);
			vector_keyOut.emplace_back(virtualKey_keyOut);
			vector_modsOut.emplace_back(int16_modsOut);
		}

		// Processing request data
		PMString pMString_shortcut;
		if (iScriptRequestData->IsPropertyGet()) // Get
		{
			// Append return data
			if (pMString_target == "ContextString")
			{
				iScriptRequestData->AppendReturnData(iScript, scriptID_prop, ScriptData(vector_contextStrOut[int32_index]));
			}
			else if (pMString_target == "ShortcutString")
			{
				pMString_shortcut = Utils<IShortcutUtils>()->GetShortcutString(
					vector_keyOut[int32_index], vector_modsOut[int32_index]);

				iScriptRequestData->AppendReturnData(iScript, scriptID_prop, ScriptData(pMString_shortcut));
			}
		}
		else if (iScriptRequestData->IsPropertyPut()) // Set
		{
			// ---------------------------------------------------------------------------------------
			// Extract request data
			result = iScriptRequestData->ExtractRequestData(scriptID_prop.Get(), scriptData);
			if (result != kSuccess) break;

			result = scriptData.GetPMString(pMString_shortcut);
			if (result != kSuccess) break;

			if (pMString_target == "ContextString")
			{
				// validate.
				if (
					pMString_shortcut != "DialogContext" &&
					pMString_shortcut != "DefaultContext" &&
					pMString_shortcut != "TableContext" &&
					pMString_shortcut != "TableObjectContext" &&
					pMString_shortcut != "KBSCContext_XMLStructureContext" &&
					pMString_shortcut != "FullScreenContext" &&
					pMString_shortcut != "TextContext"
					)
				{
					return Utils<IScriptErrorUtils>()->
						SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESKBSKeyBoardShortcutContextString);
				}

				// ---------------------------------------------------------------------------------------
				// Translation
				IShortcutContext* iShortcutContext = iShortcutManager->QueryShortcutContextByName(pMString_shortcut);
				if (iShortcutContext == nil) break;

				PMString pMString_shortcutContextLocalString = iShortcutContext->GetShortcutContextString();

				// ---------------------------------------------------------------------------------------
				// Set
				vector_contextStrOut[int32_index] = pMString_shortcutContextLocalString;
			}
			else if (pMString_target == "ShortcutString")
			{
				if (pMString_shortcut != "") {
					// ---------------------------------------------------------------------------------------
					// Parse shortcut string.
					VirtualKey virtualKey_keyOut;
					int16 int16_modsOut;
					Utils<IShortcutUtils>()->ParseShortcutString(pMString_shortcut, &virtualKey_keyOut, &int16_modsOut);

					if (virtualKey_keyOut == kVirtualNullKey)
					{
						return Utils<IScriptErrorUtils>()->
							SetMissingRequiredParameterErrorData(iScriptRequestData, p_KESKBSKeyBoardShortcutString);
					}

					// ---------------------------------------------------------------------------------------
					// Set
					vector_keyOut[int32_index] = virtualKey_keyOut;
					vector_modsOut[int32_index] = int16_modsOut;
				}
				else
				{
					// Remove
					vector_contextStrOut.erase(vector_contextStrOut.begin() + int32_index);
					vector_keyOut.erase(vector_keyOut.begin() + int32_index);
					vector_modsOut.erase(vector_modsOut.begin() + int32_index);
				}
			}

			// ---------------------------------------------------------------------------------------
			// RemoveAllShortcutsForAction
			iShortcutManager->RemoveAllShortcutsForAction(int32_actionID);

			// ---------------------------------------------------------------------------------------
			// Add shortcut.
			for (int32 i = 0; i < vector_contextStrOut.size(); i++)
			{
				iShortcutManager->AddShortcut(
					int32_actionID, vector_contextStrOut[i], vector_keyOut[i], vector_modsOut[i]);
			}
		}
		result = kSuccess;

	} while (false); // only do once.

	return result;
}