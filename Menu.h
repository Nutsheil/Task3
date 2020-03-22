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

void* ReadTXT();

void Paint(HDC hdc, void* menuu, my_enum key);