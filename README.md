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

// Is user shortcut KBSC area.
areaFlg = app.menuActions[0].keskbsIsUserShortcutKBSCArea();

// Remove all shortcuts for action.
app.menuActions[0].keskbsRemoveAllShortcutsForAction();

// Before translation action area.
beforeTranslationActionArea = app.menuActions[0].keskbsBeforeTranslationActionArea;

// Before translation action name.
beforeTranslationActionName = app.menuActions[0].keskbsBeforeTranslationActionName;

// Save current shortcut set file.
app.generalPreferences.keskbsSaveCurrentShortcutSetFile();






```
