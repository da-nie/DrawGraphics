//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "cwnd_graphics.h"

//****************************************************************************************************
//глобальные переменные
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//макроопределения
//****************************************************************************************************


//****************************************************************************************************
//карта сообщений
//****************************************************************************************************

BEGIN_MESSAGE_MAP(CWnd_Graphics,CWnd)
 ON_WM_DESTROY()
 ON_WM_PAINT()
 ON_WM_LBUTTONDOWN()
 ON_WM_RBUTTONDOWN()
 ON_WM_RBUTTONUP()
 ON_WM_LBUTTONUP()
 ON_WM_MOUSEMOVE()
 ON_WM_ERASEBKGND()
 ON_WM_SETCURSOR()
 ON_WM_TIMER()
END_MESSAGE_MAP()

//****************************************************************************************************
//конструктор и деструктор
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CWnd_Graphics::CWnd_Graphics(void)
{
 LButtonState=false;
 RButtonState=false;
 CButtonState=false;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CWnd_Graphics::~CWnd_Graphics()
{
}

//****************************************************************************************************
//закрытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//задать состояние кнопок мышки
//----------------------------------------------------------------------------------------------------
void CWnd_Graphics::SetMouseButtonState(UINT nFlags)
{
 LButtonState=false;
 RButtonState=false;
 CButtonState=false;
 if (nFlags&MK_LBUTTON) LButtonState=true;
 if (nFlags&MK_RBUTTON) RButtonState=true;
 if (nFlags&MK_MBUTTON) CButtonState=true;
}

//****************************************************************************************************
//функции обработки сообщений
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//событие уничтожения окна
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnDestroy(void)
{
 KillTimer(IDC_GRAPHICS_TIMER_MAIN);
 CWnd::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnTimer(UINT nIDEvent)
{
 if (nIDEvent==IDC_GRAPHICS_TIMER_MAIN)
 {
  CPoint cPoint;
  GetCursorPos(&cPoint);
  ScreenToClient(&cPoint);
  iGraphics_Ptr->OnTimer(CGrMousePoint(cPoint.x,cPoint.y),LButtonState,RButtonState,CButtonState);

  InvalidateRect(NULL,FALSE);
 }
 CWnd::OnTimer(nIDEvent);
}
//----------------------------------------------------------------------------------------------------
//событие отрисовки окна
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnPaint(void)
{
 /*

 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 BITMAPINFOHEADER bmih;
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=width;
 bmih.biHeight=-static_cast<int32_t>(height);
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;
 RGBQUAD rgbq;
 BITMAPINFO info;
 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 CRect cRect;
 GetClientRect(&cRect);

 void *vptr;
 iVideo_Ptr->GetVideoPointer(vptr);
 StretchDIBits(cPaintDC,cRect.left,cRect.top,cRect.right-cRect.left,cRect.bottom-cRect.top,0,0,abs(bmih.biWidth),abs(bmih.biHeight),vptr,&info,DIB_RGB_COLORS,SRCCOPY);
 */

 void *vptr;
 iVideo_Ptr->GetVideoPointer(vptr);

 CPaintDC cPaintDC(this);
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);
 //выводим на экран
 cPaintDC.BitBlt(0,0,width,height,reinterpret_cast<CDC*>(vptr),0,0,SRCCOPY);

 CWnd::OnPaint(); 
}
//----------------------------------------------------------------------------------------------------
//событие очистки фона окна
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CWnd_Graphics::OnEraseBkgnd(CDC *pDC)
{
 if (pDC->IsPrinting()) return(CWnd::OnEraseBkgnd(pDC));
 return(TRUE);
}
//----------------------------------------------------------------------------------------------------
//событие нажатия левой кнопки мышки
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnLButtonDown(UINT nFlags,CPoint point)
{ 
 SetMouseButtonState(nFlags);
 iGraphics_Ptr->OnLButtonDown(CGrMousePoint(point.x,point.y),LButtonState,RButtonState,CButtonState); 
}
//----------------------------------------------------------------------------------------------------
//событие нажатия правой кнопки мышки
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnRButtonDown(UINT nFlags,CPoint point)
{
 SetMouseButtonState(nFlags);
 iGraphics_Ptr->OnRButtonDown(CGrMousePoint(point.x,point.y),LButtonState,RButtonState,CButtonState);
}
//----------------------------------------------------------------------------------------------------
//событие отпускания правой кнопки мышки
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnRButtonUp(UINT nFlags,CPoint point)
{
 SetMouseButtonState(nFlags);
 iGraphics_Ptr->OnRButtonUp(CGrMousePoint(point.x,point.y),LButtonState,RButtonState,CButtonState);
}
//----------------------------------------------------------------------------------------------------
//событие отпускания левой кнопки мышки
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnLButtonUp(UINT nFlags,CPoint point)
{
 SetMouseButtonState(nFlags);
 iGraphics_Ptr->OnLButtonUp(CGrMousePoint(point.x,point.y),LButtonState,RButtonState,CButtonState);
}
//----------------------------------------------------------------------------------------------------
//событие перемещения мышки
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Graphics::OnMouseMove(UINT nFlags,CPoint point)
{
 SetMouseButtonState(nFlags);
 iGraphics_Ptr->OnMouseMove(CGrMousePoint(point.x,point.y),LButtonState,RButtonState,CButtonState);
}
//----------------------------------------------------------------------------------------------------
//событие смены курсора
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CWnd_Graphics::OnSetCursor(CWnd *pWnd,UINT nHitTest,UINT message)
{
 if (pWnd==this) return(TRUE);
 return(CWnd::OnSetCursor(pWnd,nHitTest,message));
}


//****************************************************************************************************
//открытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//создать окно
//----------------------------------------------------------------------------------------------------
BOOL CWnd_Graphics::Create(DWORD dwStyle,RECT rect,CWnd *pParentWnd)
{
 //регистрируем класс окна
 HICON hIcon=LoadIcon(NULL,IDI_APPLICATION);
 HCURSOR hCursor=LoadCursor(NULL,IDC_CROSS);
 HBRUSH hbrBackground=(HBRUSH)(COLOR_WINDOW);
 ClassName=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,hCursor,hbrBackground,hIcon);

// iVideo_Ptr.reset(IVideo::CreateNewCVideoSoftware(rect.right-rect.left,rect.bottom-rect.top));
 iVideo_Ptr.reset(IVideo::CreateNewCVideoWindows(rect.right-rect.left,rect.bottom-rect.top,this));
 iVideo_Ptr->Init();
 iGraphics_Ptr.reset(IGraphics::CreateNewCGraphics(iVideo_Ptr.get()));  

 BOOL result=CWnd::Create(ClassName,NULL,WS_CHILD|dwStyle,rect,pParentWnd,NULL);
 SetTimer(IDC_GRAPHICS_TIMER_MAIN,10,NULL);
 return(result); 
}
//----------------------------------------------------------------------------------------------------
//получить класс работы с графиками
//----------------------------------------------------------------------------------------------------
IGraphics* CWnd_Graphics::GetIGraphicsPtr(void)
{
 return(iGraphics_Ptr.get());
}