#ifndef C_VIDEO_QT_H
#define C_VIDEO_QT_H

//****************************************************************************************************
//класс рисования с помощью Qt
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <QtWidgets>
#include <stdint.h>
#include <memory>
#include "ivideo.h"

//****************************************************************************************************
//класс рисования с помощью Qt
//****************************************************************************************************

class CVideo_Qt:public IVideo
{
 private:
  //-переменные-----------------------------------------------------------------------------------------
  uint32_t ScreenWidth;//ширина экрана
  uint32_t ScreenHeight;//высота экрана
  std::unique_ptr<QPixmap> qPixmap_Ptr;//указатель на буфер рисования
  std::unique_ptr<QPixmap> qPixmap_Copy_Ptr;//указатель на копию буфера рисования
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CVideo_Qt(uint32_t width,uint32_t height);
  //-деструктор-----------------------------------------------------------------------------------------
  ~CVideo_Qt();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  void Init(void);//инициализизация видеорежима
  void GetScreenSize(uint32_t &width,uint32_t &height) const;//получить размеры экрана
  void SetScreenSize(uint32_t width,uint32_t height);//задать размеры экрана
  void GetVideoPointer(void* &vptr);//получить указатель на видеобуфер
  void GetLineSize(uint32_t &linesize) const;//получить размер строки в пикселях
  void PutSymbol(int64_t x,int64_t y,char symbol,uint32_t color);//вывод символа в позицию
  void PutString(int64_t x,int64_t y,const char *string,uint32_t color);//вывод строчки в позицию
  void DrawPoint(int64_t x,int64_t y,uint32_t color);//рисование точки
  void DrawLine(int64_t x1,int64_t y1,int64_t x2,int64_t y2,uint32_t color,uint32_t thickness,LINE_TYPE line_type);//рисование линии
  void GetStringImageSize(const char *string,uint32_t &width,uint32_t &height);//получить размер строки в пикселях  
  void FillRectangle(int64_t x1,int64_t y1,int64_t x2,int64_t y2,uint32_t color);//нарисовать закрашенный прямоугольник
  void SetCursorCross(void);//сделать курсор внутри области графика крестиком
  void SetCursorMagnify(void);//сделать курсор внутри области графика лупой
  void ShowCursor(void);//показать курсор
  void HideCursor(void);//скрыть курсор
  void UnClipCursor(void);//отпустить курсор
  void ClipCursor(const CGrRect &cGrRect);//запереть курсор в области
  void SetCursorPos(int32_t x,int32_t y);//установить координаты курсора
  void SaveScreen(void);//сохранить изображение в память
  void RestoreScreen(void);//восстановить изображение из памяти
 private:
  //-закрытые функции-----------------------------------------------------------------------------------
};

#endif
