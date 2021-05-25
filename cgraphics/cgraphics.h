#ifndef C_GRAPHICS_H
#define C_GRAPHICS_H

//****************************************************************************************************
//класс графиков
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <stdint.h>
#include <list>
#include <string>

#include "igraphics.h"
#include "ivideo.h"
#include "cgrdata.h"
#include "cgrrect.h"
#include "cgrmousepoint.h"
#include "ctranslator.h"

//****************************************************************************************************
//класс графиков
//****************************************************************************************************

class CGraphics:public IGraphics
{
 private:
  //-перечисления---------------------------------------------------------------------------------------
  enum SELECT_MODE
  {
   SELECT_MODE_NO_AREA,//области нет
   SELECT_MODE_SET_AREA,//область задаётся
   SELECT_MODE_AREA_IS_CREATED//область создана
  };
  //коды отсечения 
  enum CLIP_OUT_CODE
  {
   CLIP_OUT_CODE_NONE=0x00,//нет отсечения
   CLIP_OUT_CODE_X_X1=0x01,//x<x1
   CLIP_OUT_CODE_Y_Y1=0x02,//y<y1
   CLIP_OUT_CODE_X2_X=0x04,//x2<x
   CLIP_OUT_CODE_Y2_Y=0x08//y2<y
  };  
  //-константы------------------------------------------------------------------------------------------
  //-переменные-----------------------------------------------------------------------------------------
  IVideo *iVideo_Ptr;//указатель на класс работы с видео
  
  bool FirstRedraw;//создавать ли в памяти первичный образ графика
  CGrRect cGrRect_Select;//прямоугольник выделения (не скорректированный)  
  CGrRect cGrRect_CorrectSelect;//прямоугольник выделения (скорректированный)
  SELECT_MODE SelectMode;//режим работы
  bool EnableMagnify;//true-разрешено увеличение лупой
  bool EnableXValue;//true-разрешены значения по оси X 
  bool EnableYValue;//true-разрешены значения по оси Y 
  bool UserMode;//true-пользователь управляет графиком
  //цветовые настройки
  CGrColor cGrColor_BackGround;//цвет фонового поля
  CGrColor cGrColor_LegendBackGround;//цвет фонового поля легенды
  CGrColor cGrColor_XAxis;//цвет оси X
  CGrColor cGrColor_YAxis;//цвет оси Y
  CGrColor cGrColor_XGrid;//цвет сетки по X
  CGrColor cGrColor_YGrid;//цвет сетки по Y
  CGrColor cGrColor_SelectRectangle;//цвет прямоугольника выделения
  CGrColor cGrColor_XText;//цвет шрифта по X
  CGrColor cGrColor_YText;//цвет шрифта по Y
  CGrColor cGrColor_LegendText;//цвет легенды
  //настройки стиля линий
  CGrLineStyle cGrLineStyle_XAxis;//стиль линии оси X
  CGrLineStyle cGrLineStyle_YAxis;//стиль линии оси Y
  CGrLineStyle cGrLineStyle_XGrid;//стиль линии сетки по X
  CGrLineStyle cGrLineStyle_YGrid;//стиль линии сетки по Y
  //настройки системы координат
  CGrRect cGrRect_Based;
  //для масштаба 1:1 
  CGrRect cGrRect_Original;
  
