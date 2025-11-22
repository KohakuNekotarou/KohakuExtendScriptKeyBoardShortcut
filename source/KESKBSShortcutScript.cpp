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

// General includes:
#include "CProxyScript.h"

// Project includes:
#include "KESSScriptingDefs.h"
#include "KESSID.h"

class KESSShortcutScript : public CProxyScript
{
public:
	KESSShortcutScript(IPMUnknown* boss);
};

// Make the implementation available to the application.
CREATE_PMINTERFACE(KESSShortcutScript, kKESSShortcutScriptImpl)

// Constructor
KESSShortcutScript::KESSShortcutScript(IPMUnknown* boss) : CProxyScript(boss)
{
	// NOTE: this ScriptID must match that used in the call to IScriptUtils::CreateProxyScriptObject
	fObjectType = c_KESSShortcut;
}
