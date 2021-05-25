//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "cvideo_windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************************************************************************
//глобальные переменные
//****************************************************************************************************

//****************************************************************************************************
//конструктор и деструктор
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CVideo_Windows::CVideo_Windows(uint32_t width,uint32_t height,CWnd *cWnd_Ptr_Set)
{
 cWnd_Ptr=cWnd_Ptr_Set;
 ScreenWidth=width;
 ScreenHeight=height;

 //загрузим курсоры
 hCursor_Magnify=LoadCursor(NULL,IDC_ARROW);//LoadCursor(AfxGetResourceHandle(),(LPCTSTR)IDC_CURSOR_MAGNIFY);
 hCursor_Cross=LoadCursor(NULL,IDC_CROSS);

 //шрифты
 LogFont.lfHeight=14; 
 LogFont.lfWidth=0; 
 LogFont.lfEscapement=0;
 LogFont.lfOrientation=0; 
 LogFont.lfWeight=FW_NORMAL; 
 LogFont.lfItalic=0;
 LogFont.lfUnderline=0; 
 LogFont.lfStrikeOut=0; 
 LogFont.lfCharSet=DEFAULT_CHARSET; 
 LogFont.lfOutPrecision=OUT_DEFAULT_PRECIS; 
 LogFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
 LogFont.lfQuality=DEFAULT_QUALITY; 
 LogFont.lfPitchAndFamily=DEFAULT_PITCH;  
 strcpy(LogFont.lfFaceName,"Times New Roman"); 

 Init();
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CVideo_Windows::~CVideo_Windows()
{
 cDC_ScreenMemCopy.DeleteDC();
 cBitmap_ScreenMemCopy.DeleteObject();

 cDC_ScreenMem.DeleteDC();
 cBitmap_ScreenMem.DeleteObject();

 DestroyCursor(hCursor_Magnify);
 DestroyCursor(hCursor_Cross);
}
//****************************************************************************************************
//закрытые функции
//****************************************************************************************************

//****************************************************************************************************
//открытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//инициализизация видеорежима
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::Init(void)
{
 CDC *cDC_Ptr=cWnd_Ptr->GetDC();

 cDC_ScreenMem.DeleteDC();
 cBitmap_ScreenMem.DeleteObject();
 cDC_ScreenMem.CreateCompatibleDC(cDC_Ptr);
 cBitmap_ScreenMem.CreateCompatibleBitmap(cDC_Ptr,ScreenWidth,ScreenHeight);
 cDC_ScreenMem.SelectObject(&cBitmap_ScreenMem);
 cDC_ScreenMem.SetMapMode(MM_TEXT);
 
 cDC_ScreenMemCopy.DeleteDC();
 cBitmap_ScreenMemCopy.DeleteObject();
 cDC_ScreenMemCopy.CreateCompatibleDC(cDC_Ptr);
 cBitmap_ScreenMemCopy.CreateCompatibleBitmap(cDC_Ptr,ScreenWidth,ScreenHeight);
 cDC_ScreenMemCopy.SelectObject(&cBitmap_ScreenMemCopy);
 cDC_ScreenMemCopy.SetMapMode(MM_TEXT);
}
//----------------------------------------------------------------------------------------------------
//получить размеры экрана
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::GetScreenSize(uint32_t &width,uint32_t &height) const
{
 width=ScreenWidth;
 height=ScreenHeight;
}
//----------------------------------------------------------------------------------------------------
//задать размеры экрана
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::SetScreenSize(uint32_t width,uint32_t height)
{
 ScreenWidth=width;
 ScreenHeight=height;
 Init();
}
//----------------------------------------------------------------------------------------------------
//получить указатель на видеобуфер
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::GetVideoPointer(void* &vptr)
{
 vptr=&cDC_ScreenMem;
}
//----------------------------------------------------------------------------------------------------
//получить размер строки в пикселях
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::GetLineSize(uint32_t &linesize) const
{
 linesize=ScreenWidth;
}
//----------------------------------------------------------------------------------------------------
//вывод символа в позицию
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::PutSymbol(int64_t x,int64_t y,char symbol,uint32_t color)
{ 
 char str[2]={symbol,0};
 PutString(x,y,str,color);
}
//----------------------------------------------------------------------------------------------------
//вывод строчки в позицию
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::PutString(int64_t x,int64_t y,const char *string,uint32_t color)
{
 uint8_t b=(color>>0)&0xff;
 uint8_t g=(color>>8)&0xff;
 uint8_t r=(color>>16)&0xff;

 CFont cFont;
 cFont.CreateFontIndirect(&LogFont);
 int oldbkmode=cDC_ScreenMem.SetBkMode(TRANSPARENT);
 CFont *cFont_Old=cDC_ScreenMem.SelectObject(&cFont);
 cDC_ScreenMem.SetTextColor(RGB(r,g,b));
 cDC_ScreenMem.TextOut(x,y,string);

 cDC_ScreenMem.SelectObject(cFont_Old);
 cFont.DeleteObject();
 cDC_ScreenMem.SetBkMode(oldbkmode);
}
//----------------------------------------------------------------------------------------------------
//рисование точки
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::DrawPoint(int64_t x,int64_t y,uint32_t color)
{
 FillRectangle(x-1,y-1,x+1,y+1,color);
}
//----------------------------------------------------------------------------------------------------
//рисование линии
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::DrawLine(int64_t x1,int64_t y1,int64_t x2,int64_t y2,uint32_t color,uint32_t thickness,LINE_TYPE line_type)
{	
 int oldbkmode=cDC_ScreenMem.SetBkMode(TRANSPARENT);
 HPEN hOldPen;
 HPEN hPen;
 uint8_t b=(color>>0)&0xff;
 uint8_t g=(color>>8)&0xff;
 uint8_t r=(color>>16)&0xff;
 int style=0;
 if (line_type==LINE_TYPE_SOLID) style=PS_SOLID;
 if (line_type==LINE_TYPE_DOT) style=PS_DOT;
 hPen=CreatePen(style,thickness,RGB(r,g,b));
 hOldPen=(HPEN)cDC_ScreenMem.SelectObject((HPEN)hPen);
 cDC_ScreenMem.MoveTo(CPoint(x1,y1));
 cDC_ScreenMem.LineTo(x2,y2);

 cDC_ScreenMem.SelectObject((HPEN)hOldPen);
 DeleteObject(hPen);
 cDC_ScreenMem.SetBkMode(oldbkmode);
}
//----------------------------------------------------------------------------------------------------
//получить размер строки в пикселях
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::GetStringImageSize(const char *string,uint32_t &width,uint32_t &height)
{
 CFont cFont;
 cFont.CreateFontIndirect(&LogFont);
 SIZE size;
 GetTextExtentPoint32(cDC_ScreenMem,string,strlen(string),&size);
 width=size.cx;
 height=size.cy;
}
//----------------------------------------------------------------------------------------------------
//нарисовать закрашеный прямоугольник
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::FillRectangle(int64_t x1,int64_t y1,int64_t x2,int64_t y2,uint32_t color)
{
 uint8_t b=(color>>0)&0xff;
 uint8_t g=(color>>8)&0xff;
 uint8_t r=(color>>16)&0xff;

 CBrush cBrush;
 cBrush.CreateSolidBrush(RGB(r,g,b));
 cDC_ScreenMem.FillRect(&CRect(x1,y1,x2,y2),&cBrush);
}
//----------------------------------------------------------------------------------------------------
//сделать курсор внутри области графика крестиком
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::SetCursorCross(void)
{
 SetCursor(hCursor_Cross);
}
//----------------------------------------------------------------------------------------------------
//сделать курсор внутри области графика лупой
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::SetCursorMagnify(void)
{
 SetCursor(hCursor_Magnify);
}
//----------------------------------------------------------------------------------------------------
//показать курсор
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::ShowCursor(void)
{
 while(::ShowCursor(TRUE)<0);
}
//----------------------------------------------------------------------------------------------------
//скрыть курсор
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::HideCursor(void)
{
 while(::ShowCursor(FALSE)>0);
}
//----------------------------------------------------------------------------------------------------
//отпустить курсор
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::UnClipCursor(void)
{
 while(::ClipCursor(NULL)==FALSE);
}
//----------------------------------------------------------------------------------------------------
//запереть курсор в области
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::ClipCursor(const CGrRect &cGrRect)
{
 CRect cRect(static_cast<int>(cGrRect.Left),static_cast<int>(cGrRect.Top),static_cast<int>(cGrRect.Right),static_cast<int>(cGrRect.Bottom)); 
 cWnd_Ptr->ClientToScreen(&cRect);
 while(::ClipCursor(&cRect)==FALSE);
}
//----------------------------------------------------------------------------------------------------
//установить координаты курсора
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::SetCursorPos(int32_t x,int32_t y)
{
 POINT point;
 point.x=x;
 point.y=y;
 cWnd_Ptr->ClientToScreen(&point);
 ::SetCursorPos(point.x,point.y); 
}
//----------------------------------------------------------------------------------------------------
//сохранить изображение в память
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::SaveScreen(void)
{
 //копируем нарисованное
 cDC_ScreenMemCopy.BitBlt(0,0,ScreenWidth,ScreenHeight,&cDC_ScreenMem,0,0,SRCCOPY);
}
//----------------------------------------------------------------------------------------------------
//восстановить изображение из памяти
//----------------------------------------------------------------------------------------------------
void CVideo_Windows::RestoreScreen(void)
{
 //копируем обратно
 cDC_ScreenMem.BitBlt(0,0,ScreenWidth,ScreenHeight,&cDC_ScreenMemCopy,0,0,SRCCOPY);
}
