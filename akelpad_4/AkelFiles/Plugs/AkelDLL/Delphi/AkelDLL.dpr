library AkelDLL_lib;

uses
  Windows, Messages, SysUtils, RichEdit,
  AkelDLL_h;

//Identification
procedure DllAkelPadID(var pv: TPluginVersion); cdecl; export;
begin
  pv.dwAkelDllVersion:= AKELDLL;
  pv.dwExeMinVersion3x:= MakeIdentifier(3,7,4,0);
  pv.dwExeMinVersion4x:= MakeIdentifier(4,7,0,0);
end;

//Plugin extern function
procedure GetSelTextTest(var pd: TPluginData); cdecl; export;
var
  gtr: TGetTextRange;
  cr: TCharRange;
  nLen: Integer;
begin
  // Function doesn't support autoload
  if (pd.lpbAutoLoad <> nil) then
  begin
    pd.lpbAutoLoad^:= False;
    Exit;
  end;

  // If function load on start (autoload), then exit
  if pd.bOnStart then
    Exit;

  // Get current text selection
  SendMessage(pd.hWndEdit, EM_EXGETSEL, 0, LPARAM(@cr));
  gtr.cpMin:= cr.cpMin;
  gtr.cpMax:= cr.cpMax;

  // Get text from selection
  nLen:= SendMessage(pd.hMainWnd, AKD_GETTEXTRANGE, WPARAM(pd.hWndEdit), LPARAM(@gtr));
  if (nLen > 0) then
  begin
    // Show result
    if pd.bOldWindows then
      MessageBoxA(pd.hMainWnd, PChar(gtr.pText), 'Test', MB_OK)
    else
      MessageBoxW(pd.hMainWnd, PWideChar(gtr.pText), 'Test', MB_OK);

    // Free text buffer allocated with AKD_GETTEXTRANGE
    SendMessage(pd.hMainWnd, AKD_FREETEXT, 0, LPARAM(gtr.pText));
  end
  else
  begin
    // Show result
    if pd.bOldWindows then
      MessageBoxA(pd.hMainWnd, 'Text doesn''t selected', 'Test', MB_OK)
    else
      MessageBoxW(pd.hMainWnd, 'Text doesn''t selected', 'Test', MB_OK);
  end;
end;

//Plugin extern function
procedure OpenDialogTest(var pd: TPluginData); cdecl; export;
begin
  //Function doesn't support autoload
  if (pd.lpbAutoLoad <> nil) then
  begin
    pd.lpbAutoLoad^:= False;
    Exit;
  end;

  //If function load on start (autoload), then exit
  if pd.bOnStart then
    Exit;

  SendMessage(pd.hMainWnd, WM_COMMAND, IDM_FILE_OPEN, 0);
end;

//Entry point
procedure DLLEntryPoint(dwReason: DWord);
begin
  case dwReason of
    DLL_PROCESS_ATTACH: ;
    DLL_PROCESS_DETACH: ;
    DLL_THREAD_ATTACH:  ;
    DLL_THREAD_DETACH:  ;
  end;
end;

exports
  DllAkelPadID,
  GetSelTextTest,
  OpenDialogTest;

begin
  DllProc:= @DLLEntryPoint;
end.
