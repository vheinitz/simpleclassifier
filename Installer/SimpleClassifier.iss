;InnoSetup installer-builder script for SimpleClassifier.

[Setup]
AppName=SimpleClassifier
AppVerName=SimpleClassifier 0.1
OutputBaseFilename=SimpleClassifierSetup_0.1.0.0
AppPublisher=MLTools
AppCopyright=Copyright 2021 MLTools
DefaultDirName={pf}\\MLTools\\SimpleClassifier
DefaultGroupName=MLTools\SimpleClassifier
DisableDirPage=no
DisableProgramGroupPage=yes
AllowNoIcons=yes
OutputDir=..\Releases
BackColor=clGreen
;SetupIconFile=..\SimpleClassifier\image.ico
Compression=lzma
;WizardImageFile=.\res\image.bmp
;WizardSmallImageFile=.\res\image_s.bmp
SolidCompression=yes


[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]

Source: "..\deploy\translations\*.*"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "..\deploy\styles\*.*"; DestDir: "{app}\styles"; Flags: ignoreversion
Source: "..\deploy\platforms\*.*"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "..\deploy\iconengines\*.*"; DestDir: "{app}\iconengines"; Flags: ignoreversion

Source: "..\deploy\*.*"; DestDir: "{app}"; Flags: ignoreversion



[Icons]
Name: "{group}\SimpleClassifier"; Filename: "{app}\SimpleClassifier.exe"

