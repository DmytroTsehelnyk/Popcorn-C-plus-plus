#include "Engine.h"
#define _USE_MATH_DEFINES
#include <cmath>
//----------------------------------------------------------------------------------------------------
// Initialize global variables

enum E_Brick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

HPEN Highlight_Pen, Pink_Pen, Blue_Pen;
HBRUSH Pink_Brush, Blue_Brush;

const int Scale = 3;

const int Cell_Width = 16; // width of brick + 1px frame
const int Cell_Height = 8; // height of brick + 1px frame

const int X_Offset = 8; // right offset
const int Y_Offset = 6; // down offset

const int Brick_Width = 15;
const int Brick_Heigth = 7;

const int Ellipse_Size = 7;

int Inner_Width = 21;

//----------------------------------------------------------------------------------------------------
char Level_01[14][12] =
{ // Bricks position on the game frame
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
{ // Create pen and brush according to entered rgb

   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
   
}
//----------------------------------------------------------------------------------------------------
void _Init()
{ // Initialize colors on the game start

  // Create the highlight
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

  // Create pink color
   _CreateColor(255, 150, 195, Pink_Pen, Pink_Brush);

   // Create blue color
   _CreateColor(85, 185, 255, Blue_Pen, Blue_Brush);

}
//----------------------------------------------------------------------------------------------------
void _DrawBrick(HDC hdc, int x, int y, E_Brick_Type brick_type)
{ // Draws the brick with a specified color

   // Initialize variables
   HPEN pen;
   HBRUSH brush;

   // Choose color for brick according to its number
   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Pink:
      pen = Pink_Pen;
      brush = Pink_Brush;
      break;

   case EBT_Blue:
      pen = Blue_Pen;
      brush = Blue_Brush;
      break;

   default:
      return;
   }

   // Paint the brick
   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   // Create the brick on specified coordinates
   RoundRect(hdc,
      x * Scale, // X upper-left corner
      y * Scale, // Y upper-left corner
      (x + Brick_Width) * Scale, // X lower-right corner
      (y + Brick_Heigth) * Scale, // Y lower-right corner

      2 * Scale, // ellipse width to draw the rounded corners
      2 * Scale); // ellipse height to draw the rounded corners
}
//----------------------------------------------------------------------------------------------------
void _DrawBrickLetter(HDC hdc, int x, int y, int rotation_step)
{ // Draw the letter

   double offset;

   // Step conversion into rotation angle
   double rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;

   int brick_half_height = Brick_Heigth * Scale / 2;
   int back_part_offset;

   XFORM xForm, old_xForm;

   if (rotation_step == 4 || rotation_step == 12)
   {
      // Show brick background
      SelectObject(hdc, Pink_Pen);
      SelectObject(hdc, Pink_Brush);

      Rectangle(hdc, 
         x,
         y + brick_half_height - Scale,
         x + Brick_Width * Scale,
         y + brick_half_height);

      // Show brick foreground
      SelectObject(hdc, Blue_Pen);
      SelectObject(hdc, Blue_Brush);

      Rectangle(hdc, 
         x,
         y + brick_half_height,
         x + Brick_Width * Scale,
         y + brick_half_height + Scale - 1);
   }
   else
   {
   SetGraphicsMode(hdc, GM_ADVANCED);

   // Letters rotation matrix
   xForm.eM11 = 1.0f;
   xForm.eM12 = 0.0f;
   xForm.eM21 = 0.0f;
   xForm.eM22 = (float)cos(rotation_angle);
   xForm.eDx = (float)x;
   xForm.eDy = (float)y + (float)(brick_half_height);

   GetWorldTransform(hdc, &old_xForm);
   SetWorldTransform(hdc, &xForm);

      // Show brick background
      SelectObject(hdc, Pink_Pen);
      SelectObject(hdc, Pink_Brush);

      offset = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)Scale;

      back_part_offset = (int)round(offset);

      Rectangle(hdc, 0,
         -brick_half_height - back_part_offset,
         Brick_Width * Scale,
         brick_half_height - back_part_offset);

      // Show brick foreground
      SelectObject(hdc, Blue_Pen);
      SelectObject(hdc, Blue_Brush);

      Rectangle(hdc, 0,
         -brick_half_height,
         Brick_Width * Scale,
         brick_half_height);

      SetWorldTransform(hdc, &old_xForm);
   }
}
//----------------------------------------------------------------------------------------------------
void _DrawLevel(HDC hdc)
{ // Draw the Level 01 (bricks position)

   for (int col = 0; col < 14; col++)
      for (int row = 0; row < 12; row++)
         _DrawBrick(hdc,
            X_Offset + row * Cell_Width, // X coordinate position
            Y_Offset + col * Cell_Height, // Y coordinate position
            (E_Brick_Type)Level_01[col][row]); // Brick color
}
//----------------------------------------------------------------------------------------------------
void _DrawPlatform(HDC hdc, int x, int y)
{ // Draw the platform
   
   // Paint the ellipses
   SelectObject(hdc, Pink_Pen);
   SelectObject(hdc, Pink_Brush);

   // Create left ellipse
   Ellipse(hdc, 
      x * Scale, // X upper-left corner
      y * Scale, // Y upper-left corner
      (x + Ellipse_Size) * Scale, // X lower-right corner
      (y + Ellipse_Size) * Scale); // X lower-right corner

   // Create right ellipse
   Ellipse(hdc, 
      (x + Inner_Width) * Scale, // X upper-left corner
      y * Scale, // Y upper-left corner
      (x + Ellipse_Size + Inner_Width) * Scale, // X lower-right corner
      (y + Ellipse_Size) * Scale); // X lower-right corner

   // Create highlight for left ellipse
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc,

      (x + 1) * Scale, // X bounding rectangle upper-left corner
      (y + 1) * Scale, // Y bounding rectangle upper-left corner
      (x + Ellipse_Size - 1) * Scale, // X bounding rectangle lower-right corner
      (y + Ellipse_Size - 1) * Scale, // Y bounding rectangle lower-right corner

      (x + 1 + 1) * Scale, // X arc starting point
      (y + 1) * Scale, // Y arc starting point
      (x + 1) * Scale, // X arc ending point
      (y + 1 + 2) * Scale); // Y arc ending point

   //--------------------------------------------------

   // Paint the platform
   SelectObject(hdc, Blue_Pen);
   SelectObject(hdc, Blue_Brush);

   // Create the platform on specified coordinates
   RoundRect(hdc,
      (x + 4) * Scale, // X coordinate of the upper-left corner
      (y + 1) * Scale, // Y coordinate of the upper-left corner

      (x + 4 + Inner_Width - 1) * Scale, // X coordinate of the lower-right corner
      (y + 1 + 5) * Scale, // Y coordinate of the lower-right corner

      2 * Scale, // ellipse width to draw the rounded corners
      2 * Scale); // ellipse height to draw the rounded corners

}
//----------------------------------------------------------------------------------------------------
void _DrawFrame(HDC hdc)
{ // Draw the game frame

   /*_DrawLevel(hdc);

   _DrawPlatform(hdc, 80, 120);*/

   for (int step = 0; step < 16; step++)
      _DrawBrickLetter(hdc, 20 + step * Cell_Width * Scale, 100, step);
}
//----------------------------------------------------------------------------------------------------
