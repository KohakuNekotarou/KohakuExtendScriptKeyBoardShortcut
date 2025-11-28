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


#ifndef __KESKBSID_h__
#define __KESKBSID_h__

#include "SDKDef.h"

// Company:
#define kKESKBSCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kKESKBSCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kKESKBSPluginName	"KohakuExtendScriptKeyBoardShortcut"			// Name of this plug-in.
#define kKESKBSPrefixNumber	0x204606 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kKESKBSVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kKESKBSAuthor		""					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kKESKBSPrefixNumber above to modify the prefix.)
#define kKESKBSPrefix		RezLong(kKESKBSPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kKESKBSStringPrefix	SDK_DEF_STRINGIZE(kKESKBSPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kKESKBSMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kKESKBSMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kKESKBSPluginID, kKESKBSPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kKESKBSScriptProviderBoss, kKESKBSPrefix + 0)
DECLARE_PMID(kClassIDSpace, kKESKBSKeyBoardShortcutScriptProviderBoss, kKESKBSPrefix + 1)
DECLARE_PMID(kClassIDSpace, kKESKBSKeyBoardShortcutScriptObjectBoss, kKESKBSPrefix + 2)
DECLARE_PMID(kClassIDSpace, kKESKBSKeyBoardShortcutSetScriptObjectBoss, kKESKBSPrefix + 3)

// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IKESKBSIDLETASK, kKESKBSPrefix + 0)

// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kKESKBSScriptProviderImpl, kKESKBSPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kKESKBSKeyBoardShortcutScriptImpl, kKESKBSPrefix + 1)
DECLARE_PMID(kImplementationIDSpace, kKESKBSKeyBoardShortcutScriptProviderImpl, kKESKBSPrefix + 2)
DECLARE_PMID(kImplementationIDSpace, kKESKBSIdleTaskImpl, kKESKBSPrefix + 3)
DECLARE_PMID(kImplementationIDSpace, kKESKBSKeyBoardShortcutSetScriptImpl, kKESKBSPrefix + 4)
DECLARE_PMID(kImplementationIDSpace, kKESKBSKeyBoardShortcutSetScriptProviderImpl, kKESKBSPrefix + 5)

// ActionIDs:
DECLARE_PMID(kActionIDSpace, kKESKBSAboutActionID, kKESKBSPrefix + 0)

// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kKESKBSWidgetID, kKESKBSPrefix + 2)

//Script Element IDs
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSBeforeTranslationActionNamePropertyScriptElement, kKESKBSPrefix + 0)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSBeforeTranslationActionAreaPropertyScriptElement, kKESKBSPrefix + 1)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSKeyBoardShortcutObjectScriptElement, kKESKBSPrefix + 2)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSAddKeyBoardShortcutMethodScriptElement, kKESKBSPrefix + 3)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSRemoveKeyBoardShortcutMethodScriptElement, kKESKBSPrefix + 4)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSIsUserShortcutKBSCAreaMethodScriptElement, kKESKBSPrefix + 5)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSRemoveAllShortcutsForActionMethodScriptElement, kKESKBSPrefix + 6)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSSaveCurrentShortcutSetFileMethodScriptElement, kKESKBSPrefix + 7)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSKeyBoardShortcutContextStringPropertyScriptElement, kKESKBSPrefix + 8)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSKeyBoardShortcutStringPropertyScriptElement, kKESKBSPrefix + 9)
DECLARE_PMID(kScriptInfoIDSpace, kKESKBSGetNumSetsMethodScriptElement, kKESKBSPrefix + 10)

// GUIDs for script objects:
// Visual Studio : Tools > Generate GUID
#define kKESKBSKeyBoardShortcutObject_CLSID { 0xf9a6278e, 0xc3c, 0x4345, { 0xb1, 0xc2, 0x25, 0x37, 0x61, 0x99, 0x39, 0x31 } }
#define kKESKBSKeyBoardShortcutsObject_CLSID { 0xcf8c4b1c, 0x78e5, 0x4072, { 0x96, 0x48, 0x1c, 0x02, 0x45, 0xe2, 0xfa, 0xde } }

// "About Plug-ins" sub-menu:
#define kKESKBSAboutMenuKey			kKESKBSStringPrefix "kKESKBSAboutMenuKey"
#define kKESKBSAboutMenuPath		kSDKDefStandardAboutMenuPath kKESKBSCompanyKey

// "Plug-ins" sub-menu:
#define kKESKBSPluginsMenuKey 		kKESKBSStringPrefix "kKESKBSPluginsMenuKey"
#define kKESKBSPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kKESKBSCompanyKey kSDKDefDelimitMenuPath kKESKBSPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kKESKBSAboutBoxStringKey	kKESKBSStringPrefix "kKESKBSAboutBoxStringKey"
#define kKESKBSTargetMenuPath kKESKBSPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kKESKBSFirstMajorFormatNumber  RezLong(1)
#define kKESKBSFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kKESKBSCurrentMajorFormatNumber kKESKBSFirstMajorFormatNumber
#define kKESKBSCurrentMinorFormatNumber kKESKBSFirstMinorFormatNumber

#endif // __KESKBSID_h__
