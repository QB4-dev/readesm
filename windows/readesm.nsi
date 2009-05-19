; Generated NSIS script file (generated by makensitemplate.phtml 0.21)
; by 217.162.229.131 on May 19 09 @ 00:06

; NOTE: this .NSI script is designed for NSIS v1.8+

Name "readesm"
OutFile "Install_readesm_windows.exe"
!include "FileAssociation.nsh"

; Some default compiler settings (uncomment and change at will):
; SetCompress auto ; (can be off or force)
; SetDatablockOptimize on ; (can be off)
; CRCCheck on ; (can be off)
; AutoCloseWindow false ; (can be true for the window go away automatically at end)
; ShowInstDetails hide ; (can be show to have them shown, or nevershow to disable)
; SetDateSave off ; (can be on to have files restored to their orginal date)
LicenseText "You must agree to this license before installing."
LicenseData "../COPYING"

InstallDir "$PROGRAMFILES\readesm"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\readesm" ""
; DirShow show; (make this hide to not let the user change it)
DirText "Select the directory to install readesm in:"

Section "" ; (default section)
SetOutPath "$INSTDIR"
; add files / whatever that need to be installed here.
file ../readesm.exe
file ../README
file ../COPYING
file ../readesm-wrap-windows.bat
;
WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\readesm" "" "$INSTDIR"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\readesm" "DisplayName" "readesm (remove only)"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\readesm" "UninstallString" '"$INSTDIR\uninst.exe"'
${registerExtension} "$INSTDIR\readesm-wrap-windows.bat" ".esm" "ESM Digital Tachograph File"
; write out uninstaller
WriteUninstaller "$INSTDIR\uninst.exe"
SectionEnd ; end of default section


; begin uninstall settings/section
UninstallText "This will uninstall readesm from your system"

Section Uninstall
; add delete commands to delete whatever files/registry keys/etc you installed here.
Delete "$INSTDIR\uninst.exe"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\readesm"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\readesm"
${unregisterExtension} ".esm" "ESM Digital Tachograph File"
RMDir "$INSTDIR"
SectionEnd ; end of uninstall section

; eof
