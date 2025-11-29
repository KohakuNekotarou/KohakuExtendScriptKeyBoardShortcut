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
#include "IStringData.h"

// General includes:
#include "CProxyScript.h"

// Project includes:
#include "KESKBSScriptingDefs.h"
#include "KESKBSID.h"

class KESKBSKeyBoardShortcutSetScript : public CProxyScript
{
public:
	KESKBSKeyBoardShortcutSetScript(IPMUnknown* boss);

	// GetScriptObject
	virtual ScriptObject GetScriptObject(const RequestContext& requestContext) const;
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESKBSKeyBoardShortcutSetScript, kKESKBSKeyBoardShortcutSetScriptImpl)

// Constructor
KESKBSKeyBoardShortcutSetScript::KESKBSKeyBoardShortcutSetScript(IPMUnknown* boss) : CProxyScript(boss)
{
	// NOTE: this ScriptID must match that used in the call to IScriptUtils::CreateProxyScriptObject
	fObjectType = c_KESKBSKeyBoardShortcutSet;
}

// GetScriptObject
ScriptObject KESKBSKeyBoardShortcutSetScript::GetScriptObject(const RequestContext& requestContext) const
{
	// identify script object by name
	PMString pMString_setName = "";
	do
	{
		InterfacePtr<IStringData> iStringData(this, ::UseDefaultIID());
		if (iStringData == nil) break;

		pMString_setName = iStringData->Get();
	} while (false);

	return ScriptObject(ScriptData(pMString_setName), this->GetObjectType(requestContext), kFormName);
}