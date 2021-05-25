#ifndef C_GR_DATA_H
#define C_GR_DATA_H

//****************************************************************************************************
//класс хранения данных графиков
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "cgrcolor.h"
#include "cgrrect.h"
#include "cgrlinestyle.h"
#include "cgrpoint.h"

//****************************************************************************************************
//класс хранения данных графиков
//****************************************************************************************************

class CGrData
{
 private:
  //-переменные-----------------------------------------------------------------------------------------
  std::vector<CGrPoint> vector_CGrPoint;//массив исходных точек
  std::vector<CGrPoint> vector_CGrPointForView;//массив точек для отображения на экране
  CGrLineStyle cGrLineStyle;//стиль линии
  CGrColor cGrColor;//цвет линии
  bool Enable;//true-отрисовка разрешена
  std::string Name;//название графика
  long double CurrentPixelDensity;//текущая плотность пикселей
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CGrData(void);
  CGrData(const CGrData& cGrDataPtr);//конструктор копий
  //-деструктор-----------------------------------------------------------------------------------------
  ~CGrData();
 public:
  //-операторы------------------------------------------------------------------------------------------
  CGrData& operator=(const CGrData& cGrDataPtr);//операция присваивания  
  //-Открытые функции класса----------------------------------------------------------------------------  
  void SetEnable(bool enable);//установить разрешения отображения
  void SetName(const std::string &name);//установить название графика
  void SetGrColor(const CGrColor &cGrColor_Set);//установить цвет графика
  void SetGrLineStyle(const CGrLineStyle &cGrLineStyle_Set);//установить стиль линий графика

  bool GetEnable(void) const;//получить разрешение на отображение
  void GetName(std::string &name) const;//получить название графика
  void GetGrColor(CGrColor &cGrColor_Get) const;//получить цвет графика
  void GetGrLineStyle(CGrLineStyle &cGrLineStyle_Get) const;//получить стиль линий графика

  size_t GetPointAmount(void) const;//получить количество исходных точек графика
  void DeletePoints(void);//удалить все исходные точки графика

  double GetX(size_t index) const;//получить X точки графика
  double GetY(size_t index) const;//получить Y точки графика
  void SetX(size_t index,double value);//установить X точки графика
  void SetY(size_t index,double value);//установить Y точки графика
  void AddPoint(double x,double y);//добавить точку в конец графика
  std::vector<CGrPoint>& GetGrPointVector(void);//получить ссылку на вектор данных графиков

  double GetXForView(size_t index) const;//получить X точки графика для отображения на экране
  double GetYForView(size_t index) const;//получить Y точки графика для отображения на экране
  void SetViewProperty(const CGrRect &cGrRect_Coord,const CGrRect &cGrRect_Screen);//задать параметры отображения графика на экране
  size_t GetPointAmountForView(void) const;//получить количество точек графика для отображения на экране
 private:
  //-закрытые функции класса----------------------------------------------------------------------------
  void CreateForView(void);//создать вектор точек для отображения на экране
};

#endif


