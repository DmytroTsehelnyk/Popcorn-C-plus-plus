#include "Engine.h"

enum E_Brick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

HPEN Highlight_Pen, Object_Pink_Pen, Object_Blue_Pen;
HBRUSH Object_Pink_Brush, Object_Blue_Brush;

const int Global_Scale = 3;

const int Cell_Width = 16; // width of brick + 1px frame
const int Cell_Height = 8; // height of brick + 1px frame

const int Level_Offset_X = 8; // right
const int Level_Offset_Y = 6; // down

const int Brick_Width = 15;
const int Brick_Heigth = 7;

const int Circle_Size = 7;

int Inner_Width = 21;

char Level_01[14][12] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//----------------------------------------------------------------------------------------------------
void _CreateColor(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//----------------------------------------------------------------------------------------------------
void _Init()
{ // Initialize game on start

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));


  // Initialize pink pen and brush
   _CreateColor(255, 150, 195, Object_Pink_Pen, Object_Pink_Brush);

   // Initialize blue pen and brush
   _CreateColor(85, 185, 255, Object_Blue_Pen, Object_Blue_Brush);
}
//----------------------------------------------------------------------------------------------------
void _DrawBrick(HDC hdc, int x, int y, E_Brick_Type brick_type)
{ // Draws the brick with specified color

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Pink:
      // Draw a pink brick
      pen = Object_Pink_Pen;
      brush = Object_Pink_Brush;
      break;

   case EBT_Blue:
      // Draw a blue brick
      pen = Object_Blue_Pen;
      brush = Object_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   // Draw the brick on enetered coordinates with scale of x3
   RoundRect(hdc, 
      x * Global_Scale, 
      y * Global_Scale,
      (x + Brick_Width) * Global_Scale, 
      (y + Brick_Heigth) * Global_Scale,
      2 * Global_Scale, 2 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------
void _DrawLevel(HDC hdc)
{ // Draw the Level 01

   for (int col = 0; col < 14; col++)
      for (int row = 0; row < 12; row++)
         _DrawBrick(hdc, 
            Level_Offset_X + row * Cell_Width, 
            Level_Offset_Y + col * Cell_Height, 
            (E_Brick_Type)Level_01[col][row]);
}
//----------------------------------------------------------------------------------------------------
void _DrawPlatform(HDC hdc, int x, int y)
{ // Draw the platform
   
  // Assign colors to ellipces
   SelectObject(hdc, Object_Pink_Pen);
   SelectObject(hdc, Object_Pink_Brush);

   // Create left ellipse
   Ellipse(hdc, 
      x * Global_Scale, 
      y * Global_Scale, 
      (x + Circle_Size) * Global_Scale, 
      (y + Circle_Size) * Global_Scale);

   // Create right ellipse
   Ellipse(hdc, 
      (x + Inner_Width) * Global_Scale, 
      y * Global_Scale, 
      (x + Circle_Size + Inner_Width) * Global_Scale,
      (y + Circle_Size) * Global_Scale);

   // Create highlight for platform ellipse
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc,
      (x + 1) * Global_Scale,
      (y + 1) * Global_Scale,
      (x + Circle_Size - 1) * Global_Scale,
      (y + Circle_Size - 1) * Global_Scale,
      (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale,
      (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);

   // Assign colors to central part of the platform
   SelectObject(hdc, Object_Blue_Pen);
   SelectObject(hdc, Object_Blue_Brush);

   // Central part of the platforn
   RoundRect(hdc, 
      (x + 4) * Global_Scale, 
      (y + 1) * Global_Scale, 
      (x + 4 + Inner_Width - 1) * Global_Scale, 
      (y + 1 + 5) * Global_Scale,
      3 * Global_Scale, 3 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------
void _DrawFrame(HDC hdc)
{ // Draw the game frame

   _DrawLevel(hdc);

   _DrawPlatform(hdc, 50, 100);
}
//----------------------------------------------------------------------------------------------------
