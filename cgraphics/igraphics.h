#ifndef I_GRAPHICS_H
#define I_GRAPHICS_H

//****************************************************************************************************
//класс интерфейса графиков
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <stdint.h>
#include <list>
#include <string>

#include "ivideo.h"
#include "cgrdata.h"
#include "cgrrect.h"
#include "cgrmousepoint.h"

//****************************************************************************************************
//класс интерфейса графиков
//****************************************************************************************************

class IGraphics
{
 public:
  //-конструктор----------------------------------------------------------------------------------------
  //-деструктор-----------------------------------------------------------------------------------------
  virtual ~IGraphics() {}
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  virtual CGrData* AddNewGraphic(void)=0;//добавить график и получить указатель на созданный график
  virtual void DeleteAllGraphics(void)=0;//удалить все графики из памяти
  virtual void DeleteGraphic(CGrData *cGrDataPtr)=0;//удалить график из памяти
  virtual void FindRectangle(CGrRect &cGrRect) const=0;//найти описывающий прямоугольник всех активных графиков
  virtual void FindRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const=0;//найти описывающий прямоугольник всех активных графиков за последние points точек
  virtual void FindRectangleOfEndTime(CGrRect &cGrRect,long double time) const=0;//найти описывающий прямоугольник всех активных графиков за последние time время
  virtual void FindViewRectangle(CGrRect &cGrRect) const=0;//найти описывающий прямоугольник всех активных графиков с запасом по высоте
  virtual void FindViewRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const=0;//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последние points точек
  virtual void FindViewRectangleOfEndTime(CGrRect &cGrRect,long double time) const=0;//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последнее time время
  virtual void SetTimeDivider(double value)=0;//установить временной делитель
  virtual double GetTimeDivider(void) const=0;//получить временной делитель
  //функции отображения
  virtual void CancelSelect(void)=0;//убрать выделение
  virtual void Redraw(void)=0;//перерисовать изображение
  virtual void RedrawAll(void)=0;//перерисовать всё
  virtual void OnOriginalScale(void)=0;//перейти в оежим оригинального масштаба
  //функции цветовых настроек
  virtual void SetBackGroundColor(const CGrColor &cGrColor)=0;//задать цвет фона
  virtual void SetLegendBackGroundColor(const CGrColor &cGrColor)=0;//задать цвет фона легенды
  virtual void SetAxisColor(const CGrColor &cGrColor_AxisX,const CGrColor &cGrColor_AxisY)=0;//задать цвет осей
  virtual void SetGridColor(const CGrColor &cGrColor_GridX,const CGrColor &cGrColor_GridY)=0;//задать цвет сетки
  virtual void SetSelectRectangleColor(const CGrColor &cGrColor)=0;//задать цвет прямоугольника выделения
  virtual void SetTextValueColor(const CGrColor &cGrColor_TextX,const CGrColor &cGrColor_TextY,const CGrColor &cGrColor_TextLegend)=0;//задать цвет текста
  //функции настройки стиля линий
  virtual void SetAxisLineStyle(const CGrLineStyle &cGrLineStyle_AxisX,const CGrLineStyle &cGrLineStyle_AxisY)=0;//задать стиль осей
  virtual void SetGridLineStyle(const CGrLineStyle &cGrLineStyle_GridX,const CGrLineStyle &cGrLineStyle_GridY)=0;//задать стиль сетки
  //функции настройки системы координат
  virtual void SetRectangle(const CGrRect &cGrRect)=0;//задать прямоугольник системы координат
  virtual void SetGridStep(long double step_x,long double step_y)=0;//задать шаг сетки
  virtual void GetRectangle(CGrRect &cGrRect) const=0;//получить прямоугольник системы координат
  virtual void GetGridSize(long double &step_x,long double &step_y) const=0;//получить оптимальный шаг сетки
  //функции дополнительных настроек
  virtual void SetEnableMagnify(bool enable)=0;//разрешить увеличение
  virtual void SetEnableValue(bool x_value,bool y_value)=0;//разрешить отображение значений
  virtual void SetOriginalScale(const CGrRect &cGrRect)=0;//задать оригинальный масштаб
  virtual void SetMoveMode(bool inversion)=0;//задать режим инверсии мышки
  virtual bool GetSelectedRectangle(CGrRect &cGrRect) const=0;//вернуть прямоугольник выделения
  virtual void GetClientRectangle(CGrRect &cGrRect) const=0;//вернуть прямоугольник графика
  virtual void SetName(const std::string &name)=0;//установить название графика
  virtual bool GetUserMode(void) const=0;//получить режим управления пользователем
  virtual void SetUserMode(bool state)=0;//установить режим управления пользователем
  //функция вызываемая при увеличении
  virtual void OnMagnify(void)=0;//переопределяемая функция увеличения
  //сервисные функции
  virtual bool ExportAsTGAFile(char &file_name) const=0;//экспорт графика в tga-файл
  
  virtual void OnLButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//нажатие левой кнопкой мышки
  virtual void OnRButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//нажатие правой кнопкой мышки
  virtual void OnRButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//отпускание правой кнопки мышки
  virtual void OnLButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//отпускание левой кнопки мышки
  virtual void OnMouseMove(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//перемещение мышки
  virtual void OnTimer(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)=0;//событие таймера
  //-статические функции--------------------------------------------------------------------------------
  static IGraphics* CreateNewCGraphics(IVideo *iVideo_Ptr);//создать график типа CGraphics
 private:
  //-закрытые функции-----------------------------------------------------------------------------------
};

#endif

