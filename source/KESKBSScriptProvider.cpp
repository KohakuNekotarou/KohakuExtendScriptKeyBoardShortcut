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
#include "IMenuUtils.h"
#include "IScript.h"
#include "IScriptRequestData.h"
#include "IWorkspace.h"

// General includes:
#include "CAlert.h"
#include "CScriptProvider.h"
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

	// This method is called if a provider can handle a property.
	// @param scriptID_prop identifies the ID of the property to handle.
	// @param iScriptRequestData identifies an interface pointer used to extract data.
	// @param iScript_parent identifies an interface pointer on the script object representing the parent of the application object.
	virtual ErrorCode AccessProperty(ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent);

private:
	virtual ErrorCode GetBeforeTranslationActionNameOrArea(
		ScriptID scriptID_prop, IScriptRequestData* iScriptRequestData, IScript* iScript_parent, PMString pMString_target);
};

// CREATE_PMINTERFACE
// Binds the C++ implementation class onto its ImplementationID making the C++ code callable by the application.
CREATE_PMINTERFACE(KESKBSScriptProvider, kKESKBSScriptProviderImpl)

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