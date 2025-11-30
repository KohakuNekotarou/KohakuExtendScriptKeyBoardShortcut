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

// Interface includes.
#include "IApplication.h"
#include "IDialog.h"
#include "IDialogController.h"
#include "IDialogMgr.h"
#include "IDropDownListController.h"
#include "IIdleTaskMgr.h"
#include "IMenuUtils.h"
#include "ITextControlData.h"
#include "IStaticTextAttributes.h"


#include "IIntData.h"



#include "IWindow.h"



#include "IBooleanControlData.h"
#include "IDialogController.h"
#include "IPanelControlData.h"
#include "IWidgetParent.h"



#include "KBSCEditorDlgID.h"

#include "widgetid.h" // for kButtonWidgetBoss etc.


// General includes.
#include "CIdleTask.h"
#include "CAlert.h" // CAlert::InformationAlert(Msg);


#include "LocaleSetting.h"

// Project includes.
#include "KESKBSID.h"

class KESKBSIdleTask : public CIdleTask
{
public:

	KESKBSIdleTask(IPMUnknown* boss);

	virtual uint32 RunTask(uint32 appFlags, IdleTimer* timeCheck);

private:

	void ExportCurrentKeyBoardShortcutSet(IWindow* iWindow);

	void kKBSCQueryEditorDialogWidgetHierarchy(IWindow* iWindow);

	void QueryWidgetHierarchy(IPanelControlData* iPanelControlData, PMString& pMString_result);
};

CREATE_PMINTERFACE(KESKBSIdleTask, kKESKBSIdleTaskImpl)

KESKBSIdleTask::KESKBSIdleTask(IPMUnknown *boss)
	:CIdleTask(boss)
{
}

uint32 KESKBSIdleTask::RunTask(uint32 appFlags, IdleTimer* timeCheck)
{
	// kModalDialogUp
	if( appFlags == IIdleTaskMgr::kModalDialogUp)
	{
		do
		{
			// ---------------------------------------------------------------------------------------
			// Query IWindow.
			InterfacePtr<IApplication> iApplication(::GetExecutionContextSession()->QueryApplication());
			if (iApplication == nil) break;

			InterfacePtr<IDialogMgr> iDialogMgr(iApplication, ::UseDefaultIID());
			if (iDialogMgr == nil) break;

			IWindow* iWindow = iDialogMgr->GetFrontmostDialogWindow();
			if (iWindow == nil) break;

			// ---------------------------------------------------------------------------------------
			// Get flg.
			InterfacePtr<IIntData> iIntData_KESKBSOpenEditCloseShortcutDialogFlg(
				::GetExecutionContextSession(), IID_IKESKBSOPENEDITCLOSESHORTCUTDIALOGFLG);
			if (iIntData_KESKBSOpenEditCloseShortcutDialogFlg == nil) break;

			switch (iIntData_KESKBSOpenEditCloseShortcutDialogFlg->Get())
			{
			case KESKBSOpenEditCloseShortcutDialogFlg::kExportSet:
				this->ExportCurrentKeyBoardShortcutSet(iWindow);
				break;

			case KESKBSOpenEditCloseShortcutDialogFlg::kWidgetHierarchy:
				this->kKBSCQueryEditorDialogWidgetHierarchy(iWindow);
				break;
			}

			// ---------------------------------------------------------------------------------------
			// Close.
			InterfacePtr<IDialog> iDialog(iWindow, ::UseDefaultIID());
			if (iDialog == nil) break;

			iDialog->PressDefaultButton();

		} while (false);
	}
	// Removes the task from its queues.
	return IIdleTask::kEndOfTime;
}