  long double XGridStep;//шаг сетки по x
  long double YGridStep;//шаг сетки по y
  //название графика
  std::string Name;
  //управление перемещением по правой кнопке мыши 
  CGrMousePoint cGrMousePoint_RBMousePosition;//позиция мыши, в которой была нажата правая кнопка
  bool MoveMode;//true- режим перемещения
  bool InversionMoveMode;//true-режим перемещения графика относительно головы
  double TimeDivider;//временной делитель
  std::list<CGrData*> list_CGrDataPtr;//список указателей на графики
  CTranslator cTranslator;//класс перекодировки
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CGraphics(IVideo *iVideo_Ptr_Set);
  //-деструктор-----------------------------------------------------------------------------------------
  virtual ~CGraphics();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  CGrData* AddNewGraphic(void);//добавить график и получить указатель на созданный график
  void DeleteAllGraphics(void);//удалить все графики из памяти
  void DeleteGraphic(CGrData *cGrDataPtr);//удалить график из памяти
  void FindRectangle(CGrRect &cGrRect) const;//найти описывающий прямоугольник всех активных графиков
  void FindRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const;//найти описывающий прямоугольник всех активных графиков за последние points точек
  void FindRectangleOfEndTime(CGrRect &cGrRect,long double time) const;//найти описывающий прямоугольник всех активных графиков за последние time время
  void FindViewRectangle(CGrRect &cGrRect) const;//найти описывающий прямоугольник всех активных графиков с запасом по высоте
  void FindViewRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const;//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последние points точек
  void FindViewRectangleOfEndTime(CGrRect &cGrRect,long double time) const;//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последнее time время
  void SetTimeDivider(double value);//установить временной делитель
  double GetTimeDivider(void) const;//получить временной делитель
  //функции отображения
  void CancelSelect(void);//убрать выделение
  void Redraw(void);//перерисовать изображение
  void RedrawAll(void);//перерисовать всё
  void OnOriginalScale(void);//перейти в режим оригинального масштаба
  //функции цветовых настроек
  void SetBackGroundColor(const CGrColor &cGrColor);//задать цвет фона
  void SetLegendBackGroundColor(const CGrColor &cGrColor);//задать цвет фона легенды
  void SetAxisColor(const CGrColor &cGrColor_AxisX,const CGrColor &cGrColor_AxisY);//задать цвет осей
  void SetGridColor(const CGrColor &cGrColor_GridX,const CGrColor &cGrColor_GridY);//задать цвет сетки
  void SetSelectRectangleColor(const CGrColor &cGrColor);//задать цвет прямоугольника выделения
  void SetTextValueColor(const CGrColor &cGrColor_TextX,const CGrColor &cGrColor_TextY,const CGrColor &cGrColor_TextLegend);//задать цвет текста
  //функции настройки стиля линий
  void SetAxisLineStyle(const CGrLineStyle &cGrLineStyle_AxisX,const CGrLineStyle &cGrLineStyle_AxisY);//задать стиль осей
  void SetGridLineStyle(const CGrLineStyle &cGrLineStyle_GridX,const CGrLineStyle &cGrLineStyle_GridY);//задать стиль сетки
  //функции настройки системы координат
  void SetRectangle(const CGrRect &cGrRect);//задать прямоугольник системы координат
  void SetGridStep(long double step_x,long double step_y);//задать шаг сетки
  void GetRectangle(CGrRect &cGrRect) const;//получить прямоугольник системы координат
  void GetGridSize(long double &step_x,long double &step_y) const;//получить оптимальный шаг сетки
  //функции дополнительных настроек
  void SetEnableMagnify(bool enable);//разрешить увеличение
  void SetEnableValue(bool x_value,bool y_value);//разрешить отображение значений
  void SetOriginalScale(const CGrRect &cGrRect);//задать оригинальный масштаб
  void SetMoveMode(bool inversion);//задать режим инверсии мышки
  bool GetSelectedRectangle(CGrRect &cGrRect) const;//вернуть прямоугольник выделения
  void GetClientRectangle(CGrRect &cGrRect) const;//вернуть прямоугольник графика
  void SetName(const std::string &name);//установить название графика
  bool GetUserMode(void) const;//получить режим управления пользователем
  void SetUserMode(bool state);//установить режим управления пользователем
  //функция вызываемая при увеличении
  virtual void OnMagnify(void);//переопределяемая функция увеличения
  //сервисные функции
  bool ExportAsTGAFile(char &file_name) const;//экспорт графика в tga-файл
  
  void OnLButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//нажатие левой кнопкой мышки
  void OnRButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//нажатие правой кнопкой мышки
  void OnRButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//отпускание правой кнопки мышки
  void OnLButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//отпускание левой кнопки мышки
  void OnMouseMove(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//перемещение мышки
  void OnTimer(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button);//событие таймера
 private:  
  //-закрытые функции-----------------------------------------------------------------------------------
  void DrawAxisAndGrid(IVideo *iVideo_Ptr);//отрисовать оси и сетку
  void DrawGraphics(IVideo *iVideo_Ptr);//отрисовать граифики
  void DrawSelectRectangle(IVideo *iVideo_Ptr);//отрисовать прямоугольник выделения
  void DrawValue(IVideo *iVideo_Ptr);//отрисовать значения
  void DrawLegend(IVideo *iVideo_Ptr);//отрисовать легенду
  void GetClipCoord(int64_t &xa,int64_t &ya,int64_t &xb,int64_t &yb,int64_t x1,int64_t y1,int64_t x2,int64_t y2) const;//обрезать линию
  uint32_t ClipOutCode(int64_t x,int64_t y,int64_t x1,int64_t y1,int64_t x2,int64_t y2) const;//получить код для обрезания линии
  void GetLRIndex(const CGrData &cGrData,double x,int32_t &left_index,int32_t &right_index) const;//получить левый и правый индекс точек графика
};

#endif

