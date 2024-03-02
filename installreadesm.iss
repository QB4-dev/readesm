#define EXE "{app}\bin\readesm.exe"

[Setup]
AppCopyright=Andreas G�lzer
AppName=ReadESM
AppVersion=2018.1
ChangesAssociations=true
DefaultDirName=ReadESM
DefaultGroupName=ReadESM
SetupIconFile=resources\icons\readesm.ico
WizardImageFile=resources\windows-integration\installer.bmp
AppID=readesm_isl
OutputBaseFilename=readesm-setup
LicenseFile=COPYING
DisableDirPage=auto
DisableProgramGroupPage=auto
AppPublisher=Andreas G�lzer
AppPublisherURL=http://andreas.goelzer.de/

[Files]
Source: "build/readesm.exe"; DestDir: "{app}\bin";
Source: "build/*.qm"; DestDir: "{app}\translations";
Source: "resources\windows-integration\readesm.url"; DestDir: "{app}\doc";
Source: "homepage\help.html"; DestDir: "{app}\doc";
Source: "homepage\style.css"; DestDir: "{app}\doc";
Source: "dlls/*.dll"; DestDir: "{app}\bin";

[Icons]
Name: "{group}\ReadESM"; Filename: {#EXE}; WorkingDir: "{app}"
Name: "{group}\ReadESM Project Page"; Filename: "{app}\doc\readesm.url"
Name: "{userdesktop}\ReadESM"; Filename: {#EXE}; WorkingDir: "{app}"

[Registry]
Root: HKCR; Subkey: ".ddd"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".esm"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".tgd"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".add"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "x-esm.File"; ValueType: string; ValueName: ""; ValueData: "Digital Tachograph File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "x-esm.File\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\readesm.exe,0"
Root: HKCR; Subkey: "x-esm.File\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{#EXE}"" ""%1"""

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:languages\German.isl"
Name: "hu"; MessagesFile: "compiler:languages\Hungarian.isl"
Name: "ru"; MessagesFile: "compiler:languages\Russian.isl"
Name: "es"; MessagesFile: "compiler:languages\Spanish.isl"

[CustomMessages]
HomePageLink=readesm.sourceforge.net

[Code]
const Url = 'http://readesm.sourceforge.net/';

procedure UrlLabelClick(Sender: TObject);
var
errorCode : integer;
begin
ShellExec('open',Url,'','',SW_SHOWNORMAL,ewNoWait, errorCode);
end;

procedure InitializeWizard;
var
UrlLabel :TNewStaticText;
CancelBtn :TButton;
begin
CancelBtn := WizardForm.CancelButton;
UrlLabel := TNewStaticText.Create(WizardForm);
UrlLabel.Top := CancelBtn.Top + (CancelBtn.Height div 2) -(UrlLabel.Height div 2);
UrlLabel.Left := WizardForm.ClientWidth - CancelBtn.Left -CancelBtn.Width;
UrlLabel.Caption := ExpandConstant('{cm:HomePageLink}');
UrlLabel.Font.Style := UrlLabel.Font.Style + [fsUnderline] ;
UrlLabel.Cursor := crHand;
UrlLabel.Font.Color := clHighlight;
UrlLabel.OnClick := @UrlLabelClick;
UrlLabel.Parent := WizardForm;
end;
