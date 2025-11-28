#include "VCPlugInHeaders.h"

// Implementation includes:
#include "ScriptData.h"
#include "CScriptProvider.h"

// Interface includes:
#include "IApplication.h"
#include "IScript.h"
#include "IScriptRequestData.h"

// General includes:
#include "CAlert.h" // CAlert::InformationAlert(Msg);
#include "keyboarddefs.h" // for kVirtualNullKey.

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
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESKBSKeyBoardShortcutSetScriptProvider, kKESKBSKeyBoardShortcutSetScriptProviderImpl)

// Constructor
KESKBSKeyBoardShortcutSetScriptProvider::KESKBSKeyBoardShortcutSetScriptProvider(IPMUnknown* boss) : RepresentScriptProvider(boss){}

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