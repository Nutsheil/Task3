#include "Menu.h"

typedef struct MY_BUTTON
{
	char* name;
	int index;
	POINT cords[2];
	COLORREF color;
	COLORREF hight_color;
}my_button;

typedef struct MY_WINDOW
{
	int num_of_buttons;
	int index;
	POINT cords[2];
	COLORREF color;
	my_button* buttons;
	bool active;
}my_window;

typedef struct MY_MENU
{
	my_window* windows;
	int num_of_windows;
}my_menu;

char* GetWord(FILE* stream)
{
	char c;
	char* word = malloc(20);
	int i = 0;
	while (!isalnum(c = fgetc(stream)));
	do
	{
		word[i++] = c;
	} while (isalnum(c = fgetc(stream)));
	word[i] = '\0';

	return word;
}

int GetDigit(FILE* stream)
{
	char c;
	char* digits = malloc(10);
	int i = 0;
	while (!isdigit(c = fgetc(stream)));
	do
	{
		digits[i++] = c;
	} while (isdigit(c = fgetc(stream)));
	digits[i] = '\0';

	return atoi(digits);
}

void* ReadTXT()
{
	my_menu* menuTXT = (my_menu*)malloc(sizeof(my_menu));
	menuTXT->windows = (my_window*)malloc(sizeof(my_window));
	menuTXT->num_of_windows = 0;

	FILE* stream = fopen("menu.txt", "rb");
	if (stream == NULL)
		return NULL; //Не открылся текстовый файл

	char* word;
	while (strcmp(word = GetWord(stream), "MenuBegin") != 0);
	while (strcmp(word = GetWord(stream), "MenuEnd") != 0)
	{
		if (strcmp(word, "Window") == 0)
		{
			menuTXT->num_of_windows++;
			if (menuTXT->num_of_windows > 1)
				menuTXT->windows = realloc(menuTXT->windows, menuTXT->num_of_windows * sizeof(my_window));

			menuTXT->windows[menuTXT->num_of_windows - 1].buttons = (my_button*)malloc(sizeof(my_button));
			menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons = 0;

			menuTXT->windows[menuTXT->num_of_windows - 1].cords[0] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[menuTXT->num_of_windows - 1].cords[1] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[menuTXT->num_of_windows - 1].color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[menuTXT->num_of_windows - 1].index = GetDigit(stream);
		}
		if (strcmp(word, "Button") == 0)
		{
			menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons++;
			if (menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons > 1)
				menuTXT->windows[menuTXT->num_of_windows - 1].buttons = realloc(menuTXT->windows[menuTXT->num_of_windows - 1].buttons, menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons * sizeof(my_button));

			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].name = GetWord(stream);
			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].cords[0] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].cords[1] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].hight_color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[menuTXT->num_of_windows - 1].buttons[menuTXT->windows[menuTXT->num_of_windows - 1].num_of_buttons - 1].index = GetDigit(stream);
		}
	}
	return menuTXT;
}

void Paint(HDC hdc, void* menuu, my_enum key)
{
	my_menu* menu = (my_menu*)menuu;
	static int selected_button = -1, selected_window = 0;

	switch (key)
	{
	case up:
	{
		if (selected_button > 0)
		{
			selected_button--;
			for (int i = 0; i < menu->num_of_windows; i++)
			{
				if (menu->windows[i].index == menu->windows[selected_window].buttons[selected_button].index)
					menu->windows[i].active = true;
				if (menu->windows[i].index == menu->windows[selected_window].buttons[selected_button + 1].index)
					menu->windows[i].active = false;
			}
		}
		break;
	}
	case down:
	{
		if (selected_button < menu->windows[selected_window].num_of_buttons - 1)
		{
			selected_button++;
			for (int i = 0; i < menu->num_of_windows; i++)
			{
				if (menu->windows[i].index == menu->windows[selected_window].buttons[selected_button].index)
					menu->windows[i].active = true;
				if (menu->windows[i].index == menu->windows[selected_window].buttons[selected_button - 1].index)
					menu->windows[i].active = false;
			}
		}
		break;
	}
	case right:
	{
		if ((menu->windows[selected_window].buttons[selected_button].index != 0) && (selected_window < menu->num_of_windows - 1))
		{
			selected_window++;
			selected_button = 0;
		}
		break;
	}
	case left:
	{
		if (selected_window > 0)
		{
			for (int i = 0; i < menu->windows[selected_window - 1].num_of_buttons; i++)
				if (menu->windows[selected_window].index == menu->windows[selected_window - 1].buttons[i].index)
					selected_button = i;
			selected_window--;
			if (selected_window < menu->num_of_windows - 1)
				menu->windows[selected_window + 2].active = false;
		}
		break;
	}
	default:
		break;
	}

	for (int i = 0; i < menu->num_of_windows; i++)
	{
		if ((menu->windows[i].active == true) || (menu->windows[i].index == 0))
		{
			HBRUSH brush_for_window = CreateSolidBrush(menu->windows[i].color);
			SelectObject(hdc, brush_for_window);
			Rectangle(hdc, menu->windows[i].cords[0].x, menu->windows[i].cords[0].y, menu->windows[i].cords[1].x, menu->windows[i].cords[1].y);
			DeleteObject(brush_for_window);

			for (int j = 0; j < menu->windows[i].num_of_buttons; j++)
			{
				HBRUSH brush_for_button;
				if ((j == selected_button) && (i == selected_window))
				{
					brush_for_button = CreateSolidBrush(menu->windows[i].buttons[j].hight_color);
					SetBkColor(hdc, menu->windows[i].buttons[j].hight_color);
				}
				else
				{
					brush_for_button = CreateSolidBrush(menu->windows[i].buttons[j].color);
					SetBkColor(hdc, menu->windows[i].buttons[j].color);
				}
				SelectObject(hdc, brush_for_button);
				Rectangle(hdc, menu->windows[i].buttons[j].cords[0].x, menu->windows[i].buttons[j].cords[0].y, menu->windows[i].buttons[j].cords[1].x, menu->windows[i].buttons[j].cords[1].y);
				DeleteObject(brush_for_button);

				DrawText(hdc, menu->windows[i].buttons[j].name, strlen(menu->windows[i].buttons[j].name), menu->windows[i].buttons[j].cords, DT_CENTER);
			}
		}
	}
}