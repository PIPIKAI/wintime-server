// View Code - SimpleEdit-Demo.c
   /*C����Windows������� -> �����ı��༭�� -> ��ʾ*/
   #define _WIN32_WINNT 0x0600
   #include <windows.h>
   #include <stdio.h>
   
   /*���ؼ���ʹ�õ�ID*/
   #define ID_EDITBOX    1        //�ı��༭��ؼ�
   #define ID_TXTPATH    2        //·���༭��ؼ�
   #define ID_SAVEBTN    3        //�����ļ���ť
  #define ID_CLSBTN    4        //��ձ༭����ť
  #define ID_GROUP    5        //��Ͽ�
  
  LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM ) ;
  int CreateChildWindow(HWND, HWND *, LPARAM ) ;                //������ʹ�õ����Ӵ��ڿؼ�
  int SavaInputContent( TCHAR *, TCHAR * ) ;                    //������������ֵ��ļ�
  
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
          MessageBox( NULL, TEXT("�޷�ע�ᴰ����!"), TEXT("����"), MB_OK | MB_ICONERROR ) ;
          return 0 ;
      }
  
      hwnd = CreateWindow( szAppName, TEXT("C����Windows������� -> �����ı��༭�� -> ��ʾ"), WS_OVERLAPPEDWINDOW,
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
      static    TCHAR *szBuffer ;        //������
      static    TCHAR szPath[256] ;        //�ı�·��
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
          MoveWindow( hwndChild[ID_EDITBOX], 0, 0, rect.right, rect.bottom-50, TRUE ) ;        //�����ı��༭��
          MoveWindow( hwndChild[ID_TXTPATH], 60,  rect.bottom-31, 200, 20, TRUE ) ;            //�����ı�·�������
          MoveWindow( hwndChild[ID_SAVEBTN], 280, rect.bottom-35, 50,  25, TRUE ) ;            //�������水ť
          MoveWindow( hwndChild[ID_CLSBTN ], 400, rect.bottom-35, 50,  25, TRUE ) ;            //������հ�ť
          MoveWindow( hwndChild[ID_GROUP  ], 10,  rect.bottom-50, 330, 48, TRUE ) ;            //������Ͽ�
          return 0 ;
  
      case WM_PAINT:
          GetClientRect(hwnd, &rect) ;
          hdc = BeginPaint( hwnd, &ps ) ;
          TextOut( hdc, 20, rect.bottom-30, TEXT("·��:"), lstrlen(TEXT("·��:")) ) ;
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
                 wsprintf(szLineNum, "����: %i    �ַ�����: %i", iLineCount, iCharCount) ;
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
                 MessageBox(NULL, TEXT("·������Ϊ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION) ;
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
 
     //�����༭��
     hwndChild[ID_EDITBOX] = CreateWindow( TEXT("edit"), NULL,
         WS_CHILD | WS_VISIBLE | WS_VSCROLL  |
         ES_LEFT | ES_MULTILINE |  ES_AUTOVSCROLL,
         0, 0, 0, 0,
         hwnd, (HMENU)ID_EDITBOX, hInst, NULL ) ;
 
     //·�������
     hwndChild[ID_TXTPATH] = CreateWindow( TEXT("edit"), NULL,
         WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
         0, 0, 0, 0,
         hwnd, (HMENU)ID_TXTPATH, hInst, NULL ) ;
 
     //���水ť
     hwndChild[ID_SAVEBTN] = CreateWindow( TEXT("button"), TEXT("����"),
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
         hwnd, (HMENU)ID_SAVEBTN, hInst, NULL) ;
 
     //��հ�ť
     hwndChild[ID_CLSBTN] = CreateWindow( TEXT("button"), TEXT("���"),
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
         hwnd, (HMENU)ID_CLSBTN, hInst, NULL) ;
     
     //��Ͽ�
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
         MessageBox(NULL, TEXT("�ļ�����ʧ��!"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION) ;
         return -1 ;
     }
     fputs( content, fSvae ) ;
     fclose(fSvae) ;
     MessageBox(NULL, TEXT("����ɹ�!"), TEXT("�ɹ�"), MB_OK | MB_ICONINFORMATION) ;
 
     return 0 ;
 }