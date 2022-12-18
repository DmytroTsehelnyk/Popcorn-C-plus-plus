#pragma once

#include <Windows.h>

enum E_Key_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

void _InitEngine(HWND hwnd);
void _DrawFrame(HDC hdc, RECT &paint_area);
int _OnKeyDown(E_Key_Type key_type);
