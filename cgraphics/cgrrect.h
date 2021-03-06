#ifndef C_GR_RECT_H
#define C_GR_RECT_H

//****************************************************************************************************
//класс прямоугольника графика
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <stdint.h>

//****************************************************************************************************
//класс прямоугольника графика
//****************************************************************************************************

class CGrRect
{
 public:
  //-переменные-----------------------------------------------------------------------------------------
  long double Left;//координаты
  long double Right; 	
  long double Top;
  long double Bottom;
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CGrRect(double left=0,double right=0,double top=0,double bottom=0);
  //-деструктор-----------------------------------------------------------------------------------------
  ~CGrRect();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  //-закрытые функции-----------------------------------------------------------------------------------
};

#endif

