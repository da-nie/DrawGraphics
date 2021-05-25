//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "cgraphics.h"
#include <math.h>

//****************************************************************************************************
//константы
//****************************************************************************************************
static const double EPS=0.00000001;//предел увеличения
static const double VIEW_SCALE=1.05;//коэффициент масштабирования вида

//****************************************************************************************************
//конструктор и деструктор
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CGraphics::CGraphics(IVideo *iVideo_Ptr_Set)
{
 iVideo_Ptr=iVideo_Ptr_Set;
	
 //установим значения по умолчанию
 //цвета 
 cGrColor_BackGround=CGrColor(16,16,64);
 cGrColor_LegendBackGround=CGrColor(16,16,48);

 cGrColor_XAxis=CGrColor(255,255,100);

 cGrColor_YAxis=CGrColor(255,255,100);

 cGrColor_XGrid=CGrColor(100,100,50);

 cGrColor_YGrid=CGrColor(100,100,50);

 cGrColor_SelectRectangle=CGrColor(0,255,255);

 cGrColor_XText=CGrColor(0,192,255);
 cGrColor_YText=CGrColor(0,192,255);
 
 cGrColor_LegendText=CGrColor(0,192,255);

 //стиль линий
 cGrLineStyle_XAxis=CGrLineStyle(IVideo::LINE_TYPE_SOLID,2,false,false);
 cGrLineStyle_YAxis=CGrLineStyle(IVideo::LINE_TYPE_SOLID,2,false,false);

 cGrLineStyle_XGrid=CGrLineStyle(IVideo::LINE_TYPE_DOT,1,false,false);
 cGrLineStyle_YGrid=CGrLineStyle(IVideo::LINE_TYPE_DOT,1,false,false);
 
 //система координат по-умолчанию
 cGrRect_Based=CGrRect(-10,10,10,-10);
 cGrRect_Original=cGrRect_Based;
 //шаг сетки
 XGridStep=2;
 YGridStep=2;
 //прочие параметры
 SelectMode=SELECT_MODE_NO_AREA;
 EnableMagnify=true;
 EnableXValue=true;
 EnableYValue=true;
 MoveMode=false;

 UserMode=false;
 FirstRedraw=true;
 InversionMoveMode=false;
 TimeDivider=1;

 Name="Unknow";
 
 cGrMousePoint_RBMousePosition=CGrMousePoint(0,0); 
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CGraphics::~CGraphics()
{  
 DeleteAllGraphics();
}
//****************************************************************************************************
//закрытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//отрисовать оси и сетку
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawAxisAndGrid(IVideo *iVideo_Ptr)
{
 CGrRect cGrRect_Client;
 GetClientRectangle(cGrRect_Client);	

 int32_t grid_xmin=(int32_t)(cGrRect_Based.Left/XGridStep)-1;
 int32_t grid_xmax=(int32_t)(cGrRect_Based.Right/XGridStep)+1;
 int32_t grid_ymin=(int32_t)(cGrRect_Based.Bottom/YGridStep)-1;
 int32_t grid_ymax=(int32_t)(cGrRect_Based.Top/YGridStep)+1;
 long double dsx=static_cast<long double>(cGrRect_Client.Right)/(cGrRect_Based.Right-cGrRect_Based.Left);
 long double dsy=static_cast<long double>(cGrRect_Client.Bottom)/(cGrRect_Based.Top-cGrRect_Based.Bottom);
 int32_t screen_xaxis=(int32_t)((0-cGrRect_Based.Left)*dsx);
 int32_t screen_yaxis=static_cast<int32_t>(cGrRect_Client.Bottom-(int32_t)((0-cGrRect_Based.Bottom)*dsy)-1);
 //рисуем сетку
 for(int32_t x=grid_xmin;x<grid_xmax;x++)
 {
  int32_t screen_x=(int32_t)(((long double)(x)*XGridStep-cGrRect_Based.Left)*dsx);
  if (screen_x<0 || screen_x>=cGrRect_Client.Right) continue;
  iVideo_Ptr->DrawLine(static_cast<int64_t>(screen_x),0,static_cast<int64_t>(screen_x),static_cast<int64_t>(cGrRect_Client.Bottom),cGrColor_XGrid.Color,cGrLineStyle_XGrid.Thickness,cGrLineStyle_XGrid.LineType);
 }
 for(int32_t y=grid_ymin;y<grid_ymax;y++)
 {
  int32_t screen_y=static_cast<int32_t>(cGrRect_Client.Bottom-(int32_t)(((long double)(y)*YGridStep-cGrRect_Based.Bottom)*dsy)-1);
  if (screen_y<0 || screen_y>=cGrRect_Client.Bottom) continue;
  iVideo_Ptr->DrawLine(0,static_cast<int64_t>(screen_y),static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(screen_y),cGrColor_YGrid.Color,cGrLineStyle_YGrid.Thickness,cGrLineStyle_YGrid.LineType);
 } 
 //рисуем нижнюю границу, отделяющую легенду от графика
 iVideo_Ptr->DrawLine(static_cast<int64_t>(cGrRect_Client.Left),static_cast<int64_t>(cGrRect_Client.Bottom),static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(cGrRect_Client.Bottom),cGrColor_XAxis.Color,cGrLineStyle_XAxis.Thickness,cGrLineStyle_XAxis.LineType);
 //рисуем оси
 if (screen_xaxis>=0 && screen_xaxis<cGrRect_Client.Right)
 {
  iVideo_Ptr->DrawLine(static_cast<int64_t>(screen_xaxis),0,static_cast<int64_t>(screen_xaxis),static_cast<int64_t>(cGrRect_Client.Bottom),cGrColor_XAxis.Color,cGrLineStyle_XAxis.Thickness,cGrLineStyle_XAxis.LineType);
 }
 if (screen_yaxis>=0 && screen_yaxis<cGrRect_Client.Bottom)
 {
  iVideo_Ptr->DrawLine(0,static_cast<int64_t>(screen_yaxis),static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(screen_yaxis),cGrColor_YAxis.Color,cGrLineStyle_YAxis.Thickness,cGrLineStyle_YAxis.LineType);
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовать граифики
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawGraphics(IVideo *iVideo_Ptr)
{
 CGrRect cGrRect_Client;//клиентская область окна графика
 GetClientRectangle(cGrRect_Client);
 long double dsx=static_cast<long double>((cGrRect_Client.Right))/(cGrRect_Based.Right-cGrRect_Based.Left);
 long double dsy=static_cast<long double>((cGrRect_Client.Bottom))/(cGrRect_Based.Top-cGrRect_Based.Bottom);

 long double step_x=(cGrRect_Based.Right-cGrRect_Based.Left)/(long double)(cGrRect_Client.Right-cGrRect_Client.Left);
 if (step_x<1) step_x=1;
 int level=(int)(log(static_cast<double>(step_x))/log(10.0));

 std::list<CGrData*>::iterator begin_of_list;
 std::list<CGrData*>::iterator end_of_list;
 std::list<CGrData*>::iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);
  if (cGrDataPtr->GetEnable()==false)//запрещено рисовать этот график
  {
   current++;
   continue;
  }  
  //задаём классу графиков параметры отображения, чтобы он подготовил изображение с нужной плотностью пикселей
  cGrDataPtr->SetViewProperty(cGrRect_Based,cGrRect_Client);

  CGrColor cGrColor;
  cGrDataPtr->GetGrColor(cGrColor);
  CGrLineStyle cGrLineStyle;
  cGrDataPtr->GetGrLineStyle(cGrLineStyle);
  //выясним, с каких точек рисовать этот график
  int32_t left_index_xmax;
  int32_t right_index_xmax;
  int32_t left_index_xmin;
  int32_t right_index_xmin;
  GetLRIndex(*cGrDataPtr,cGrRect_Based.Right,left_index_xmax,right_index_xmax);
  if (left_index_xmax<0)//график правее
  {
   current++;
   continue;
  }
  GetLRIndex(*cGrDataPtr,cGrRect_Based.Left,left_index_xmin,right_index_xmin);
  if (right_index_xmin<0)//график левее
  {
   current++;
   continue;
  }
  int32_t left_point=left_index_xmin;
  if (left_point<0) left_point=right_index_xmin;
  int32_t right_point=right_index_xmax;
  if (right_point<0) right_point=left_index_xmax;
  if (cGrLineStyle.AsPoint==true)//рисуем как набор точек
  {
   for(int32_t i=left_point;i<=right_point;i++)
   {
    CGrPoint cGrPoint(cGrDataPtr->GetXForView(i),cGrDataPtr->GetYForView(i));
    int64_t x=static_cast<int64_t>((cGrPoint.X-cGrRect_Based.Left)*dsx);
    int64_t y=static_cast<int64_t>(cGrRect_Client.Bottom-static_cast<int64_t>((cGrPoint.Y-cGrRect_Based.Bottom)*dsy)-1);
    if (x<0 || y<0) continue;
    if (x>=cGrRect_Client.Right || y>=cGrRect_Client.Bottom) continue;
	//рисуем точку
	for(int32_t dx=-1;dx<=1;dx++)
	{
 	 for(int32_t dy=-1;dy<=1;dy++) iVideo_Ptr->DrawPoint(x+dx,y+dy,cGrColor.Color);
	}
   }
  }
  else
  {
   for(int32_t i=left_point;i<right_point;i++)
   {
    CGrPoint cGrPoint_1(cGrDataPtr->GetXForView(i),cGrDataPtr->GetYForView(i));
    CGrPoint cGrPoint_2(cGrDataPtr->GetXForView(i+1),cGrDataPtr->GetYForView(i+1));
    int64_t x1=static_cast<int64_t>((cGrPoint_1.X-cGrRect_Based.Left)*dsx);
    int64_t x2=static_cast<int64_t>((cGrPoint_2.X-cGrRect_Based.Left)*dsx);
    int64_t y1=static_cast<int64_t>(cGrRect_Client.Bottom-static_cast<int64_t>((cGrPoint_1.Y-cGrRect_Based.Bottom)*dsy)-1);
    int64_t y2=static_cast<int64_t>(cGrRect_Client.Bottom-static_cast<int64_t>((cGrPoint_2.Y-cGrRect_Based.Bottom)*dsy)-1);
    if ((x1<0 && x2<0) || (y1<0 && y2<0)) continue;
    if ((x1>=cGrRect_Client.Right && x2>=cGrRect_Client.Right) || (y1>=cGrRect_Client.Bottom && y2>=cGrRect_Client.Bottom)) continue;
    //используем алгоритм Сазерленда-Кохена для отсечения прямой по экрану
	if (cGrPoint_2.X-cGrPoint_1.X>1 && cGrLineStyle.AsStuff==true)//соединяем ступенькой
	{
     int64_t xs1=x1;
	 int64_t xs2=x2;
	 int64_t ys1=y1;
	 int64_t ys2=y1;
     GetClipCoord(xs1,ys1,xs2,ys2,0,0,static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(cGrRect_Client.Bottom));
     iVideo_Ptr->DrawLine(xs1,ys1,xs2,ys2,cGrColor.Color,cGrLineStyle.Thickness,cGrLineStyle.LineType);   	
     xs1=x2;
	 xs2=x2;
	 ys1=y1;
	 ys2=y2;
     GetClipCoord(xs1,ys1,xs2,ys2,0,0,static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(cGrRect_Client.Bottom));
     iVideo_Ptr->DrawLine(xs1,ys1,xs2,ys2,cGrColor.Color,cGrLineStyle.Thickness,cGrLineStyle.LineType);   	
	}
	else
	{
     GetClipCoord(x1,y1,x2,y2,0,0,static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(cGrRect_Client.Bottom));
     iVideo_Ptr->DrawLine(x1,y1,x2,y2,cGrColor.Color,cGrLineStyle.Thickness,cGrLineStyle.LineType);   	
	}
   }
  }
  current++;
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовать прямоугольник выделения
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawSelectRectangle(IVideo *iVideo_Ptr)
{
 if (SelectMode==SELECT_MODE_SET_AREA || SelectMode==SELECT_MODE_AREA_IS_CREATED)
 {
  iVideo_Ptr->DrawLine(static_cast<int64_t>(cGrRect_Select.Left),static_cast<int64_t>(cGrRect_Select.Top),static_cast<int64_t>(cGrRect_Select.Right),static_cast<int64_t>(cGrRect_Select.Top),cGrColor_SelectRectangle.Color,1,IVideo::LINE_TYPE_DOT);
  iVideo_Ptr->DrawLine(static_cast<int64_t>(cGrRect_Select.Right),static_cast<int64_t>(cGrRect_Select.Top),static_cast<int64_t>(cGrRect_Select.Right),static_cast<int64_t>(cGrRect_Select.Bottom),cGrColor_SelectRectangle.Color,1,IVideo::LINE_TYPE_DOT);
  iVideo_Ptr->DrawLine(static_cast<int64_t>(cGrRect_Select.Left),static_cast<int64_t>(cGrRect_Select.Bottom),static_cast<int64_t>(cGrRect_Select.Right),static_cast<int64_t>(cGrRect_Select.Bottom),cGrColor_SelectRectangle.Color,1,IVideo::LINE_TYPE_DOT);
  iVideo_Ptr->DrawLine(static_cast<int64_t>(cGrRect_Select.Left),static_cast<int64_t>(cGrRect_Select.Top),static_cast<int64_t>(cGrRect_Select.Left),static_cast<int64_t>(cGrRect_Select.Bottom),cGrColor_SelectRectangle.Color,1,IVideo::LINE_TYPE_DOT);
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовать значения
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawValue(IVideo *iVideo_Ptr)
{
 CGrRect cGrRect_Client;
 GetClientRectangle(cGrRect_Client);
 
 static const size_t STRING_BUFFER_SIZE=255;//размер буфера для строк

 char string[STRING_BUFFER_SIZE];
 
 int32_t grid_xmin=static_cast<int32_t>(cGrRect_Based.Left/XGridStep)-1;
 int32_t grid_xmax=static_cast<int32_t>(cGrRect_Based.Right/XGridStep)+1;
 int32_t grid_ymin=static_cast<int32_t>(cGrRect_Based.Bottom/YGridStep)-1;
 int32_t grid_ymax=static_cast<int32_t>(cGrRect_Based.Top/YGridStep)+1;
 long double dsx=static_cast<long double>(cGrRect_Client.Right)/(cGrRect_Based.Right-cGrRect_Based.Left);
 long double dsy=static_cast<long double>(cGrRect_Client.Bottom)/(cGrRect_Based.Top-cGrRect_Based.Bottom);
 
 //текст по оси X
 if (EnableXValue==true)
 {
  for(int32_t x=grid_xmin;x<grid_xmax;x++)
  {
   int32_t screen_x=static_cast<int32_t>((static_cast<long double>(x)*XGridStep-cGrRect_Based.Left)*dsx);
   float v=static_cast<float>(static_cast<long double>(x)*XGridStep/TimeDivider);
   //snprintf(string,STRING_BUFFER_SIZE,"%g",v);
   sprintf(string,"%g",v);
   uint32_t str_width;
   uint32_t str_height;   
   iVideo_Ptr->GetStringImageSize(string,str_width,str_height);   
   if (screen_x+static_cast<int32_t>(str_width)/2<0 || screen_x-static_cast<int32_t>(str_width)/2>=cGrRect_Client.Right) continue;
   iVideo_Ptr->PutString(static_cast<int64_t>(screen_x-static_cast<int64_t>(str_width)/2),static_cast<int64_t>(cGrRect_Client.Bottom-static_cast<int32_t>(str_height)),string,cGrColor_XText.Color);
  }
 }
 //текст по оси Y
 if (EnableYValue==true)
 {
  for(int32_t y=grid_ymin;y<grid_ymax;y++)
  {
   int32_t screen_y=static_cast<int32_t>(cGrRect_Client.Bottom)-static_cast<int32_t>((static_cast<long double>(y)*YGridStep-cGrRect_Based.Bottom)*dsy)-1;
   float v=static_cast<float>(static_cast<long double>(y)*YGridStep);
  //snprintf(string,STRING_BUFFER_SIZE,"%g",v);
   sprintf(string,"%g",v);
   uint32_t str_width;
   uint32_t str_height;
   iVideo_Ptr->GetStringImageSize(string,str_width,str_height);   
   if (screen_y+static_cast<int32_t>(str_height)<0 || screen_y-static_cast<int32_t>(str_height)>=cGrRect_Client.Bottom) continue;
   iVideo_Ptr->PutString(static_cast<int64_t>(cGrRect_Client.Left),static_cast<int64_t>(screen_y-static_cast<int64_t>(str_height)),string,cGrColor_YText.Color);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовать легенду
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawLegend(IVideo *iVideo_Ptr)
{
 CGrRect cGrRect_Client;
 GetClientRectangle(cGrRect_Client);	
	
 static const int32_t LEGEND_X_OFFSET=5;//отступ по X
 static const int32_t LEGEND_Y_OFFSET=5;//отступ по Y
 static const int32_t LEGEND_X_SPACE=2;//промежуток по x

 //возьмём область экрана	
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);	
 //очищаем поле легенды
 iVideo_Ptr->FillRectangle(0,static_cast<int64_t>(cGrRect_Client.Bottom),static_cast<int64_t>(width),static_cast<int64_t>(height),cGrColor_LegendBackGround.Color);

 //выводим легенду
 int32_t x_item_pos=LEGEND_X_OFFSET;
 
 std::list<CGrData*>::iterator begin_of_list;
 std::list<CGrData*>::iterator end_of_list;
 std::list<CGrData*>::iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);  
  if (cGrDataPtr->GetEnable()==false)//запрещено рисовать этот график
  {
   current++;
   continue;
  }
  //узнаем размер текста
  uint32_t str_width;
  uint32_t str_height;
  std::string name;
  cGrDataPtr->GetName(name);
  iVideo_Ptr->GetStringImageSize(cTranslator.Convert(name.c_str()).c_str(),str_width,str_height);
  //закрашенный прямоугольник
  int64_t x1=x_item_pos;
  int64_t x2=x1+static_cast<int64_t>(str_height);
  int64_t y1=static_cast<int64_t>(cGrRect_Client.Bottom)+LEGEND_Y_OFFSET;
  int64_t y2=y1+static_cast<int64_t>(str_height);
  CGrColor cGrColor;
  cGrDataPtr->GetGrColor(cGrColor);
  iVideo_Ptr->FillRectangle(x1,y1,x2,y2,cGrColor.Color);
  
  x_item_pos+=static_cast<int32_t>(str_height)+LEGEND_X_SPACE;
  //название графика  
  iVideo_Ptr->PutString(static_cast<int64_t>(x_item_pos),static_cast<int64_t>(cGrRect_Client.Bottom+LEGEND_Y_OFFSET),cTranslator.Convert(name.c_str()).c_str(),cGrColor_LegendText.Color);
  x_item_pos+=static_cast<int32_t>(str_width)+LEGEND_X_OFFSET;
  current++;
 }
 //узнаем размер любой букав текста
 uint32_t str_width;
 uint32_t str_height;
 iVideo_Ptr->GetStringImageSize("A",str_width,str_height);
 //название графика
 iVideo_Ptr->PutString(LEGEND_X_OFFSET,cGrRect_Client.Bottom+7+static_cast<int32_t>(str_height),cTranslator.Convert(Name.c_str()).c_str(),cGrColor_LegendText.Color);
}
//----------------------------------------------------------------------------------------------------
//обрезать линию
//----------------------------------------------------------------------------------------------------
void CGraphics::GetClipCoord(int64_t &xa,int64_t &ya,int64_t &xb,int64_t &yb,int64_t x1,int64_t y1,int64_t x2,int64_t y2) const
{
 int64_t new_x1=xa;
 int64_t new_y1=ya;
 int64_t new_x2=xb;
 int64_t new_y2=yb; 
 int64_t code1=ClipOutCode(new_x1,new_y1,x1,y1,x2,y2);
 int64_t code2=ClipOutCode(new_x2,new_y2,x1,y1,x2,y2);
 int64_t inside=((code1|code2)==0);
 int64_t outside=((code1&code2)!=0);
 while (!outside && !inside)
 {
  if (code1==CLIP_OUT_CODE_NONE)
  {
   int64_t temporary=code1;
   code1=code2;
   code2=temporary;
   temporary=new_x1;
   new_x1=new_x2;
   new_x2=temporary;
   temporary=new_y1;
   new_y1=new_y2;
   new_y2=temporary;
  }
  if (code1&CLIP_OUT_CODE_X_X1)
  {
   new_y1+=static_cast<int64_t>(static_cast<long double>((new_y2-new_y1)*(x1-new_x1))/(static_cast<long double>(new_x2-new_x1)));
   new_x1=x1;
  }
  else
  {
   if (code1&CLIP_OUT_CODE_Y_Y1)
   { 
    new_x1+=static_cast<int64_t>(static_cast<long double>((new_x2-new_x1)*(y1-new_y1))/(static_cast<long double>(new_y2-new_y1)));
    new_y1=y1;
   }
   else
   {
    if (code1&CLIP_OUT_CODE_X2_X)
    {
     new_y1+=static_cast<int64_t>(static_cast<long double>((new_y2-new_y1)*(x2-new_x1))/(static_cast<long double>(new_x2-new_x1)));
     new_x1=x2;
	}
	else
	{
     if (code1&CLIP_OUT_CODE_Y2_Y)
	 { 
      new_x1+=static_cast<int64_t>(static_cast<long double>((new_x2-new_x1)*(y2-new_y1))/(static_cast<long double>(new_y2-new_y1)));
      new_y1=y2;
	 }
	}
   }
  }
  code1=ClipOutCode(new_x1,new_y1,x1,y1,x2,y2);
  code2=ClipOutCode(new_x2,new_y2,x1,y1,x2,y2);
  inside=((code1|code2)==0);
  outside=((code1&code2)!=0);
 }
 xa=new_x1;
 xb=new_x2;
 ya=new_y1;
 yb=new_y2;	
}
//----------------------------------------------------------------------------------------------------
//получить код для обрезания линии
//----------------------------------------------------------------------------------------------------
uint32_t CGraphics::ClipOutCode(int64_t x,int64_t y,int64_t x1,int64_t y1,int64_t x2,int64_t y2) const
{
 uint32_t code=CLIP_OUT_CODE_NONE;
 if (x<x1) code|=CLIP_OUT_CODE_X_X1;
 if (y<y1) code|=CLIP_OUT_CODE_Y_Y1;
 if (x>x2) code|=CLIP_OUT_CODE_X2_X;
 if (y>y2) code|=CLIP_OUT_CODE_Y2_Y;
 return(code);
}
//----------------------------------------------------------------------------------------------------
//получить левый и правый индекс точек графика
//----------------------------------------------------------------------------------------------------
void CGraphics::GetLRIndex(const CGrData &cGrData,double x,int32_t &left_index,int32_t &right_index) const
{
 size_t amount=cGrData.GetPointAmountForView();
 left_index=-1;
 right_index=-1;
 if (amount==0) return;//нет точек
 long double left=cGrData.GetXForView(0);
 long double right=cGrData.GetXForView(amount-1);
 if (x<=left)
 {
  left_index=-1;//левее точек нет
  right_index=0;
  return;
 }
 if (x>=right)
 {
  left_index=amount-1;
  right_index=-1;//правее точек нет
  return;
 }
 //x находится посередине между левой и правой границами
 left_index=0;
 right_index=amount-1;
 while(1)
 {
  int32_t center_index=(left_index+right_index)/2;//центральная точка
  if (center_index==left_index || center_index==right_index) break;
  long double xc=cGrData.GetXForView(center_index);
  if (x<xc)
  {
   right_index=center_index;//переносим правую точку
   continue;
  }
  if (x>xc)
  {
   left_index=center_index;//переносим левую точку
   continue;
  }
  break;  
 }
}


//****************************************************************************************************
//открытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//добавить график и получить указатель на созданный график
//----------------------------------------------------------------------------------------------------
CGrData* CGraphics::AddNewGraphic(void)
{
 CGrData *cGrDataPtr=new CGrData;
 cGrDataPtr->SetEnable(false);
 cGrDataPtr->SetName("Unknow");
 cGrDataPtr->SetGrColor(CGrColor(0,0,0));
 cGrDataPtr->SetGrLineStyle(CGrLineStyle(IVideo::LINE_TYPE_SOLID,1,false,false));
 //добавляем график в список
 list_CGrDataPtr.push_back(cGrDataPtr);
 //возвращаем указатель на добавленный график
 return(cGrDataPtr);	
}
//----------------------------------------------------------------------------------------------------
//удалить все графики из памяти
//----------------------------------------------------------------------------------------------------
void CGraphics::DeleteAllGraphics(void)
{
 std::list<CGrData*>::iterator begin_of_list;
 std::list<CGrData*>::iterator end_of_list;
 std::list<CGrData*>::iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);
  delete(cGrDataPtr);
  current++;
 }
 list_CGrDataPtr.clear();
}
//----------------------------------------------------------------------------------------------------
//удалить график из памяти
//----------------------------------------------------------------------------------------------------
void CGraphics::DeleteGraphic(CGrData *cGrDataPtr)
{
 std::list<CGrData*>::iterator begin_of_list;
 std::list<CGrData*>::iterator end_of_list;
 std::list<CGrData*>::iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtrL=*(current);
  if (cGrDataPtr==cGrDataPtrL)
  {
   std::list<CGrData*>::iterator next=current;
   next++;
   list_CGrDataPtr.erase(current,next);
   delete(cGrDataPtrL);
   break;
  }
  current++;
 }
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков
//----------------------------------------------------------------------------------------------------
void CGraphics::FindRectangle(CGrRect &cGrRect) const
{
 cGrRect=CGrRect(0,0,0,0);

 bool first=true;
 std::list<CGrData*>::const_iterator begin_of_list;
 std::list<CGrData*>::const_iterator end_of_list;
 std::list<CGrData*>::const_iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);  
  if (cGrDataPtr->GetPointAmount()>0 && cGrDataPtr->GetEnable()==true)
  {
   //ищем минимальное и максимальное значения
   long double left=0;
   long double right=0;
   long double bottom=0;
   long double top=0;
   bool oncopy=true;
   size_t amount=cGrDataPtr->GetPointAmount();
   for(size_t n=0;n<amount;n++)
   {
    long double x=cGrDataPtr->GetX(n);
    long double y=cGrDataPtr->GetY(n);
	if (oncopy==true)
	{
     left=x;
	 right=x;
	 top=y;
	 bottom=y;
	 oncopy=false;
	}
    if (x<left) left=x;
    if (x>right) right=x;
    if (y<bottom) bottom=y;
    if (y>top) top=y;
   }
   if (first==true)
   {
    first=false;
    cGrRect=CGrRect(left,right,bottom,top);    
   }
   if (left<cGrRect.Left) cGrRect.Left=left;
   if (right>cGrRect.Right) cGrRect.Right=right;
   if (bottom<cGrRect.Bottom) cGrRect.Bottom=bottom;
   if (top>cGrRect.Top) cGrRect.Top=top;
  }
  current++;
 }
 if (fabs(cGrRect.Right-cGrRect.Left)<EPS) cGrRect.Right+=1;
 if (fabs(cGrRect.Top-cGrRect.Bottom)<EPS) cGrRect.Top+=1;	
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков за последние points точек
//----------------------------------------------------------------------------------------------------
void CGraphics::FindRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const
{
 cGrRect=CGrRect(0,0,0,0);

 bool first=true;
 std::list<CGrData*>::const_iterator begin_of_list;
 std::list<CGrData*>::const_iterator end_of_list;
 std::list<CGrData*>::const_iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);  
  if (cGrDataPtr->GetPointAmount()>0 && cGrDataPtr->GetEnable()==true)
  {
   //ищем минимальное и максимальное значения
   long double left=0;
   long double right=0;
   long double bottom=0;
   long double top=0;
   bool oncopy=true;
   size_t amount=cGrDataPtr->GetPointAmount();
   size_t begin;
   if (points>amount) begin=0;
                 else begin=amount-points;
   for(size_t n=begin;n<amount;n++)
   {
    long double x=cGrDataPtr->GetX(n);
    long double y=cGrDataPtr->GetY(n);
	if (oncopy==true)
	{
     left=x;
	 right=x;
	 top=y;
	 bottom=y;
	 oncopy=false;
	}
    if (x<left) left=x;
    if (x>right) right=x;
    if (y<bottom) bottom=y;
    if (y>top) top=y;
   }
   if (first==true)
   {
    first=false;
    cGrRect=CGrRect(left,right,bottom,top);    
   }
   if (left<cGrRect.Left) cGrRect.Left=left;
   if (right>cGrRect.Right) cGrRect.Right=right;
   if (bottom<cGrRect.Bottom) cGrRect.Bottom=bottom;
   if (top>cGrRect.Top) cGrRect.Top=top;
  }
  current++;
 }
 if (fabs(cGrRect.Right-cGrRect.Left)<EPS) cGrRect.Right+=1;
 if (fabs(cGrRect.Top-cGrRect.Bottom)<EPS) cGrRect.Top+=1;
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков за последние time время
//----------------------------------------------------------------------------------------------------
void CGraphics::FindRectangleOfEndTime(CGrRect &cGrRect,long double time) const
{
 cGrRect=CGrRect(0,0,0,0);

 bool first=true;
 std::list<CGrData*>::const_iterator begin_of_list;
 std::list<CGrData*>::const_iterator end_of_list;
 std::list<CGrData*>::const_iterator current;
 begin_of_list=list_CGrDataPtr.begin();
 end_of_list=list_CGrDataPtr.end();
 current=begin_of_list;
 while(current!=end_of_list)
 {
  CGrData *cGrDataPtr=*(current);  
  if (cGrDataPtr->GetPointAmount()>0 && cGrDataPtr->GetEnable()==true)
  {
   //ищем минимальное и максимальное значения
   long double left=0;
   long double right=0;
   long double bottom=0;
   long double top=0;
   bool oncopy=true;
   size_t amount=cGrDataPtr->GetPointAmount();
   long double et=cGrDataPtr->GetX(amount-1);
   for(size_t n=0;n<amount;n++)
   {
    long double x=cGrDataPtr->GetX(n);
    long double y=cGrDataPtr->GetY(n);
	if (x<et-time) continue;
	if (oncopy==true)
	{
     left=x;
	 right=x;
	 top=y;
	 bottom=y;
	 oncopy=false;
	}
    if (x<left) left=x;
    if (x>right) right=x;
    if (y<bottom) bottom=y;
    if (y>top) top=y;
   }
   if (first==true)
   {
    first=false;
    cGrRect=CGrRect(left,right,bottom,top);    
   }
   if (left<cGrRect.Left) cGrRect.Left=left;
   if (right>cGrRect.Right) cGrRect.Right=right;
   if (bottom<cGrRect.Bottom) cGrRect.Bottom=bottom;
   if (top>cGrRect.Top) cGrRect.Top=top;
  }
  current++;
 }
 if (fabs(cGrRect.Right-cGrRect.Left)<EPS) cGrRect.Right+=1;
 if (fabs(cGrRect.Top-cGrRect.Bottom)<EPS) cGrRect.Top+=1;
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков с запасом по высоте
//----------------------------------------------------------------------------------------------------
void CGraphics::FindViewRectangle(CGrRect &cGrRect) const
{
 FindRectangle(cGrRect);
 long double center=(cGrRect.Top+cGrRect.Bottom)/2.0;
 long double height=cGrRect.Top-center;
 height*=VIEW_SCALE;
 cGrRect.Top=center+height;
 cGrRect.Bottom=center-height;
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последние points точек
//----------------------------------------------------------------------------------------------------
void CGraphics::FindViewRectangleOfEndPoints(CGrRect &cGrRect,size_t points) const
{
 FindRectangleOfEndPoints(cGrRect,points);
 long double center=(cGrRect.Top+cGrRect.Bottom)/2.0;
 long double height=cGrRect.Top-center;
 height*=VIEW_SCALE;
 cGrRect.Top=center+height;
 cGrRect.Bottom=center-height;
}
//----------------------------------------------------------------------------------------------------
//найти описывающий прямоугольник всех активных графиков с запасом по высоте за последнее time время
//----------------------------------------------------------------------------------------------------
void CGraphics::FindViewRectangleOfEndTime(CGrRect &cGrRect,long double time) const
{
 FindRectangleOfEndTime(cGrRect,time);
 long double center=(cGrRect.Top+cGrRect.Bottom)/2.0;
 long double height=cGrRect.Top-center;
 height*=VIEW_SCALE;
 cGrRect.Top=center+height;
 cGrRect.Bottom=center-height;
}
//----------------------------------------------------------------------------------------------------
//установить временной делитель
//----------------------------------------------------------------------------------------------------
void CGraphics::SetTimeDivider(double value)
{
 if (value==0) value=1;
 TimeDivider=value;
 OnMagnify();
}
//----------------------------------------------------------------------------------------------------
//получить временной делитель
//----------------------------------------------------------------------------------------------------
double CGraphics::GetTimeDivider(void) const
{
 return(TimeDivider);
}

//----------------------------------------------------------------------------------------------------
//убрать выделение
//----------------------------------------------------------------------------------------------------
void CGraphics::CancelSelect(void)
{
 SelectMode=SELECT_MODE_NO_AREA;
 Redraw();
}
//----------------------------------------------------------------------------------------------------
//перерисовать изображение
//----------------------------------------------------------------------------------------------------
void CGraphics::Redraw(void)
{
 if (FirstRedraw==true)//требуется обновление внеэкранного контекста
 {
  //возьмём область экрана	
  CGrRect cGrRect_Client;
  GetClientRectangle(cGrRect_Client);	
  //очищаем поле легенды
  iVideo_Ptr->FillRectangle(0,0,static_cast<int64_t>(cGrRect_Client.Right),static_cast<int64_t>(cGrRect_Client.Bottom),cGrColor_BackGround.Color);
 /* uint32_t width;
  uint32_t height;
  iVideo_Ptr->GetClientRectangle(width,height);	
  iVideo_Ptr->FillRectangle(0,0,width,height,cGrColor_BackGround.Color);
  */
  //рисуем всё, что требуется
  DrawAxisAndGrid(iVideo_Ptr);//оси и сетка
  DrawGraphics(iVideo_Ptr);//все графики
  DrawValue(iVideo_Ptr);//значения
  FirstRedraw=false;
  iVideo_Ptr->SaveScreen();
 }
 iVideo_Ptr->RestoreScreen();
 DrawSelectRectangle(iVideo_Ptr);//прямоугольник выделения
 DrawLegend(iVideo_Ptr);//легенда
}
//----------------------------------------------------------------------------------------------------
//перерисовать всё
//----------------------------------------------------------------------------------------------------
void CGraphics::RedrawAll(void)
{
 FirstRedraw=true;
 Redraw();
}
//----------------------------------------------------------------------------------------------------
//перейти в оригинальный режим
//----------------------------------------------------------------------------------------------------
void CGraphics::OnOriginalScale(void)
{
 CancelSelect();
 SetRectangle(cGrRect_Original);
 OnMagnify();
 UserMode=false;
}

//----------------------------------------------------------------------------------------------------
//задать цвет фона
//----------------------------------------------------------------------------------------------------
void CGraphics::SetBackGroundColor(const CGrColor &cGrColor)
{
 cGrColor_BackGround=cGrColor;	
}
//----------------------------------------------------------------------------------------------------
//задать цвет фона легенды
//----------------------------------------------------------------------------------------------------
void CGraphics::SetLegendBackGroundColor(const CGrColor &cGrColor)
{
 cGrColor_LegendBackGround=cGrColor;	
}
//----------------------------------------------------------------------------------------------------
//задать цвет осей
//----------------------------------------------------------------------------------------------------
void CGraphics::SetAxisColor(const CGrColor &cGrColor_AxisX,const CGrColor &cGrColor_AxisY)
{
 cGrColor_XAxis=cGrColor_AxisX;
 cGrColor_YAxis=cGrColor_AxisY;	
}
//----------------------------------------------------------------------------------------------------
//задать цвет сетки
//----------------------------------------------------------------------------------------------------
void CGraphics::SetGridColor(const CGrColor &cGrColor_GridX,const CGrColor &cGrColor_GridY)
{
 cGrColor_XGrid=cGrColor_GridX;
 cGrColor_YGrid=cGrColor_GridY;
}
//----------------------------------------------------------------------------------------------------
//задать цвет прямоугольника выделения
//----------------------------------------------------------------------------------------------------
void CGraphics::SetSelectRectangleColor(const CGrColor &cGrColor)
{
 cGrColor_SelectRectangle=cGrColor;
}
//----------------------------------------------------------------------------------------------------
//задать цвет текста
//----------------------------------------------------------------------------------------------------
void CGraphics::SetTextValueColor(const CGrColor &cGrColor_TextX,const CGrColor &cGrColor_TextY,const CGrColor &cGrColor_TextLegend)
{
 cGrColor_XText=cGrColor_TextX;
 cGrColor_YText=cGrColor_TextY;
 cGrColor_LegendText=cGrColor_TextLegend;
}

//----------------------------------------------------------------------------------------------------
//задать стиль осей
//----------------------------------------------------------------------------------------------------
void CGraphics::SetAxisLineStyle(const CGrLineStyle &cGrLineStyle_AxisX,const CGrLineStyle &cGrLineStyle_AxisY)
{
 cGrLineStyle_XAxis=cGrLineStyle_AxisX;
 cGrLineStyle_YAxis=cGrLineStyle_AxisY;
}
//----------------------------------------------------------------------------------------------------
//задать стиль сетки
//----------------------------------------------------------------------------------------------------
void CGraphics::SetGridLineStyle(const CGrLineStyle &cGrLineStyle_GridX,const CGrLineStyle &cGrLineStyle_GridY)
{
 cGrLineStyle_XGrid=cGrLineStyle_GridX;
 cGrLineStyle_YGrid=cGrLineStyle_GridY;
}

//----------------------------------------------------------------------------------------------------
//задать прямоугольник системы координат
//----------------------------------------------------------------------------------------------------
void CGraphics::SetRectangle(const CGrRect &cGrRect)
{
 cGrRect_Based=cGrRect;
}
//----------------------------------------------------------------------------------------------------
//задать шаг сетки
//----------------------------------------------------------------------------------------------------
void CGraphics::SetGridStep(long double step_x,long double step_y)
{
 XGridStep=step_x;
 YGridStep=step_y;
}
//----------------------------------------------------------------------------------------------------
//получить прямоугольник системы координат
//----------------------------------------------------------------------------------------------------
void CGraphics::GetRectangle(CGrRect &cGrRect) const
{
 cGrRect=cGrRect_Based;	
}
//----------------------------------------------------------------------------------------------------
//получить оптимальный шаг сетки
//----------------------------------------------------------------------------------------------------
void CGraphics::GetGridSize(long double &step_x,long double &step_y) const
{
 step_x=XGridStep;
 step_y=YGridStep;	
}
//----------------------------------------------------------------------------------------------------
//разрешить увеличение
//----------------------------------------------------------------------------------------------------
void CGraphics::SetEnableMagnify(bool enable)
{
 EnableMagnify=enable;	
}
//----------------------------------------------------------------------------------------------------
//разрешить отображение значений
//----------------------------------------------------------------------------------------------------
void CGraphics::SetEnableValue(bool x_value,bool y_value)
{
 EnableXValue=x_value;
 EnableYValue=y_value;
}
//----------------------------------------------------------------------------------------------------
//задать оригинальный масштаб
//----------------------------------------------------------------------------------------------------
void CGraphics::SetOriginalScale(const CGrRect &cGrRect)
{
 cGrRect_Original=cGrRect;
}
//----------------------------------------------------------------------------------------------------
//задать режим инверсии мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::SetMoveMode(bool inversion)
{
 InversionMoveMode=inversion;
}
//----------------------------------------------------------------------------------------------------
//вернуть прямоугольник выделения
//----------------------------------------------------------------------------------------------------
bool CGraphics::GetSelectedRectangle(CGrRect &cGrRect) const
{
 if (SelectMode==SELECT_MODE_SET_AREA || SelectMode==SELECT_MODE_AREA_IS_CREATED)
 {
  cGrRect=cGrRect_Select;
  if (cGrRect.Left>cGrRect.Right)
  {
   cGrRect.Left=cGrRect_Select.Right;
   cGrRect.Right=cGrRect_Select.Left;
  }
  if (cGrRect.Top>cGrRect.Bottom)
  {
   cGrRect.Top=cGrRect_Select.Bottom;
   cGrRect.Bottom=cGrRect_Select.Top;
  }
  return(true);
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//вернуть прямоугольник графика
//----------------------------------------------------------------------------------------------------
void CGraphics::GetClientRectangle(CGrRect &cGrRect) const
{
 //возьмём область экрана	
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);
 //вычтем из неё размер легенды
 uint32_t str_width;	
 uint32_t str_height;
 iVideo_Ptr->GetStringImageSize("A",str_width,str_height);
 height-=str_height*2;
 height-=10;
 cGrRect=CGrRect(0,width,0,height);
}
//----------------------------------------------------------------------------------------------------
//установить название графика
//----------------------------------------------------------------------------------------------------
void CGraphics::SetName(const std::string &name)
{
 Name=name;	
}
//----------------------------------------------------------------------------------------------------
//получить режим управления пользователем
//----------------------------------------------------------------------------------------------------
bool CGraphics::GetUserMode(void) const
{
 return(UserMode);	
}
//----------------------------------------------------------------------------------------------------
//установить режим управления пользователем
//----------------------------------------------------------------------------------------------------
void CGraphics::SetUserMode(bool state)
{
 UserMode=state;	
}
//----------------------------------------------------------------------------------------------------
//функция увеличения
//----------------------------------------------------------------------------------------------------
void CGraphics::OnMagnify(void)
{
 CGrRect cGrRect;	
 GetRectangle(cGrRect);
 //приведём масштаб к "нормальному" виду
 static const int32_t STEP_5=5;//шаг на 5 единиц
 static const int32_t STEP_2=2;//шаг на 2 единицы
 static const double POW_SCALE=10;//шаг перехода между шкалами
 static const int32_t INITIAL_STEP=5;//нашальный шаг по x и y

 size_t MinTicksX=INITIAL_STEP;
 long double grossStepX=(cGrRect.Right-cGrRect.Left)/MinTicksX;
 grossStepX/=TimeDivider;

 long double stepx=pow(POW_SCALE,static_cast<double>(floor(log10(grossStepX))));
 if (STEP_5*stepx<grossStepX) stepx*=STEP_5;
 else
 {
  if (STEP_2*stepx<grossStepX) stepx*=STEP_2;
 }
 stepx*=TimeDivider;
 cGrRect.Left=floor(cGrRect.Left/stepx)*stepx; 
 cGrRect.Right=ceil(cGrRect.Right/stepx)*stepx;

 int32_t MinTicksY=INITIAL_STEP;
 long double grossStepY=(cGrRect.Top-cGrRect.Bottom)/MinTicksY;
 long double stepy=pow(POW_SCALE,static_cast<double>(floor(log10(grossStepY))));
 if (STEP_5*stepy<grossStepY) stepy*=STEP_5;
 else
 {
  if (STEP_2*stepy<grossStepY) stepy*=STEP_2;
 }
 cGrRect.Bottom=floor(cGrRect.Bottom/stepy)*stepy; 
 cGrRect.Top=ceil(cGrRect.Top/stepy)*stepy; 
 SetGridStep(stepx,stepy);
 SetRectangle(cGrRect);
 RedrawAll();
}

//----------------------------------------------------------------------------------------------------
//экспорт графика в tga-файл
//----------------------------------------------------------------------------------------------------
bool CGraphics::ExportAsTGAFile(char &file_name) const
{
 //функция планируется к реализации
 return(true);	
}
  
//----------------------------------------------------------------------------------------------------
//нажатие левой кнопкой мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::OnLButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 if (MoveMode==true) return;
 if (EnableMagnify==false) return;
 UserMode=true;

 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 if (SelectMode==SELECT_MODE_NO_AREA) //начало выделения
 {
  //запираем курсор в области графика, исключая легенду
  CGrRect cGrRect_Window(0,width,0,height);
  CGrRect cGrRect_Graphics;
  GetClientRectangle(cGrRect_Graphics);
  cGrRect_Window.Bottom=cGrRect_Window.Top+(cGrRect_Graphics.Bottom-cGrRect_Graphics.Top);
  iVideo_Ptr->ClipCursor(cGrRect_Window);

  SelectMode=SELECT_MODE_SET_AREA;
  cGrRect_Select=CGrRect(cGrMousePoint.X,cGrMousePoint.X,cGrMousePoint.Y,cGrMousePoint.Y);
  return;
 }
 if (SelectMode==SELECT_MODE_AREA_IS_CREATED)//щелчок при наличии выделения
 {
  iVideo_Ptr->SetCursorCross();
  iVideo_Ptr->UnClipCursor();
  SelectMode=SELECT_MODE_NO_AREA;  
  Redraw();
  if (cGrMousePoint.X<cGrRect_CorrectSelect.Left || cGrMousePoint.X>cGrRect_CorrectSelect.Right) return;
  if (cGrMousePoint.Y<cGrRect_CorrectSelect.Top || cGrMousePoint.Y>cGrRect_CorrectSelect.Bottom) return;  
  //пользователь щёлкнул в область выделения
  if (width==0 || height==0) return;  
  long double idsx=static_cast<long double>(cGrRect_Based.Right-cGrRect_Based.Left)/static_cast<long double>(width);
  long double idsy=static_cast<long double>(cGrRect_Based.Top-cGrRect_Based.Bottom)/static_cast<long double>(height);
  long double new_xmin=static_cast<long double>((cGrRect_CorrectSelect.Left))*idsx+cGrRect_Based.Left;
  long double new_xmax=static_cast<long double>((cGrRect_CorrectSelect.Right))*idsx+cGrRect_Based.Left;
  long double new_ymax=static_cast<long double>((static_cast<long double>(height)-cGrRect_CorrectSelect.Top-1))*idsy+cGrRect_Based.Bottom;
  long double new_ymin=static_cast<long double>((static_cast<long double>(height)-cGrRect_CorrectSelect.Bottom-1))*idsy+cGrRect_Based.Bottom;
  if (fabs(new_xmax-new_xmin)<=EPS || fabs(new_ymax-new_ymin)<=EPS) return;//некуда больше увеличивать
  cGrRect_Based=CGrRect(new_xmin,new_xmax,new_ymax,new_ymin);

  OnMagnify();
  return;
 }
}
//----------------------------------------------------------------------------------------------------
//нажатие правой кнопкой мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::OnRButtonDown(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 //щёлчок в области выделения
 if (SelectMode==SELECT_MODE_AREA_IS_CREATED)//щелчок при наличии выделения
 {       
  //освобождаем курсор
  iVideo_Ptr->UnClipCursor();
  SelectMode=SELECT_MODE_NO_AREA;//убираем выделение
  //пользователь щёлкнул в область выделения
  if (cGrMousePoint.X>=cGrRect_CorrectSelect.Left && cGrMousePoint.X<=cGrRect_CorrectSelect.Right &&
      cGrMousePoint.Y>=cGrRect_CorrectSelect.Top && cGrMousePoint.Y<=cGrRect_CorrectSelect.Bottom)
  {   
   //делаем масштаб 1:1
   OnOriginalScale();
   iVideo_Ptr->HideCursor();
   return;
  }
 }
 //запираем курсор внутри области окна графика
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);
 CGrRect cGrRect_Window(0,width,0,height);
 iVideo_Ptr->ClipCursor(cGrRect_Window);

 //отменяем выделение
 CancelSelect();
 UserMode=true;
 cGrMousePoint_RBMousePosition=cGrMousePoint;
 MoveMode=true;
 iVideo_Ptr->HideCursor();
}
//----------------------------------------------------------------------------------------------------
//отпускание правой кнопки мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::OnRButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 iVideo_Ptr->UnClipCursor();
 iVideo_Ptr->ShowCursor();
 MoveMode=false;
}
//----------------------------------------------------------------------------------------------------
//отпускание левой кнопки мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::OnLButtonUp(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 if (MoveMode==true) return;
 if (EnableMagnify==false) return;
 if (SelectMode==SELECT_MODE_SET_AREA)//конец выделения
 {
  iVideo_Ptr->UnClipCursor();
  SelectMode=SELECT_MODE_AREA_IS_CREATED;
  if (cGrRect_Select.Left==cGrRect_Select.Right || cGrRect_Select.Top==cGrRect_Select.Bottom) SelectMode=SELECT_MODE_NO_AREA;//так выделять нельзя
  //скорректируем прямоугольник выделения
  cGrRect_CorrectSelect=cGrRect_Select;
  //исправим ошибки координат
  if (cGrRect_Select.Left>cGrRect_Select.Right)
  {
   cGrRect_CorrectSelect.Right=cGrRect_Select.Left;
   cGrRect_CorrectSelect.Left=cGrRect_Select.Right;
  }
  if (cGrRect_Select.Top>cGrRect_Select.Bottom)
  {
   cGrRect_CorrectSelect.Top=cGrRect_Select.Bottom;
   cGrRect_CorrectSelect.Bottom=cGrRect_Select.Top;
  }
  return;
 }
}
//----------------------------------------------------------------------------------------------------
//перемещение мышки
//----------------------------------------------------------------------------------------------------
void CGraphics::OnMouseMove(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 if (MoveMode==true) return;
 if (EnableMagnify==false) return;
 if (SelectMode==SELECT_MODE_SET_AREA && l_button==true) 
 {
  cGrRect_Select.Right=cGrMousePoint.X;
  cGrRect_Select.Bottom=cGrMousePoint.Y;  
  Redraw();
  return;
 }
 if (SelectMode==SELECT_MODE_AREA_IS_CREATED)
 {
  Redraw();
  CGrRect cGrRect_Temporary=cGrRect_Select;
  if (cGrMousePoint.X<cGrRect_CorrectSelect.Left || cGrMousePoint.X>cGrRect_CorrectSelect.Right)
  {
   iVideo_Ptr->SetCursorCross();
   return;
  }
  if (cGrMousePoint.Y<cGrRect_CorrectSelect.Top || cGrMousePoint.Y>cGrRect_CorrectSelect.Bottom)
  {
   iVideo_Ptr->SetCursorCross();
   return;
  }
  iVideo_Ptr->SetCursorMagnify();
 }
 else iVideo_Ptr->SetCursorCross();
}
//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
void CGraphics::OnTimer(const CGrMousePoint &cGrMousePoint,bool l_button,bool r_button,bool c_button)
{
 //запросим область экрана	
 CGrRect cGrRect;
 GetClientRectangle(cGrRect);
 
 if (cGrMousePoint.X<cGrRect.Left) return;
 if (cGrMousePoint.Y<cGrRect.Top) return;
 if (cGrMousePoint.X>cGrRect.Right) return;
 if (cGrMousePoint.Y>cGrRect.Bottom) return;	
	
 if (MoveMode==true)
 {
  if (cGrMousePoint_RBMousePosition.X==cGrMousePoint.X && cGrMousePoint_RBMousePosition.Y==cGrMousePoint.Y) return;
  
  long double dx=cGrMousePoint_RBMousePosition.X-cGrMousePoint.X;
  long double dy=cGrMousePoint_RBMousePosition.Y-cGrMousePoint.Y;
  
  long double length_x=cGrRect.Right-cGrRect.Left;
  long double length_y=cGrRect.Bottom-cGrRect.Top;
  long double scale_x=0;
  long double scale_y=0;
  if (length_x>0) scale_x=(cGrRect_Based.Right-cGrRect_Based.Left)/length_x;
  if (length_y>0) scale_y=(cGrRect_Based.Top-cGrRect_Based.Bottom)/length_y;
  dx*=scale_x;
  dy*=scale_y;

  if (InversionMoveMode==true)
  {
   cGrRect_Based.Left+=dx;
   cGrRect_Based.Right+=dx;
   cGrRect_Based.Bottom-=dy;
   cGrRect_Based.Top-=dy;
  }
  else
  {
   cGrRect_Based.Left-=dx;
   cGrRect_Based.Right-=dx;
   cGrRect_Based.Bottom+=dy;
   cGrRect_Based.Top+=dy;
  }
  iVideo_Ptr->SetCursorPos(cGrMousePoint_RBMousePosition.X,cGrMousePoint_RBMousePosition.Y);
  SetRectangle(cGrRect_Based);
  RedrawAll();
  return;
 } 
 cGrMousePoint_RBMousePosition=cGrMousePoint;
}
