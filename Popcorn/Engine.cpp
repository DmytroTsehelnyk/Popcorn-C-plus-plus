#include "Engine.h"
#define _USE_MATH_DEFINES
#include <cmath>
//----------------------------------------------------------------------------------------------------
// Initialize global variables

enum E_Letter_Type
{
   ELT_None,
   ELT_O
};

enum E_Brick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

HWND Hwnd;
HPEN Highlight_Pen, Letter_Pen, Pink_Pen, Blue_Pen, BG_Pen;
HBRUSH Pink_Brush, Blue_Brush, BG_Brush;
RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

// global constant variables
const int Scale = 3;

const int Cell_Width = 16;
const int Cell_Height = 8;

const int X_Offset = 8;
const int Y_Offset = 6;

const int Level_Width = 14; // Level width in cells
const int Level_Heigth = 12;

const int Brick_Width = 15;
const int Brick_Heigth = 7;

const int Ellipse_Size = 7;
const int Platform_Y_Pos = 185;

const int Platform_Height = 7;

// global variables
int Platform_Speed = 2;
int Inner_Width = 21;
int Platform_X_Pos = 0;
int Platform_X_Step = Scale * Platform_Speed;
int Platform_Width = 28;

//----------------------------------------------------------------------------------------------------
char Level_01[Level_Width][Level_Heigth] =
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
void _RedrawPlatform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = (X_Offset + Platform_X_Pos) * Scale;
   Platform_Rect.top = Platform_Y_Pos * Scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//----------------------------------------------------------------------------------------------------
void _InitEngine(HWND hwnd)
{ // Initialize colors on the game start

   Hwnd = hwnd;
      
   // Create the highlight
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   
   // Create color for letters
   Letter_Pen = CreatePen(PS_SOLID, Scale, RGB(255, 255, 255));

   // Create pink color
   _CreateColor(255, 150, 195, Pink_Pen, Pink_Brush);

   // Create blue color
   _CreateColor(85, 185, 255, Blue_Pen, Blue_Brush);

   // Create background color
   _CreateColor(55, 65, 70, BG_Pen, BG_Brush);

   Level_Rect.left = X_Offset * Scale;
   Level_Rect.top = Y_Offset * Scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Width * Level_Heigth * Scale;

   _RedrawPlatform();
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
void _SetColor(bool is_color_switch, 
   HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{ // Set new color for brick on rotation
   if (is_color_switch)
   {
      front_pen = Pink_Pen;
      front_brush = Pink_Brush;

      back_pen = Blue_Pen;
      back_brush = Blue_Brush;
   }
   else
   {
      front_pen = Blue_Pen;
      front_brush = Blue_Brush;

      back_pen = Pink_Pen;
      back_brush = Pink_Brush;
   }

}
//----------------------------------------------------------------------------------------------------
void _DrawBrickLetter(HDC hdc, int x, int y, int rotation_step, 
   E_Brick_Type brick_type, E_Letter_Type letter_type)
{ // Draw the letter
   
   bool switch_color;

   double offset;
   double rotation_angle;
   
   int back_part_offset;
   int brick_half_height = Brick_Heigth * Scale / 2;

   XFORM xForm, old_xForm;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;

   // Correcting the rotation step and rotation angle
   rotation_angle = rotation_step % 16;

   // Step conversion into rotation angle
   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);

   // Rotate the brick only if it's blue or pink
   if (!(brick_type == EBT_Blue || brick_type == EBT_Pink))
      return;

   if (rotation_step > 4 && rotation_step <= 12)
   {
      // if brick_type is blue, set switch_color to true / if not, to false
      switch_color = brick_type == EBT_Blue;
   }
   else
   {
      // if brick_type is pink, set switch_color to true / if not, to false
      switch_color = brick_type == EBT_Pink;
   }

   _SetColor(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      // Show brick background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, 
         x,
         y + brick_half_height - Scale,
         x + Brick_Width * Scale,
         y + brick_half_height);

      // Show brick foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

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
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)Scale;

      back_part_offset = (int)round(offset);

      Rectangle(hdc, 0,
         -brick_half_height - back_part_offset,
         Brick_Width * Scale,
         brick_half_height - back_part_offset);

      // Show brick foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0,
         -brick_half_height,
         Brick_Width * Scale,
         brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
         SelectObject(hdc, Letter_Pen);
         Ellipse(hdc, 
            0 + 5 * Scale, 
            (-5 * Scale) / 2,
            0 + 10 * Scale, 
            5 * Scale / 2);
         }
      }

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
   
   // Clear platform position background
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   Rectangle(hdc, 
      Prev_Platform_Rect.left,
      Prev_Platform_Rect.top,
      Prev_Platform_Rect.right,
      Prev_Platform_Rect.bottom);

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
void _DrawFrame(HDC hdc, RECT &paint_area)
{ // Draw the game frame

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      _DrawLevel(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      _DrawPlatform(hdc, X_Offset + Platform_X_Pos, Platform_Y_Pos);

   /*for (int step = 0; step < 16; step++)
   {
      _DrawBrickLetter(hdc, 20 + step * Cell_Width * Scale, 100, step, EBT_Blue, ELT_O);
      _DrawBrickLetter(hdc, 20 + step * Cell_Width * Scale, 140, step, EBT_Pink, ELT_O);
   }*/
}
//----------------------------------------------------------------------------------------------------
int _OnKeyDown(E_Key_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      _RedrawPlatform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      _RedrawPlatform();
      break;

   case EKT_Space:

      break;

   default:
      break;
   }
   return 0;
}
//----------------------------------------------------------------------------------------------------
