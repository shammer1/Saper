/*
SAPER
Управление при помощи 'w' 'a' 's' 'd' ' '
Якубенко



                        КАК РАБОТАЕТ МОЙ КОД  
			+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
main --->	|    Game																																													  | 
			+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
			|																																															  |
			|									+---------------------------------------------------------------------------------------------------------------------------------------+				  |
			|GenerateFied --> DrawStartField-->	|Цикл считывания клавиатуры	(выходим когда конец игры)																			        |-->EndGame		  |
			|									+---------------------------------------------------------------------------------------------------------------------------------------+				  |
			|									|													+---------------------------------------------------------------------------+		|					
			|									| проверяем на w/a/s/d -->проверяем на маркировку-->| проверяем на пробел														|		|	
												|	СhoiceCell					MarkCell			+---------------------------------------------------------------------------+		|
												|												+-->|OpenCell																	|		|
												|												|	+---------------------------------------------------------------------------+		|
												|												|	|			  +-------------------------------+								|		|
												|												|	|PaintCell--->|OpenCellsAround(проверка на 0) | --> проверка на конец игры	|		|
												|												|	|			  +-------------------------------+								|		|
												|												|	|			  |								  |								|		|
												|												+-----------------+    запускаем функцию OpenCell |								|		|
												|													|			  |    для всех клеток рядом	  |								|		|
												|													|			  +-------------------------------+								|		|
												|													+---------------------------------------------------------------------------+		|
												|																																		|
												+---------------------------------------------------------------------------------------------------------------------------------------+
*/

#include <windows.h>
#include<conio.h>
#include<stdio.h>
#include<string.h>
#include<ctime>
#include<cstdlib>

struct/*информация о клетке*/ Cell 
{
	char value='0' , status = 1; // value - количество мин рядом / status - состояние клетки 3 типа открыта(0)/закрыта(1)/флажок(2)
};
void Menu_DrawMenu();//Рисуем меню
void Menu_DrawPointer(int);//Рисуем выделение кнопки меню
int  Menu_СhooseParagraph (int);//выбераем другую кнопку меню
int  Menu_Settings(int*, int*, int*);//Кнопка настройки/цикл считывания клавиатуру
void	  Settings_DrawSettings(int*, int*, int*);//рисуем настройки
void	  Settings_DrawPointer(int);//Рисуем выделение кнопки настроек
int		  Settings_СhooseParagraph(int);//выбераем другую кнопку настроек
void	  Settings_EditOptions(int, int, int*, int*, int*);//меняем длину/высоту/мины
void Menu_Game(int, int, int);//Заходим в игру/цикл считывания клавиатуры
Cell*	  Game_GenerateFied(int, int, int);//рандомно генерируем поле
void	  Game_DrawStartField(int, int);//рисуем все клеточки

void	  Game_DrawPointer(int, int);//рисуем указатель на клетку(красный квадратик)															|
int		  Game_СhoiceCell(int , int , int , int );//двигаем указатель на клетку																	|
int		  Game_MarkCell(char ,int ,int );//ставим флажок на клетку																				|
int	/*+>*/Game_OpenCell(int, int, Cell*, int, int);//Пробел/проверка клетки(закрыта/открыта мина/не мина) и проверка на конец игры				| Цикл считывания клавиатуры
void/*|*/	   OpenCell_DrawCell(Cell *, int, int);//нарисавать циферку на клетке																|
int	/*+------*/OpenCell_OpenCellsAround(int ,int ,Cell * ,int ,int );//открываем все клетки рядом с нашей которые не выходят за границы поля	|
int			   OpenCell_CheckMarkAround(int , int , Cell * Field, int , int );//количество флажков вокруг == количеству мин вокруг				|

void	  Game_DrawEnd(Cell *, int, int, int);//рисуем открытое поле и выводим результат

HPEN RedPen = CreatePen(PS_SOLID, 1, RGB(220, 40, 40));     // 
HPEN GreyPen = CreatePen(PS_SOLID, 1, RGB(224, 224, 224));	// кисти и перья
HBRUSH RedBrush = CreateSolidBrush(RGB(220, 40, 40));		// 
HBRUSH YellowBrush = CreateSolidBrush(RGB(200, 200, 0));	// 
HWND hWnd = GetConsoleWindow(); //Получаем дескриптор окна консоли
HDC  hDC = GetDC(hWnd);			//Получаем контекст устройства по полученному дескриптору
RECT Rect; //прямоугольник

