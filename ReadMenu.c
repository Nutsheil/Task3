#include "Menu.h"

char* GetWord(FILE* stream)
{
	int i = 0, lenght_of_word = 10;
	char c;
	char* word = malloc(lenght_of_word);
	while (!isalnum(c = fgetc(stream)));
	do
	{
		word[i++] = c;
		if (i == lenght_of_word)
			word = realloc(word, lenght_of_word += 10);
	} while (isalnum(c = fgetc(stream)));
	word[i] = '\0';

	return word;
}

int GetDigit(FILE* stream)
{
	int i = 0, lenght_of_digits = 10;
	char c;
	char* digits = malloc(10);
	while (!isdigit(c = fgetc(stream)));
	do
	{
		digits[i++] = c;
		if (i == lenght_of_digits)
			digits = realloc(digits, lenght_of_digits += 10);
	} while (isdigit(c = fgetc(stream)));
	digits[i] = '\0';

	return atoi(digits);
}

void* ReadTXT()
{
	my_menu* menuTXT = (my_menu*)malloc(sizeof(my_menu));
	menuTXT->windows = (my_window*)malloc(sizeof(my_window));
	menuTXT->num_of_windows = 0;
	menuTXT->selected_button = -1;
	menuTXT->selected_window = 0;
	int WinNum = -1;

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
			WinNum++;

			if (menuTXT->num_of_windows > 1)
				menuTXT->windows = realloc(menuTXT->windows, menuTXT->num_of_windows * sizeof(my_window));

			menuTXT->windows[WinNum].buttons = (my_button*)malloc(sizeof(my_button));
			menuTXT->windows[WinNum].num_of_buttons = 0;

			menuTXT->windows[WinNum].cords[0] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[WinNum].cords[1] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[WinNum].color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[WinNum].connect_with_button = GetDigit(stream);
		}
		if (strcmp(word, "Button") == 0)
		{
			if (WinNum == -1)
				exit(1);

			menuTXT->windows[WinNum].num_of_buttons++;
			if (menuTXT->windows[WinNum].num_of_buttons > 1)
				menuTXT->windows[WinNum].buttons = realloc(menuTXT->windows[WinNum].buttons, menuTXT->windows[WinNum].num_of_buttons * sizeof(my_button));

			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].name = GetWord(stream);
			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].cords[0] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].cords[1] = (POINT){ GetDigit(stream), GetDigit(stream) };
			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].hight_color = RGB(GetDigit(stream), GetDigit(stream), GetDigit(stream));
			menuTXT->windows[WinNum].buttons[menuTXT->windows[WinNum].num_of_buttons - 1].connect_with_window = GetDigit(stream);
		}
	}
	return menuTXT;
}

void Click_Handler(my_menu* menu ,my_enum key)
{
	switch (key)
	{
	case up:
	{
		if (menu->selected_button > 0)
		{
			menu->selected_button--;
			for (int i = 0; i < menu->num_of_windows; i++)
			{
				if (menu->windows[i].connect_with_button == menu->windows[menu->selected_window].buttons[menu->selected_button].connect_with_window)
					menu->windows[i].active = true;
				if (menu->windows[i].connect_with_button == menu->windows[menu->selected_window].buttons[menu->selected_button + 1].connect_with_window)
					menu->windows[i].active = false;
			}
		}
		break;
	}
	case down:
	{
		if (menu->selected_button < menu->windows[menu->selected_window].num_of_buttons - 1)
		{
			menu->selected_button++;
			for (int i = 0; i < menu->num_of_windows; i++)
			{
				if (menu->windows[i].connect_with_button == menu->windows[menu->selected_window].buttons[menu->selected_button].connect_with_window)
					menu->windows[i].active = true;
				if (menu->windows[i].connect_with_button == menu->windows[menu->selected_window].buttons[menu->selected_button - 1].connect_with_window)
					menu->windows[i].active = false;
			}
		}
		break;
	}
	case right:
	{
		if ((menu->windows[menu->selected_window].buttons[menu->selected_button].connect_with_window != 0) && (menu->selected_window < menu->num_of_windows - 1))
		{
			menu->selected_window++;
			menu->selected_button = 0;
		}
		break;
	}
	case left:
	{
		if (menu->selected_window > 0)
		{
			for (int i = 0; i < menu->windows[menu->selected_window - 1].num_of_buttons; i++)
				if (menu->windows[menu->selected_window].connect_with_button == menu->windows[menu->selected_window - 1].buttons[i].connect_with_window)
					menu->selected_button = i;
			menu->selected_window--;
			if (menu->selected_window < menu->num_of_windows - 1)
				menu->windows[menu->selected_window + 2].active = false;
		}
		break;
	}
	default:
		break;
	}
}

void Paint(HDC hdc, my_menu* menu, my_enum key)
{
	Click_Handler(menu, key);

	for (int i = 0; i < menu->num_of_windows; i++)
	{
		if ((menu->windows[i].active == true) || (menu->windows[i].connect_with_button == 0))
		{
			HBRUSH brush_for_window = CreateSolidBrush(menu->windows[i].color);
			SelectObject(hdc, brush_for_window);
			Rectangle(hdc, menu->windows[i].cords[0].x, menu->windows[i].cords[0].y, menu->windows[i].cords[1].x, menu->windows[i].cords[1].y);
			DeleteObject(brush_for_window);

			for (int j = 0; j < menu->windows[i].num_of_buttons; j++)
			{
				HBRUSH brush_for_button;
				if ((j == menu->selected_button) && (i == menu->selected_window))
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