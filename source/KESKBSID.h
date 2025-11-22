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


#ifndef __KESSID_h__
#define __KESSID_h__

#include "SDKDef.h"

// Company:
#define kKESSCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kKESSCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kKESSPluginName	"KohakuExtendScriptShortcut"			// Name of this plug-in.
#define kKESSPrefixNumber	0xe204606 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kKESSVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kKESSAuthor		""					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kKESSPrefixNumber above to modify the prefix.)
#define kKESSPrefix		RezLong(kKESSPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kKESSStringPrefix	SDK_DEF_STRINGIZE(kKESSPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kKESSMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kKESSMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kKESSPluginID, kKESSPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kKESSShortcutScriptProviderBoss, kKESSPrefix + 0)
DECLARE_PMID(kClassIDSpace, kKESSShortcutScriptObjectBoss, kKESSPrefix + 1)

// InterfaceIDs:
//DECLARE_PMID(kInterfaceIDSpace, IID_IKESSINTERFACE, kKESSPrefix + 0)

// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kKESSShortcutScriptProviderImpl, kKESSPrefix + 0)
DECLARE_PMID(kImplementationIDSpace, kKESSShortcutScriptImpl, kKESSPrefix + 1)

// ActionIDs:
DECLARE_PMID(kActionIDSpace, kKESSAboutActionID, kKESSPrefix + 0)

// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kKESSWidgetID, kKESSPrefix + 2)

//Script Element IDs
DECLARE_PMID(kScriptInfoIDSpace, kKESSShortcutObjectScriptElement, kKESSPrefix + 0)
DECLARE_PMID(kScriptInfoIDSpace, kKESSAddShortcutMethodScriptElement, kKESSPrefix + 1)

// GUIDs for script objects:
// Visual Studio : Tools > Generate GUID
#define kKESSShortcutObject_CLSID { 0xf9a6278e, 0xc3c, 0x4345, { 0xb1, 0xc2, 0x25, 0x37, 0x61, 0x99, 0x39, 0x31 } }
#define kKESShortcutsObject_CLSID { 0xcf8c4b1c, 0x78e5, 0x4072, { 0x96, 0x48, 0x1c, 0x02, 0x45, 0xe2, 0xfa, 0xde } }

// "About Plug-ins" sub-menu:
#define kKESSAboutMenuKey			kKESSStringPrefix "kKESSAboutMenuKey"
#define kKESSAboutMenuPath		kSDKDefStandardAboutMenuPath kKESSCompanyKey

// "Plug-ins" sub-menu:
#define kKESSPluginsMenuKey 		kKESSStringPrefix "kKESSPluginsMenuKey"
#define kKESSPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kKESSCompanyKey kSDKDefDelimitMenuPath kKESSPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kKESSAboutBoxStringKey	kKESSStringPrefix "kKESSAboutBoxStringKey"
#define kKESSTargetMenuPath kKESSPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kKESSFirstMajorFormatNumber  RezLong(1)
#define kKESSFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kKESSCurrentMajorFormatNumber kKESSFirstMajorFormatNumber
#define kKESSCurrentMinorFormatNumber kKESSFirstMinorFormatNumber

#endif // __KESSID_h__
