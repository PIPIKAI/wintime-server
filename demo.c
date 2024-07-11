// View Code - SimpleEdit-Demo.c
   /*C语言Windows程序设计 -> 简易文本编辑器 -> 演示*/
   #define _WIN32_WINNT 0x0600
   #include <windows.h>
   #include <stdio.h>
   
   /*各控件所使用的ID*/
   #define ID_EDITBOX    1        //文本编辑框控件
   #define ID_TXTPATH    2        //路径编辑框控件
   #define ID_SAVEBTN    3        //保存文件按钮
  #define ID_CLSBTN    4        //清空编辑区按钮
  #define ID_GROUP    5        //组合框
  
  LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM ) ;
  int CreateChildWindow(HWND, HWND *, LPARAM ) ;                //创建将使用到的子窗口控件
  int SavaInputContent( TCHAR *, TCHAR * ) ;                    //保存输入的文字到文件
  
  int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow )
  {
      static TCHAR szAppName[] = TEXT( "demo" ) ;
      HWND        hwnd ;
      MSG            msg ;
      WNDCLASS    wndclass ;
  
      wndclass.lpfnWndProc    = WndProc ;
      wndclass.style            = CS_HREDRAW | CS_VREDRAW ;
      wndclass.hInstance        = hInstance ;
      wndclass.cbClsExtra        = 0 ;
      wndclass.cbWndExtra        = 0 ;
      wndclass.hbrBackground    = CreateSolidBrush(RGB(236, 233, 216)) ;
      wndclass.hCursor        = LoadCursor( NULL, IDC_ARROW ) ;
      wndclass.hIcon            = LoadIcon( NULL, IDI_APPLICATION ) ;
      wndclass.lpszClassName    = szAppName ;
      wndclass.lpszMenuName    = NULL ;
      
      if( !RegisterClass(&wndclass) )
      {
          MessageBox( NULL, TEXT("无法注册窗口类!"), TEXT("错误"), MB_OK | MB_ICONERROR ) ;
          return 0 ;
      }
  
      hwnd = CreateWindow( szAppName, TEXT("C语言Windows程序设计 -> 简易文本编辑器 -> 演示"), WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, CW_USEDEFAULT,
          CW_USEDEFAULT, CW_USEDEFAULT,
          NULL, NULL, hInstance, NULL ) ;
  
      ShowWindow( hwnd, iCmdShow ) ;
      UpdateWindow( hwnd ) ;
  
      while( GetMessage(&msg, NULL, 0, 0) )
      {
          TranslateMessage( &msg ) ;
          DispatchMessage( &msg ) ;
      }
  
      return msg.wParam ;
  }
  
  LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
  {
      static        HWND hwndChild[5] ;
      HDC            hdc ;
      PAINTSTRUCT ps ;
  
      RECT    rect ;
      static    TCHAR *szBuffer ;        //缓冲区
      static    TCHAR szPath[256] ;        //文本路径
      static  TCHAR szLineNum[32] ;
      static    TCHAR szCharNum[32] ;
      static int        iLength ;
      int iLineCount, iCharCount ;
  
      switch( message )
      {
      case WM_CREATE:
          CreateChildWindow( hwnd, hwndChild, lParam ) ;
          return 0 ;
  
      case WM_SIZE:
          GetClientRect(hwnd, &rect) ;
          MoveWindow( hwndChild[ID_EDITBOX], 0, 0, rect.right, rect.bottom-50, TRUE ) ;        //调整文本编辑区
          MoveWindow( hwndChild[ID_TXTPATH], 60,  rect.bottom-31, 200, 20, TRUE ) ;            //调整文本路径输入框
          MoveWindow( hwndChild[ID_SAVEBTN], 280, rect.bottom-35, 50,  25, TRUE ) ;            //调整保存按钮
          MoveWindow( hwndChild[ID_CLSBTN ], 400, rect.bottom-35, 50,  25, TRUE ) ;            //调整清空按钮
          MoveWindow( hwndChild[ID_GROUP  ], 10,  rect.bottom-50, 330, 48, TRUE ) ;            //调整组合框
          return 0 ;
  
      case WM_PAINT:
          GetClientRect(hwnd, &rect) ;
          hdc = BeginPaint( hwnd, &ps ) ;
          TextOut( hdc, 20, rect.bottom-30, TEXT("路径:"), lstrlen(TEXT("路径:")) ) ;
          TextOut( hdc, 500, rect.bottom-30, szLineNum, lstrlen(szLineNum) ) ;
          EndPaint( hwnd, &ps ) ;
          return 0 ;
  
      case WM_COMMAND:
          switch(LOWORD(wParam))
          {
          case ID_EDITBOX:
              switch(HIWORD(wParam))
             {
             case EN_UPDATE:
                 iLineCount = SendMessage( hwndChild[ID_EDITBOX], EM_GETLINECOUNT, 0, 0 ) ;
                 iCharCount = GetWindowTextLength( hwndChild[ID_EDITBOX] ) ;
                 wsprintf(szLineNum, "行数: %i    字符数量: %i", iLineCount, iCharCount) ;
                 InvalidateRect(hwnd, NULL, FALSE) ;
                 break ;
             default:
                 break ;
             }
             return 0 ;
 
         case ID_SAVEBTN:
             iLength = GetWindowTextLength(hwndChild[ID_EDITBOX]) ;
             if( iLength != 0)
                 szBuffer = malloc(iLength*2) ;
             else
                 return -1 ;
             GetWindowText( hwndChild[ID_EDITBOX], szBuffer, GetWindowTextLength(hwndChild[ID_EDITBOX]) + 1 ) ;
             if(GetWindowText( hwndChild[ID_TXTPATH], szPath, 256 ) < 1)
             {
                 MessageBox(NULL, TEXT("路径不能为空"), TEXT("提示"), MB_OK | MB_ICONINFORMATION) ;
                 return -1 ;
             }
             SavaInputContent( szPath, szBuffer ) ;
             return 0 ;
 
         case ID_CLSBTN:
             SetWindowText( hwndChild[ID_EDITBOX], TEXT("") ) ;
             return 0 ;
 
         default:
             break ;
         }
         return 0 ;
 
     case WM_DESTROY:
         PostQuitMessage(0) ;
         return 0 ;
     }
 
     return DefWindowProc( hwnd, message, wParam, lParam ) ;
 }
 
 int CreateChildWindow(HWND hwnd, HWND *hwndChild, LPARAM lParam)
 {
     HINSTANCE hInst = ((LPCREATESTRUCT) lParam) -> hInstance ;
 
     //创建编辑区
     hwndChild[ID_EDITBOX] = CreateWindow( TEXT("edit"), NULL,
         WS_CHILD | WS_VISIBLE | WS_VSCROLL  |
         ES_LEFT | ES_MULTILINE |  ES_AUTOVSCROLL,
         0, 0, 0, 0,
         hwnd, (HMENU)ID_EDITBOX, hInst, NULL ) ;
 
     //路径输入框
     hwndChild[ID_TXTPATH] = CreateWindow( TEXT("edit"), NULL,
         WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
         0, 0, 0, 0,
         hwnd, (HMENU)ID_TXTPATH, hInst, NULL ) ;
 
     //保存按钮
     hwndChild[ID_SAVEBTN] = CreateWindow( TEXT("button"), TEXT("保存"),
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
         hwnd, (HMENU)ID_SAVEBTN, hInst, NULL) ;
 
     //清空按钮
     hwndChild[ID_CLSBTN] = CreateWindow( TEXT("button"), TEXT("清空"),
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
         hwnd, (HMENU)ID_CLSBTN, hInst, NULL) ;
     
     //组合框
     hwndChild[ID_GROUP] = CreateWindow( TEXT("button"), NULL,
         WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0,
         hwnd, (HMENU)ID_GROUP, hInst, NULL) ;
 
     return 0 ;
 }
 
 int SavaInputContent( TCHAR *path, TCHAR *content )
 {
     FILE *fSvae ;
 
     fSvae = fopen( path, "w" ) ;
     if(fSvae == NULL)
     {
         MessageBox(NULL, TEXT("文件创建失败!"), TEXT("提示"), MB_OK | MB_ICONINFORMATION) ;
         return -1 ;
     }
     fputs( content, fSvae ) ;
     fclose(fSvae) ;
     MessageBox(NULL, TEXT("保存成功!"), TEXT("成功"), MB_OK | MB_ICONINFORMATION) ;
 
     return 0 ;
 }