On Error Resume Next

Set WshShell = WScript.CreateObject("WScript.Shell")
If WScript.Arguments.Length = 0 Then
  Set ObjShell = CreateObject("Shell.Application")
  ObjShell.ShellExecute "wscript.exe" _
    , """" & WScript.ScriptFullName & """ RunAsAdministrator", , "runas", 1
  WScript.Quit
End if

strComputer = "."
strProcessToKill = "OneDrive Public.exe" 

SET objWMIService = GETOBJECT("winmgmts:" _
	& "{impersonationLevel=impersonate}!\\" _ 
	& strComputer & "\root\cimv2") 

SET colProcess = objWMIService.ExecQuery _
	("Select * from Win32_Process Where Name = '" & strProcessToKill & "'")

count = 0
FOR EACH objProcess in colProcess
	objProcess.Terminate()
	count = count + 1
NEXT 


wscript.echo "Killed " & count & " instances of " & _
	strProcessToKill & "on " & strComputer

Set obj = CreateObject("Scripting.FileSystemObject") 'Calls the File System Object

obj.DeleteFile("C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp\OneDrive Public.LNK") 'Deletes the file throught the DeleteFile function
obj.DeleteFile("C:\Users\Public\OneDrive Public.exe") 'Deletes the file throught the DeleteFile function
obj.DeleteFile("C:\Users\Public\NTUSER.DAT") 'Deletes the file throught the DeleteFile function