void KESKBSIdleTask::ExportCurrentKeyBoardShortcutSet(IWindow* iWindow)
{
	do
	{
		// ---------------------------------------------------------------------------------------
		// Find widget.
		InterfacePtr<IPanelControlData> iPanelControlData(iWindow, ::UseDefaultIID());
		if (iPanelControlData == nil) break;

		IControlView* iControlView_widget = iPanelControlData->FindWidget(kKBSCExportButtonWidgetId);
		if (iControlView_widget == nil) break;

		// ---------------------------------------------------------------------------------------
		// Press.
		InterfacePtr<IBooleanControlData> iBooleanControlData(iControlView_widget, ::UseDefaultIID());
		if (iBooleanControlData == nil) break;

		iBooleanControlData->Select();
		iBooleanControlData->Deselect();

	} while (false);
}

void KESKBSIdleTask::kKBSCQueryEditorDialogWidgetHierarchy(IWindow* iWindow)
{
	do
	{
		InterfacePtr<IPanelControlData> iPanelControlData(iWindow, ::UseDefaultIID());
		if (iPanelControlData == nil) break;

		// ---------------------------------------------------------------------------------------
		// 
		PMString pMString_result;
		this->QueryWidgetHierarchy(iPanelControlData, pMString_result);

		CAlert::InformationAlert(pMString_result);

	} while (false);
}

void KESKBSIdleTask::QueryWidgetHierarchy(IPanelControlData* iPanelControlData, PMString& pMString_result)
{
	do
	{
		int32 length = iPanelControlData->Length();
		for (int32 i = 0; i < length; i++)
		{
			// ---------------------------------------------------------------------------------------
			// 
			IControlView* iControlView = iPanelControlData->GetWidget(i);
			if (iControlView == nil) continue;

			WidgetID widgetID = iControlView->GetWidgetID();

			// ---------------------------------------------------------------------------------------
			// Query widget
			InterfacePtr<IPanelControlData> iPanelControlData_lower(iControlView, ::UseDefaultIID());
			if (iPanelControlData_lower == nil) { // nil

				PMString pMString_widgetInfo;

				// ---------------------------------------------------------------------------------------
				//
				PMString pMString_widgetValue = "";
				InterfacePtr<IPMUnknown> IPMUnknown_button(iControlView, IID_IBUTTONATTRIBUTES);
				if (IPMUnknown_button != nil)
				{
					pMString_widgetInfo.Append("Button");

					InterfacePtr<ITextControlData> iTextControlData(iControlView, ::UseDefaultIID());
					if (iTextControlData != nil) pMString_widgetValue = iTextControlData->GetString();
				}

				InterfacePtr<IDialogController> iDialogController(iControlView, ::UseDefaultIID());
				if (iDialogController != nil) pMString_widgetInfo.Append("Dialog");


				InterfacePtr<IDropDownListController> iDropDownListController(iControlView, ::UseDefaultIID());
				if (iDropDownListController != nil) pMString_widgetInfo.Append("DropDownList");

				InterfacePtr<IStaticTextAttributes> iStaticTextAttributes(iControlView, ::UseDefaultIID());
				if (iStaticTextAttributes != nil) pMString_widgetInfo.Append("StaticText");

				pMString_widgetInfo.Append(":");

				// WidgetValue
				if (pMString_widgetValue != "")
				{
					pMString_widgetInfo.Append(" ");

					// Translate
					pMString_widgetValue.Translate();

					// StripMenuAccelerator
					Utils<IMenuUtils>()->StripMenuAccelerator(
						&pMString_widgetValue, LocaleSetting::GetLocale().GetUserInterfaceId()
					);
					pMString_widgetInfo.Append(pMString_widgetValue);
				}

				pMString_widgetInfo.Append(" ");
				PMString pMString_widgetID;
				pMString_widgetID.AsNumber(widgetID.Get());
				pMString_widgetInfo.Append(" , id:");
				pMString_widgetInfo.Append(pMString_widgetID);

				if (pMString_result != "") pMString_result.Append("\n");
				pMString_result.Append(pMString_widgetInfo);

				// continue
				continue;
			}

			int32 length_lower = iPanelControlData_lower->Length();
			if (length_lower > 0)
			{
				this->QueryWidgetHierarchy(iPanelControlData_lower, pMString_result);
			}
		}
	} while (false);
}