int  main()
{
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	CONSOLE_CURSOR_INFO structCursorInfo;												//
	GetConsoleCursorInfo((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);	//Выключаем курсор
	structCursorInfo.bVisible = FALSE;													//
	SetConsoleCursorInfo((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);	//
	MoveWindow(hWnd, 125, 125, 33 + 700, 39 + 500, FALSE);//меняем размер окна
	static LOGFONT font_menu;									//создаем шрифт
	strcpy_s(font_menu.lfFaceName, "Times New Roman");	//копируем в строку название шрифта 
	font_menu.lfHeight = 25;							//высота                                
	font_menu.lfWidth = 10;								//ширина
	font_menu.lfWeight = 1000;							//толщина

	int NamberParagraph=0;
	char button;
	int height = 12, length = 16, mins = 30;//параметры игры

	while (1)//цикл считывания клавиатуры
	{
		Menu_DrawMenu();//Рисуем меню
		Menu_DrawPointer(NamberParagraph);//Рисуем выделение кнопки меню
		button = _getch();
		if (button == 'w')
			NamberParagraph = Menu_СhooseParagraph(NamberParagraph - 1);//
		else if (button == 's')											//выбераем другую кнопку меню
			NamberParagraph = Menu_СhooseParagraph(NamberParagraph + 1);//
		else if (button == ' ')											//
		{
			if (NamberParagraph == 0)		//игра
			{
				Menu_Game(height, length, mins);
				SelectObject(hDC, (HFONT)CreateFontIndirect(&font_menu));//вернем шрифт меню
				MoveWindow(hWnd, 125, 125, 33 + 700, 39 + 500, FALSE);//вернем окно к размеру меню
				SetTextColor(hDC, RGB(255, 255, 255));
			}
			else if (NamberParagraph == 1)	//настройки
				Menu_Settings(&height,  &length, &mins);
			else							//выход
			{
				ReleaseDC(hWnd, hDC);
				return 0;
			}
		}
	}
}
void Menu_DrawMenu()
{
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = 700;
	Rect.bottom = 500;
	FillRect(hDC, &Rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
	Rect.left = 150;
	Rect.right = 550;
	for (int y = 50; y < 500; y = y + 150)
	{
		Rect.top = y;
		Rect.bottom = 75 + y;
		FillRect(hDC, &Rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
	}
	Rect.top = 75;
	DrawText(hDC, (LPCTSTR)"GAME", 4, &Rect, DT_CENTER);
	Rect.top = 225;
	DrawText(hDC, (LPCTSTR)"SETTINGS", 8, &Rect, DT_CENTER);
	Rect.top = 375;
	DrawText(hDC, (LPCTSTR)" EXIT", 6, &Rect, DT_CENTER);
}
void Menu_DrawPointer(int NamberParagraph)
{
	Rect.top = 50 + NamberParagraph * 150;
	Rect.bottom = 125 + NamberParagraph * 150;
	FrameRect(hDC, &Rect, RedBrush);
}
int  Menu_СhooseParagraph(int NewNamberParagraph)
{
	if (NewNamberParagraph > 2)
		NewNamberParagraph = NewNamberParagraph - 3;
	else if (NewNamberParagraph < 0)
		NewNamberParagraph = NewNamberParagraph + 3;
	return NewNamberParagraph;
}
int  Menu_Settings(int* height, int* length, int* mins)
{
	int NamberParagraph = 0;
	char button;
	while (1)
	{
		Settings_DrawSettings(height, length , mins );//рисуем настройки
		Settings_DrawPointer(NamberParagraph);//Рисуем выделение кнопки настроек
		button = _getch();
		if (button == 'w')
			NamberParagraph = Settings_СhooseParagraph(NamberParagraph - 1);//
		else if (button == 's')												//выбераем другую кнопку настроек
			NamberParagraph = Settings_СhooseParagraph(NamberParagraph + 1);//
		else if (button == 'a')												
			Settings_EditOptions(NamberParagraph, -1, height, length, mins);//
		else if (button == 'd')												//меняем длину/высоту/мины
			Settings_EditOptions(NamberParagraph, 1, height, length, mins); //
		else if ((button == ' ')&&(NamberParagraph == 3))	//выход			
			return 0;														
	}
}
void	  Settings_DrawSettings( int* height, int* length, int* mins)
{
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = 700;
	Rect.bottom = 500;
	FillRect(hDC, &Rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

	Rect.left = 400;
	Rect.right = 550;
	for (int y = 45; y< 300; y = y + 125)
	{
		Rect.top = y;
		Rect.bottom = 50 + y;
		FillRect(hDC, &Rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
	}

	char str[4];
	Rect.top = 55;
	sprintf_s(str, "%i", *length);
	DrawText(hDC, str, strlen(str), &Rect, DT_CENTER);

	Rect.top = 180;
	sprintf_s(str, "%i", *height);
	DrawText(hDC, str, strlen(str), &Rect, DT_CENTER);

	Rect.top = 305;
	sprintf_s(str, "%i", *mins);
	DrawText(hDC, str, strlen(str), &Rect, DT_CENTER);

	Rect.left = 100;
	Rect.right = 350;
	for (int y = 30; y<500; y = y + 125)
	{
		Rect.top = y;
		Rect.bottom = 75 + y;
		FillRect(hDC, &Rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
	}
	Rect.top = 55;
	DrawText(hDC, (LPCTSTR)"Columns", 7, &Rect, DT_CENTER);
	Rect.top = 180;
	DrawText(hDC, (LPCTSTR)"Lines", 5, &Rect, DT_CENTER);
	Rect.top = 305;
	DrawText(hDC, (LPCTSTR)"Mines", 5, &Rect, DT_CENTER);
	Rect.top = 430;
	DrawText(hDC, (LPCTSTR)" BACK", 6, &Rect, DT_CENTER);
}
void	  Settings_DrawPointer( int NamberParagraph)
{
	Rect.top = 30 + NamberParagraph * 125;
	Rect.bottom = 105 + NamberParagraph * 125;
	FrameRect(hDC, &Rect, RedBrush);
}
int		  Settings_СhooseParagraph(int NewNamberParagraph)
{
	if (NewNamberParagraph > 3)
		NewNamberParagraph = NewNamberParagraph - 4;
	else if (NewNamberParagraph < 0)
		NewNamberParagraph = NewNamberParagraph + 4;
	return NewNamberParagraph;
}
void	  Settings_EditOptions(int NamberParagraph, int edit, int* height, int* length, int* mins)
{
	if (NamberParagraph == 3)
		return;
	if ((NamberParagraph == 0) && (*length + edit > 0) && (*length + edit < 30) && ((*length + edit)*(*height) >= *mins ))//возможны ли такие изменения 
		*length = *length + edit;
	else if ((NamberParagraph == 1) && (*height + edit > 0) && (*height + edit <30) && ((*length)*(*height + edit) >= *mins ))//возможны ли такие изменения
		*height = *height + edit;
	else if ((NamberParagraph == 2) && (*mins + edit >= 0) && ((*length)*(*height) >= *mins + edit))//возможны ли такие изменения
		*mins = *mins + edit;

}
void Menu_Game( int height, int length, int mins)
{
	Cell *Field = Game_GenerateFied( height, length, mins);//рандомно заполняем масив структур Cell
	Game_DrawStartField(height, length);//рисуем стартовое поле
	int NamberCell = 0,counter = 0;//номер клетки с которой мы начинаем и счетчик открытых клеток
	char button;
	while (1)//Цикл считывания клавиатуры
	{
		Game_DrawPointer((NamberCell % length) * 30, int(NamberCell / length) * 30);//рисуем указатель(красный квадратик)
		button = _getch();
		 	 if (button == 'w')
			NamberCell = Game_СhoiceCell(NamberCell, NamberCell - length, height, length);//указатель вверх
		else if (button == 's')
			NamberCell = Game_СhoiceCell(NamberCell, NamberCell + length, height, length);//указатель вниз
		else if (button == 'a')
			NamberCell = Game_СhoiceCell(NamberCell, NamberCell - 1, height, length);//указатель вправо
		else if (button == 'd')
			NamberCell = Game_СhoiceCell(NamberCell, NamberCell + 1, height, length);//указатель влево
		else if (button == 'q')
			Field[NamberCell].status = Game_MarkCell(Field[NamberCell].status, (NamberCell % length) * 30, int(NamberCell / length) * 30);//маркировка клетки(кнопка q)(флажок)
		else if (button == ' ')
		{
			counter = Game_OpenCell(NamberCell, counter, &Field[0], height, length);//команда открыть клетку(пробел)
			if ((counter == -1) || (counter >= height*length - mins))//проверка на конец игры
				break;
		}
	}
	Game_DrawEnd(&Field[0], height, length, counter);//открытие поля и результат
	_getch();
}
Cell*	  Game_GenerateFied(int height, int length, int mins)
{
	Cell *Field = new Cell[height*length];
	int NamberCell = 0, MinsCounter = 0;//номер клетки и счетчик мин
	srand(time(NULL));//для правильного  рандома
	for (MinsCounter = 0; MinsCounter < mins; MinsCounter++)
	{
		NamberCell = rand() % (height*length);//рандомно выбераем клетку поля
		if (Field[NamberCell].value == 'm')//когда в клетке уже есть мина
			MinsCounter--;
		else
		{
			Field[NamberCell].value = 'm';//ставим мину
			for (int i = -1; i<2; i++)													// 
				for (int j = -1; j<2; j++)												// увеличиваем количество мин
					if ((int((NamberCell + i) / length) == int(NamberCell / length)) &&	// у всех соседних клеток
						(NamberCell + j*length + i<height*length) &&					// если они не мины
						(NamberCell + j*length + i >= 0) &&								// и не выходят за границы поля
						(Field[NamberCell + j*length + i].value != 'm'))				//
					Field[NamberCell + j*length + i].value++;							//
		}
	}
	return &Field[0];
}
void	  Game_DrawStartField(int height, int length)
{
	static LOGFONT font_game;
	font_game.lfWeight = 800; //толщина
	SelectObject(hDC, (HFONT)CreateFontIndirect(&font_game));

	MoveWindow(hWnd, 150, 150, 34 + length * 30, 40 + height * 30, FALSE);//меняем размер окна 
	SetBkColor(hDC, RGB(255, 255, 255));
	SetTextColor(hDC, RGB(0, 0, 0));
	SelectObject(hDC, GreyPen);
	SelectObject(hDC, (HPEN)GetStockObject(GRAY_BRUSH));
	for (int j = 0; j < length * 30; j = j + 30)
		for (int i = 0; i < height * 30; i = i + 30)
			Rectangle(hDC, j, i, 31 + j, 31 + i);
	SelectObject(hDC, RedPen);
	Rectangle(hDC, 0, 0, 31, 31);
}
void	  Game_DrawPointer(int x, int y)
{
	SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
	SelectObject(hDC, RedPen);
	Rectangle(hDC, x, y, 31 + x, 31 + y);
}
int		  Game_СhoiceCell(int OldNamberCell, int NewNamberCell,int height, int length)
{
	if ((NewNamberCell >= 0) && (NewNamberCell < height*length))
	{
		SelectObject(hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
		SelectObject(hDC, GreyPen);
		Rectangle(hDC,	(OldNamberCell % length) * 30,
						int(OldNamberCell / length) * 30,
						31 + (OldNamberCell % length) * 30,
						31 + int(OldNamberCell / length) * 30);
		return NewNamberCell;
	}
	else
		return OldNamberCell;
}
int		  Game_MarkCell( char status, int x, int y)
{
	if (status == 0)//клетка открыта
		return 0;
	if (status == 1)//ставим флажок
	{
		SelectObject(hDC, GreyPen);
		SelectObject(hDC, RedBrush);
		Rectangle(hDC, x, y, 31 + x, 31 + y);
		return 2;
	}
	else//убираем флажок
	{
		SelectObject(hDC, GreyPen);
		SelectObject(hDC, (HPEN)GetStockObject(GRAY_BRUSH));
		Rectangle(hDC, x, y, 31 + x, 31 + y);
		return 1;
	}
};
int		  Game_OpenCell( int NamberCell, int counter, Cell * Field, int height, int length)
{
	if (Field[NamberCell].status == 1)																		 //закрыта ли клетка
	{
		if (Field[NamberCell].value == 'm')																	 //проверка на мину
			return -1;																						 // когда мина
		else 
		{
			OpenCell_DrawCell(&Field[NamberCell], (NamberCell % length) * 30, int(NamberCell / length) * 30);//нарисавать циферку на клетке
			if (Field[NamberCell].value == '0')																 //если вокруг клетки 0 мин
				counter = OpenCell_OpenCellsAround(NamberCell, counter, &Field[0], height, length);			 //то открываем все клетки рядом с нашей которые не выходят за границы поля
			return counter + 1;
		}
	}
	else 
	if(OpenCell_CheckMarkAround(NamberCell, counter, Field, height, length))								 //количество флажков вокруг == количеству мин вокруг
		counter = OpenCell_OpenCellsAround(NamberCell, counter, &Field[0], height, length);					 //открываем все клетки рядом с нашей которые не выходят за границы поля
	return counter;
}	  
void		   OpenCell_DrawCell( Cell * CurrentCell, int x, int y)
{
	CurrentCell->status = 0;
	SelectObject(hDC, (HBRUSH)GetStockObject(WHITE_BRUSH));
	SelectObject(hDC, GreyPen);
	Rectangle(hDC, x, y, 31 + x , 31 + y );
	Rect.left = x;
	Rect.top = y+7;
	Rect.right = 31 + x;
	Rect.bottom = 31 + y;
	if (CurrentCell->value == '0')
		return;
	if (CurrentCell->value == 'm')
	{
		SetTextColor(hDC, RGB(255, 255, 255));
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, (HPEN)GetStockObject(WHITE_PEN));
		Rectangle(hDC, x, y, 31 + x, 31 + y);
	}
	DrawText(hDC, &(CurrentCell->value), 1, &Rect, DT_CENTER);
	SetTextColor(hDC, RGB(0, 0, 0));
}
int			   OpenCell_OpenCellsAround(int NamberCell, int counter, Cell * Field, int height, int length)
{
	for(int i=-1;i<2;i++)
		for(int j=-1;(j<2)&&(counter!=-1);j++)
			if(	(int((NamberCell+i)/length)== int(NamberCell / length))&&							//проверка на то что ячейки рядом не вышли за поле и ячейка закрыта
				(NamberCell+j*length+i<height*length) && 
				(NamberCell + j*length + i>=0) && 
				( j == int((NamberCell + j*length + i)/ length)-int(NamberCell / length)) &&
				(Field[NamberCell + j*length + i].status == 1)) 
			counter= Game_OpenCell	(NamberCell + j*length + i, counter, &Field[0], height, length);
	return counter;
}
int			   OpenCell_CheckMarkAround(int NamberCell, int counter, Cell * Field, int height, int length)
{
	int MarkCounter = 0;
	if ((Field[NamberCell].status == 0) && //открыта
		(Field[NamberCell].value != '0'))  //и не пустая
	{
		for (int i = -1; i < 2; i++)																//
			for (int j = -1; j < 2; j++)															//
				if ((int((NamberCell + i) / length) == int(NamberCell / length)) &&					//
					(NamberCell + j*length + i < height*length) &&									//
					(NamberCell + j*length + i >= 0) &&												//считаем сколько флажков рядом
					(j == int((NamberCell + j*length + i) / length) - int(NamberCell / length) &&	//
					(Field[NamberCell + j*length + i].status == 2)))								//
					MarkCounter++;																	//
		if (MarkCounter == Field[NamberCell].value - '0')
			return 1;
	}
	return 0;
}
void	  Game_DrawEnd( Cell * Field, int height, int length , int counter)
{
	MoveWindow(hWnd, 150, 150, 34 + length * 30, 80 + height * 30, FALSE);//меняем размер окна
	Sleep(50);//костыль
	for (int NamberCell = 0; NamberCell<height*length; NamberCell++)
		OpenCell_DrawCell(&Field[NamberCell], (NamberCell % length) * 30, int(NamberCell / length) * 30);
	Rect.left = 0;
	Rect.top = 10 + height * 30;
	Rect.right = length * 30;
	Rect.bottom = 40 + height * 30;
	SelectObject(hDC, RedPen);
	SelectObject(hDC, RedBrush);
	Rectangle(hDC, -1 , 1 + height * 30, 1 + length * 30, 41 + height * 30);
	if (counter == -1)
	{
		SelectObject(hDC, (HPEN)GetStockObject(BLACK_PEN));
		SelectObject(hDC, RedBrush);
		Rectangle(hDC, 0, 1 + height * 30, 1 + length * 30, 41 + height * 30);
		SetTextColor(hDC, RGB(255, 255, 255));
		DrawText(hDC, (LPCTSTR)" LOSE!!!", 9, &Rect, DT_CENTER);
	}
	else
	{
		SelectObject(hDC, (HPEN)GetStockObject(BLACK_PEN));
		SelectObject(hDC, YellowBrush);
		Rectangle(hDC, 0 , 1 + height * 30, 1 + length * 30, 41 + height * 30);
		SetTextColor(hDC, RGB(255, 255, 255));
		DrawText(hDC, (LPCTSTR)" WIN!!!!", 9, &Rect, DT_CENTER);
	}
	delete[]Field;
}