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
#include "IEditBoxAttributes.h"
#include "IIdleTaskMgr.h"
#include "IListBoxController.h"
#include "IMenuUtils.h"
#include "IStaticTextAttributes.h"
#include "IStringListControlData.h"
#include "ITextControlData.h"












#include "IIntData.h"



#include "IWindow.h"

#include "IPNGIconAttributes.h"

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

	void KBSCQueryKeyBoardShortcutEditorDialogWidget(IWindow* iWindow);

	void QueryWidget(IPanelControlData* iPanelControlData, PMString& pMString_result);
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

			case KESKBSOpenEditCloseShortcutDialogFlg::kQueryWidget:
				this->KBSCQueryKeyBoardShortcutEditorDialogWidget(iWindow);
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

void KESKBSIdleTask::KBSCQueryKeyBoardShortcutEditorDialogWidget(IWindow* iWindow)
{
	do
	{
		InterfacePtr<IPanelControlData> iPanelControlData(iWindow, ::UseDefaultIID());
		if (iPanelControlData == nil) break;

		// ---------------------------------------------------------------------------------------
		// 
		PMString pMString_result;
		this->QueryWidget(iPanelControlData, pMString_result);

		CAlert::InformationAlert(pMString_result);

	} while (false);
}

void KESKBSIdleTask::QueryWidget(IPanelControlData* iPanelControlData, PMString& pMString_result)
{
	std::vector<PMString> vector_pMString_widget{
		"",
		"kKBSCEditorDlgId",
		"kKBSCActionsListWidgetId",
		"kKBSCShortcutsListWidgetId",
		"kKBSCShortcutEditBoxWidgetId",
		"kKBSCCurrentActionTextWidgetId",
		"kKBSCAssignButtonWidgetId",
		"kKBSCAreaRowWidgetId",
		"kKBSCSetsComboWidgetId",
		"kKBSCProductAreasComboWidgetId",
		"kKBSCDuplicateButtonWidgetId",
		"kKBSCDeleteButtonWidgetId",
		"kKBSCSaveButtonWidgetId",
		"kKBSCExportButtonWidgetId",
		"kKBSCDuplicateDlgId",
		"kKBSCNewNameEditBoxWidgetId",
		"kKBSCBasedOnComboWidgetId",
		"kKBSCSetLabelWidgetId",
		"kProductAreaLabelWidgetId",
		"kCommandsLabelWidgetId",
		"kKBSCGroupWidgetId",
		"kPressLabelWidgetId",
		"kCurrentLabelWidgetId",
		"kKBSCGroup2WidgetId",
		"kNameLabelWidgetId",
		"kBasedOnLabelWidgetId",
		"kKBSCRemoveButtonWidgetId",
		"kKBSCContextListWidgetID",
		"kContextLabelWidgetID",
		"kKBSCPsIconWidgetID",
		"kKBSCAiIconWidgetID",
		"kPsAiIconPanelWidgetID"
	};

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
			PMString pMString_widgetInfo = "";
			InterfacePtr<IPanelControlData> iPanelControlData_lower(iControlView, ::UseDefaultIID());
			if (iPanelControlData_lower == nil) { // nil

				PMString pMString_widgetValue = "";

				// Button
				InterfacePtr<IPMUnknown> IPMUnknown_button(iControlView, IID_IBUTTONATTRIBUTES);
				if (IPMUnknown_button != nil)
				{
					pMString_widgetInfo.Append("<Button> ");

					InterfacePtr<ITextControlData> iTextControlData(iControlView, ::UseDefaultIID());
					if (iTextControlData != nil) pMString_widgetValue = iTextControlData->GetString();
				}

				// DropDownList
				InterfacePtr<IDropDownListController> iDropDownListController(iControlView, ::UseDefaultIID());
				if (iDropDownListController != nil)
				{
					pMString_widgetInfo.Append("<DropDownList> ");

					InterfacePtr<IStringListControlData> iStringListControlData(iControlView, ::UseDefaultIID());
					if (iStringListControlData != nil)
					{
						pMString_widgetValue = iStringListControlData->GetString(iDropDownListController->GetSelected());
					}
				}

				// EditBox
				InterfacePtr<IEditBoxAttributes> iEditBoxAttributes(iControlView, ::UseDefaultIID());
				if (iEditBoxAttributes != nil) pMString_widgetInfo.Append("<EditBox>");

				// Icon
				InterfacePtr<IPNGIconAttributes> iPNGIconAttributes(iControlView, ::UseDefaultIID());
				if (iPNGIconAttributes != nil) pMString_widgetInfo.Append("<Icon>");

				// StaticText
				InterfacePtr<IStaticTextAttributes> iStaticTextAttributes(iControlView, ::UseDefaultIID());
				if (iStaticTextAttributes != nil)
				{
					pMString_widgetInfo.Append("<StaticText> ");
					InterfacePtr<ITextControlData> iTextControlData(iControlView, ::UseDefaultIID());
					if (iTextControlData != nil) pMString_widgetValue = iTextControlData->GetString();
				}

				// WidgetValue
				if (pMString_widgetValue != "")
				{
					// Translate
					pMString_widgetValue.Translate();

					// StripMenuAccelerator
					Utils<IMenuUtils>()->StripMenuAccelerator(
						&pMString_widgetValue, LocaleSetting::GetLocale().GetUserInterfaceId()
					);
					pMString_widgetInfo.Append(pMString_widgetValue);
				}

				// ---------------------------------------------------------------------------------------
				// kWidgetIDSpace
				if (pMString_widgetInfo != "") pMString_widgetInfo.Append(" , ");

				for (int32 i = 0; i < vector_pMString_widget.size(); i++)
				{
					if (widgetID.Get() == kKBSCEditorDlgPrefix + i)
					{
						pMString_widgetInfo.Append(vector_pMString_widget[i]);
						pMString_widgetInfo.Append(" ");
					}
				}

				if (pMString_widgetInfo != "")
				{
					PMString pMString_widgetID;
					pMString_widgetID.AsNumber(widgetID.Get());
					pMString_widgetInfo.Append(pMString_widgetID);

					if (pMString_result != "") pMString_result.Append("\n");
					pMString_result.Append(pMString_widgetInfo);
				}

				// continue
				continue;
			}
			
			// ---------------------------------------------------------------------------------------
			// kWidgetIDSpace
			for (int32 i = 0; i < vector_pMString_widget.size(); i++)
			{
				if (widgetID.Get() == kKBSCEditorDlgPrefix + i)
				{
					// Dialog
					InterfacePtr<IDialogController> iDialogController(iControlView, ::UseDefaultIID());
					if (iDialogController != nil)
					{
						pMString_widgetInfo.Append("<Dialog>");
						pMString_widgetInfo.Append(" ");

						PMString pMString_name;
						iDialogController->GetName(pMString_name);

						// Translate
						pMString_name.Translate();
						pMString_widgetInfo.Append(pMString_name);
					}

					// ListBox
					InterfacePtr<IListBoxController> iListBoxController(iControlView, ::UseDefaultIID());
					if (iListBoxController != nil)
					{
						pMString_widgetInfo.Append("<ListBox>");
					}

					if (pMString_widgetInfo != "")
					{
						pMString_widgetInfo.Append(" , ");
						pMString_widgetInfo.Append(vector_pMString_widget[i]);
						pMString_widgetInfo.Append(" ");

						PMString pMString_widgetID;
						pMString_widgetID.AsNumber(widgetID.Get());
						pMString_widgetInfo.Append(pMString_widgetID);
					}
					else
					{
						pMString_widgetInfo.Append("<kWidgetIDSpace> ");
						pMString_widgetInfo.Append(vector_pMString_widget[i]);
					}
				}
			}

			if (pMString_widgetInfo != "")
			{
				if (pMString_result != "") pMString_result.Append("\n");

				pMString_result.Append(pMString_widgetInfo);
			}

			int32 length_lower = iPanelControlData_lower->Length();
			if (length_lower > 0)
			{
				this->QueryWidget(iPanelControlData_lower, pMString_result);
			}
		}
	} while (false);
}