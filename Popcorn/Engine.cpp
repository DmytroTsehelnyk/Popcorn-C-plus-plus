#include "Engine.h"

enum E_Brick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

HPEN Brick_Pink_Pen, Brick_Blue_Pen;
HBRUSH Brick_Pink_Brush, Brick_Blue_Brush;

const int Global_Scale = 3;

const int Cell_Width = 16; // width of brick + 1px frame
const int Cell_Height = 8; // height of brick + 1px frame

const int Level_Offset_X = 8; // right
const int Level_Offset_Y = 6; // down

const int Brick_Width = 15;
const int Brick_Heigth = 7;

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
void _Init()
{ // Initialize game on start

  // Purple brick
   Brick_Pink_Pen = CreatePen(PS_SOLID, 0, RGB(255, 150, 195)); // pink frame
   Brick_Pink_Brush = CreateSolidBrush(RGB(255, 150, 195)); // pink filling

   // Blue brick
   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(85, 185, 255)); // blue frame
   Brick_Blue_Brush = CreateSolidBrush(RGB(85, 185, 255)); // blue filling

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
      pen = Brick_Pink_Pen;
      brush = Brick_Pink_Brush;
      break;

   case EBT_Blue:
      // Draw a blue brick
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   // Draw the brick on enetered coordinates with scale of x3
   // Coordinates = X/Y offset * 3
   RoundRect(hdc, x * Global_Scale, y * Global_Scale,
      (x + Brick_Width) * Global_Scale, (y + Brick_Heigth) * Global_Scale,
      2 * Global_Scale, 2 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------
void _DrawLevel(HDC hdc)
{ // Draw the Level 01

   for (int col = 0; col < 14; col++)
      for (int row = 0; row < 12; row++)
         _DrawBrick(hdc, Level_Offset_X + row * Cell_Width, Level_Offset_Y + col * Cell_Height, 
            (E_Brick_Type)Level_01[col][row]);
}
//----------------------------------------------------------------------------------------------------
void _DrawFrame(HDC hdc)
{ // Draw the game frame

   _DrawLevel(hdc);

}
//----------------------------------------------------------------------------------------------------
