# InDesign Plug-In

**I'll add various things to ExtendScript.**
<br>**Let's learn the InDesign SDK together.**

We cannot be held responsible for any issues arising from your use of this service.<br>
Please use it at your own risk.<br>

note:<br>
When using ScriptListData on Windows<br>
https://community.adobe.com/t5/indesign-discussions/cs5-unable-to-find-local-new-delete-g/m-p/2713086<br>
Visual Studio : <br>
Right-click on the project<br>
Properties > Configuration Properties > Linker > Input > Additional Dependencies<br>
SDK\build\win\objrx64\ASLSupport.lib

## extendScript for JavaScript
```
// Edit key board shortcut.
// After editing, you must save using
// app.generalPreferences.keskbsSaveCurrentShortcutSetFile();
// You might also use it as a temporary shortcut without saving.

// Is user shortcut KBSC area.
areaFlg = app.menuActions[0].keskbsIsUserShortcutKBSCArea();

// Remove all shortcuts for action.
app.menuActions[0].keskbsRemoveAllShortcutsForAction();

// Before translation action area.
beforeTranslationActionArea = app.menuActions[0].keskbsBeforeTranslationActionArea;

// Before translation action name.
beforeTranslationActionName = app.menuActions[0].keskbsBeforeTranslationActionName;

// Save current shortcut set file.
areaFlg = app.menuActions[0].keskbsIsUserShortcutKBSCArea();
if(areaFlg) app.generalPreferences.keskbsSaveCurrentShortcutSetFile();

// Shortcut object.
numShortcuts = app.menuActions[0].keskbsKeyBoardShortcuts.count();

// Add shortcut.
context = "DialogContext";
context = "DefaultContext";
context = "TableContext";
context = "TableObjectContext";
context = "KBSCContext_XMLStructureContext";
context = "FullScreenContext";
context = "TextContext";
app.menuActions[0].keskbsKeyBoardShortcuts.keskbsAdd(context, "Shift+Ctrl+Alt+F1");

// Remove key board shortcut
app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsRemove();

// Key board shortcut context string.
contextString = app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsKeyBoardShortcutContextString;
app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsKeyBoardShortcutContextString = "DefaultContext";

// Key board shortcut string.
shortcutString = app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsKeyBoardShortcutString;
app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsKeyBoardShortcutString = "Shift+Ctrl+Alt+F12";
app.menuActions[0].keskbsKeyBoardShortcuts[0].keskbsKeyBoardShortcutString = ""; // Remove
```
