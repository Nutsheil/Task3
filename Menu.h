#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#pragma warning(disable: 4996)

typedef enum MY_ENUM
{
	up, down, right, left, other
}my_enum;

typedef struct MY_BUTTON
{
	char* name;
	int connect_with_window;
	POINT cords[2];
	COLORREF color;
	COLORREF hight_color;
}my_button;

typedef struct MY_WINDOW
{
	int num_of_buttons;
	int connect_with_button;
	POINT cords[2];
	COLORREF color;
	my_button* buttons;
	bool active;
}my_window;

typedef struct MY_MENU
{
	my_window* windows;
	int num_of_windows, selected_button, selected_window;
}my_menu;

my_menu* ReadTXT();

void Paint(HDC hdc, my_menu* menu, my_enum key);