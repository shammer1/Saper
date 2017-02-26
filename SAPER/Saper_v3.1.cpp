/*Saper v3.1*/
#include<windows.h>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<stdio.h>
LRESULT CALLBACK Function_WNDCLASS_Game(HWND, UINT, WPARAM, LPARAM);//�������� ������� ����
bool Primary_options();
bool Open_Settings();
bool Open_Menu();
bool Open_Game();
bool Settings_button_function(int);
bool Menu_button_function(int);
HINSTANCE Descriptor_Program;

class MENU_Window
{
public:
	HWND Descriptor;						//���������� ����
	HBRUSH Background_BRASH ;				//���� ����
	HBRUSH Buttom_Fill_BRASH ;				//���� ������
	HBRUSH Buttom_Frame_BRASH ;				//���� ������� ������
	int Amount_button;						//���������� ������ � ����
	int Amount_coment;						//���������� ����������� � ����
	struct BUTTON
	{
		int x1, x2, y1, y2;
		char text[10];
		bool(*funfunction)(int);
	}*Button;			//������ ����
	struct COMENT
	{
		int x1, x2, y1, y2;
		char text[100];
	}*Coment;			//���������� ����
						
	bool/*��� �������� ����*/ Draw()
	{
		PAINTSTRUCT Info_Paint;
		HDC �ontext = BeginPaint(Descriptor, &Info_Paint);			//�������� ��������
		SetBkMode(�ontext, TRANSPARENT);
		SetTextColor(�ontext, RGB(0, 0, 0));

		SelectObject(�ontext, Background_BRASH);
		Rectangle(�ontext, -1, -1, 500, 400);
		for (int N_button = 0; N_button<Amount_button; N_button++)  //������ ������
			Draw_Button(N_button, �ontext);
		for (int N_c = 0; N_c < Amount_coment; N_c++)				//������ ����������
			Draw_Coment(N_c, �ontext);

		EndPaint(Descriptor, &Info_Paint);							//����������� ��������
		ReleaseDC(Descriptor, �ontext);
		return 0;
	};
	bool/*��� �������� ������*/	Draw_Button(int N_button, HDC Context)
	{
		RECT Rect = { Button[N_button].x1 , Button[N_button].y1 , Button[N_button].x2 , Button[N_button].y2 };
		FillRect(Context, &Rect, Buttom_Fill_BRASH);
		FrameRect(Context, &Rect, Buttom_Frame_BRASH);
		DrawText(Context, Button[N_button].text, strlen(Button[N_button].text), &Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		return 0;
	}
	bool/*��� �������� ����������*/	Draw_Coment(int N_coment, HDC Context)
	{
		RECT Rect = { Coment[N_coment].x1 , Coment[N_coment].y1 , Coment[N_coment].x2 , Coment[N_coment].y2 };
		DrawText(Context, Coment[N_coment].text, strlen(Coment[N_coment].text), &Rect, DT_CENTER);
		return 0;
	}
	bool/*��� ��������� �� ������� ������*/	Click(int x , int y)
	{
		for (int i = 0; i < Amount_button; i++)
			if ((Button[i].x1 < x) && (x < Button[i].x2) && (y < Button[i].y2) && (Button[i].y1 < y))
			{
				Button[i].funfunction(i);
				break;
			}
		return 0;
	}

	~MENU_Window()
	{
		delete[] Button;
		delete[] Coment;
	}
}*Menu, *Settings;
class GAME
{
protected:
	int cell_size = 30, height = 12, length = 16, mins = 30, counter = 0;
public:
	HWND Descriptor;
	HWND LoseDescriptor;			//����������� ����
	HWND WinDescriptor;
	HBRUSH Cell_Mark_BRASH = CreateSolidBrush(RGB(220, 40, 40));
	HBRUSH Cell_Mina_BRASH = CreateSolidBrush(RGB(220, 40, 40));
	HBRUSH Cell_BRASH = (HBRUSH)LTGRAY_BRUSH;							//������
	HBRUSH Cell_Frame_BRASH = CreateSolidBrush(RGB(230, 230, 230));
	HBRUSH Win_BRASH = CreateSolidBrush(RGB(255, 255, 51));
	HBRUSH Lose_BRASH = CreateSolidBrush(RGB(220, 40, 40));
	struct Cell
	{
		int x, y;
		int status = 1;
		char value = '0';
	}**Field;			//������

	int Get_Options(char option)
	{
		switch (option)
		{
		case 'm':	return mins;
		case 'h':	return height;
		case 'l':	return length;
		case 's':	return cell_size;
		default:	PostQuitMessage(31);
		}
		return 1;
	}
	int Edit_Options(char option, int edit)
	{
		switch (option)
		{
		case 'm':
			if ((mins + edit >= 0) && (length*height >= mins + edit))							//�������� �� ����� ���������
				mins = mins + edit;
			break;
		case 'h':
			if ((height + edit > 0) && (height + edit <30) && (length*(height + edit) >= mins))	//�������� �� ����� ���������
				height = height + edit;
			break;
		case 'l':
			if ((length + edit > 0) && (length + edit < 30) && ((length + edit)*height >= mins))//�������� �� ����� ��������� 
				length = length + edit;
			break;
		case 'c':
			counter = 0;
			break;
		default:PostQuitMessage(32);
		}
		return 0;
	}

	bool GenerateFied()
	{
		int N_c, N_l;														//����� �������(Namber_Column) � ����� �����(Namber_line)
		Field = new Cell*[height];
		for (N_c = 0; N_c < height; N_c++)
			Field[N_c] = new Cell[length];
		for (N_c = 0; N_c < height; N_c++)									//��������� ��������� ������
			for (N_l = 0; N_l < length; N_l++)								//
			{																//
				Field[N_c][N_l].y = N_c*cell_size;							//
				Field[N_c][N_l].x = N_l*cell_size;							//
			}																//

		srand(time(NULL));													//��� ������� �������
		for (int MinsCounter = 0; MinsCounter < mins; MinsCounter++)
		{
			N_c = rand() % (height);										//�������� �������� ������ ����
			N_l = rand() % (length);
			if (Field[N_c][N_l].value == 'm')								//����� � ������ ��� ���� ����
				MinsCounter--;
			else
			{
				Field[N_c][N_l].value = 'm';								//������ ����
				for (int i = -1; i < 2; i++)
					for (int j = -1; j < 2; j++)
						if (												// ����������� ���������� ���
							(0 <= N_l + i) && (N_l + i < length) &&			// � ���� �������� ������
							(0 <= N_c + j) && (N_c + j < height) &&			// ���� ��� �� ����																				
							(Field[N_c + j][N_l + i].value != 'm')			// � �� ������� �� ������� ����
							)
							Field[N_c + j][N_l + i].value++;
			}
		}
		return 0;
	}
	bool Draw()
	{
		PAINTSTRUCT Info_Paint;
		HDC Context = BeginPaint(Descriptor, &Info_Paint);
		SetBkMode(Context, TRANSPARENT);
		SetTextColor(Context, RGB(0, 0, 0));
		SelectObject(Context, Cell_BRASH);

		for (int N_c = 0; N_c<height; N_c++)
			for (int N_l = 0; N_l<length; N_l++)
				Draw_Cell(N_c, N_l, Context);

		EndPaint(Descriptor, &Info_Paint);//����������� ��������
		ReleaseDC(Descriptor, Context);
		return 0;
	}
	bool Draw_Cell(int N_c, int N_l, HDC Context)					//����� �������(Namber_Column) � ����� �����(Namber_line)
	{	//������� � ������� ����� ���� ������
		RECT Rect = { Field[N_c][N_l].x , Field[N_c][N_l].y , Field[N_c][N_l].x + cell_size , Field[N_c][N_l].y + cell_size };
		switch (Field[N_c][N_l].status)
		{
		case 0:													//�������					
			if (Field[N_c][N_l].value == '0')
				break;
			if (Field[N_c][N_l].value == 'm')
				FillRect(Context, &Rect, Cell_Mina_BRASH);
			DrawText
			(Context, &Field[N_c][N_l].value, 1, &Rect,
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;
		case 1:													//�������
			FillRect(Context, &Rect, Cell_BRASH);
			break;
		case 2:													//������
			FillRect(Context, &Rect, Cell_Mark_BRASH);
			break;
		default:PostQuitMessage(7);
		}
		FrameRect(Context, &Rect, Cell_Frame_BRASH);
		return 0;
	}
	bool Open_Cell(int N_c, int N_l)											 //����� �������(Namber_Column) � ����� �����(Namber_line)
	{
		if (Field[N_c][N_l].status == 1)
		{
			if (Field[N_c][N_l].value == 'm') End(0);							 //������ �� ����
			else
			{
				Field[N_c][N_l].status = 0;										 //��������� ������
				if (Field[N_c][N_l].value == '0')								 //���� ������ ������ 0 ���
					Open_CellsAround(N_c, N_l);									 //�� ��������� ��� ������ ����� � ����� ������� �� ������� �� ������� ����
				counter++;
				RECT Rect = { Field[N_c][N_l].x , Field[N_c][N_l].y , Field[N_c][N_l].x + cell_size , Field[N_c][N_l].y + cell_size };
				InvalidateRect(Descriptor, &Rect, TRUE);						 //��������� �������� ������
			}
			if (counter == height*length - mins) End(1);						 //�������� �� ������
		}
		else
			if ((Field[N_c][N_l].status == 0) && (Field[N_c][N_l].value != '0'))	 //�������� �� �������
			{
				int Mark_counter = 0;
				for (int i = -1; i < 2; i++)
					for (int j = -1; j < 2; j++)
						if ((0 <= N_l + i) && (N_l + i < length) &&				 //������� ���������� ������� ����� � �������
							(0 <= N_c + j) && (N_c + j < height) &&
							(Field[N_c + j][N_l + i].status == 2))
							Mark_counter++;
				if (Mark_counter == Field[N_c][N_l].value - '0')					 //��������� ��� ������ �����
					Open_CellsAround(N_c, N_l);
			}
		return 0;
	}
	bool Open_CellsAround(int N_c, int N_l)				//����� �������(Namber_Column) � ����� �����(Namber_line)
	{
		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
				if ((0 <= N_l + i) && (N_l + i < length) &&
					(0 <= N_c + j) && (N_c + j < height) &&		//��������� ��� ������ �����
					(Field[N_c + j][N_l + i].status == 1))	//������� �� ������� �� ������� ����
					Open_Cell(N_c + j, N_l + i);
		return 0;
	}
	bool Mark_Cell(int N_c, int N_l)
	{
		if (Field[N_c][N_l].status != 0)				//������ �� �������
		{
			if (Field[N_c][N_l].status == 1)			//������ ������
				Field[N_c][N_l].status = 2;
			else										//������� ������
				Field[N_c][N_l].status = 1;

			RECT Rect = { Field[N_c][N_l].x , Field[N_c][N_l].y , Field[N_c][N_l].x + cell_size , Field[N_c][N_l].y + cell_size };
			InvalidateRect(Descriptor, &Rect, FALSE);   //��������� ������
		}
		return 0;
	}
	bool End(int status_game)
	{
		for (int N_c = 0; N_c < height; N_c++)
			for (int N_l = 0; N_l < length; N_l++)
				Field[N_c][N_l].status = 0;								//������� ���
		RECT Rect;
		GetWindowRect(Menu->Descriptor, &Rect);							//������ ��������� ����
		MoveWindow(Menu->Descriptor,
			Rect.left, Rect.top,								//��������� ������ �������� ����
			length*cell_size + 16, height*cell_size + 89,		//������ � ������
			FALSE											);	//�����������
		InvalidateRect(Game->Descriptor, NULL, TRUE);
		HWND EndDescriptor = CreateWindow( "Game_WNDCLASS", "End Game",
										WS_CHILD,
										0, height*cell_size,			//���������							
										length*cell_size, 50,			//������/������											
										Menu->Descriptor,				//������������ ����						
										NULL,
										Descriptor_Program,
										NULL						);
		if (EndDescriptor == NULL)	PostQuitMessage(6);
		if (status_game == 0)
			LoseDescriptor = EndDescriptor;							//������ ��� ��������(2 ������ ����)
		else
			WinDescriptor = EndDescriptor;
		ShowWindow(EndDescriptor, SW_SHOW);
		UpdateWindow(EndDescriptor);
		return 0;
	}
	bool Draw_End(int status_game, HWND EndDescriptor)					//�������� ������/��������
	{
		PAINTSTRUCT Info_Paint;
		HDC Context = BeginPaint(EndDescriptor, &Info_Paint);
		RECT Rect = { 0,0,length*cell_size,50 };
		SetBkMode(Context, FALSE);

		switch (status_game)
		{
		case 0:														//�������
			FillRect(Context, &Rect, Lose_BRASH);
			DrawText(Context, "LOSE", 4, &Rect,
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		case 1:														//������
			FillRect(Context, &Rect, Win_BRASH);
			DrawText(Context, "WIN", 3, &Rect,
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;

		default:break;
		}
		FrameRect(Context, &Rect, CreateSolidBrush(RGB(0, 0, 0)));

		EndPaint(EndDescriptor, &Info_Paint);
		ReleaseDC(EndDescriptor, Context);
		return 0;
	}
}*Game;

int WINAPI	WinMain(HINSTANCE descriptor_prog,HINSTANCE,LPSTR,int)
{
	Descriptor_Program = descriptor_prog;
	Primary_options();
	Open_Menu();
	MSG messge_code;
	while (GetMessage(&messge_code, NULL, 0, 0))	//�������� ��������� �� �������
	{
		TranslateMessage(&messge_code);				//����������� ��������� ������ � �������
		DispatchMessage(&messge_code);				//������� ��������� ��������������� ������� ����(Function_WNDCLASS_Game)
	}
	return(messge_code.wParam);
}
bool Primary_options()
{
	WNDCLASS W_Class;								//������ ��������� ��������� WNDCLASS
	W_Class.lpszClassName = "Game_WNDCLASS";		//��� ������
	W_Class.hInstance = Descriptor_Program;			//������������� �������� ����������
	W_Class.lpfnWndProc = Function_WNDCLASS_Game;	//��������� �� ������� ����
	W_Class.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//���� ���� ����
	W_Class.hCursor = NULL;							//������
	W_Class.hIcon = NULL;							//������ � ��� �� ����� ����
	W_Class.lpszMenuName = NULL;					//� ���� ���� �� �����
	W_Class.style = NULL;							//�����
	W_Class.cbClsExtra = NULL;
	W_Class.cbWndExtra = NULL;
	RegisterClass(&W_Class);
	//��������� ���� � ���������
	Menu = new MENU_Window;
	*Menu = { NULL, CreateSolidBrush(RGB(224, 224, 224)), CreateSolidBrush(RGB(255, 255, 255)), CreateSolidBrush(RGB(255, 255, 51)) , 3 , 3 };	
	Menu->Button = new MENU_Window::BUTTON[Menu->Amount_button];
	Menu->Button[0] = { 25 ,150,50,100,"Play",Menu_button_function };
	Menu->Button[1] = { 25 ,150,150,200,"Settings",Menu_button_function };
	Menu->Button[2] = { 25 ,150,250,300,"Exit",Menu_button_function };
	Menu->Coment = new MENU_Window::COMENT[Menu->Amount_coment];
	Menu->Coment[0] = { 280,450,280,300,"����������� �����"};
	Menu->Coment[1] = { 200,400,120,140,"SAPER" };
	Menu->Coment[2] = { 200,470,133,153,"v 3.1" };
	Settings = new MENU_Window;
	*Settings = { NULL, CreateSolidBrush(RGB(224, 224, 224)), CreateSolidBrush(RGB(255, 255, 255)), CreateSolidBrush(RGB(255, 255, 51)) , 13 , 1 };
	Settings->Button = new MENU_Window::BUTTON[13];
	Settings->Button[0] =  { 35,135,25,75,"Mins",Settings_button_function };
	Settings->Button[1] =  { 35,135,100,150,"Length",Settings_button_function };
	Settings->Button[2] =  { 35,135,175,225,"Height",Settings_button_function };
	Settings->Button[3] =  { 35,135,250,300,"Back",Settings_button_function };
	Settings->Button[4] =  { 180,210,35,65,"-",Settings_button_function };
	Settings->Button[5] =  { 220,320,30,70,"30",Settings_button_function };
	Settings->Button[6] =  { 330,360,35,65,"+",Settings_button_function };
	Settings->Button[7] =  { 180,210,110,140,"-",Settings_button_function };
	Settings->Button[8] =  { 220,320,105,145,"12",Settings_button_function };
	Settings->Button[9] =  { 330,360,110,140,"+",Settings_button_function };
	Settings->Button[10] = { 180,210,185,215,"-",Settings_button_function };
	Settings->Button[11] = { 220,320,180,220,"16",Settings_button_function };
	Settings->Button[12] = { 330,360,185,215,"+",Settings_button_function };
	Settings->Coment = new MENU_Window::COMENT;
	*Settings->Coment = { 240,450,270,300,"��������� ���������� ����" };
	Game= new GAME;
	return 0;
}
bool Open_Menu()
{
	Menu->Descriptor = CreateWindow
	(
		"Game_WNDCLASS",											//��� ������
		"Saper",													//��������� ����
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//����� ����
		100,														//��������� ���� �� ������ �� �
		100,														//��������� �� �
		500,														//������
		400,														//������
		NULL,														//������������� ������������� ����
		NULL,														//������������� ����
		Descriptor_Program,											//������������� ���������� ���������
		NULL														//���������� �������������� ����������
	);
	ShowWindow(Menu->Descriptor, SW_SHOW);				//������� ���� �� ������ �� �����
	UpdateWindow(Menu->Descriptor);						//������� ���������� ����
	return 0;
}
bool Open_Settings()
{
	Settings->Descriptor = CreateWindow
	(
		"Game_WNDCLASS",	//��� ������
		"Settings",			//��������� ����
		WS_CHILD,			//����� ����
		0,0,500,400,		//��������� � ������ ������
		Menu->Descriptor,	//������������� ������������� ����
		NULL,				//������������� ����
		Descriptor_Program, //������������� ���������� ���������
		NULL				//���������� �������������� ����������
	);
	ShowWindow(Settings->Descriptor, SW_SHOW);	//������� ���� �� ������ �� �����
	UpdateWindow(Settings->Descriptor);			//������� ���������� ����
	return 0;
}
bool Open_Game()
{
	Game->Edit_Options('c', 0);
	Game->GenerateFied();
	RECT Rect;
	GetWindowRect(Menu->Descriptor, &Rect);
	MoveWindow(	Menu->Descriptor,
				Rect.left, Rect.top,								//���������
				Game->Get_Options('l')*Game->Get_Options('s')+16,	//������
				Game->Get_Options('h')*Game->Get_Options('s')+39,	//������
				FALSE											 ); //�����������
	Game->Descriptor = CreateWindow
	(
		"Game_WNDCLASS",											//��� ������
		"Game",														//��������� ����
		WS_CHILD,													//����� ����
		0,															//��������� ���� �� ������ �� �
		0,															//��������� �� �
		Game->Get_Options('l')*Game->Get_Options('s'),				//������
		Game->Get_Options('h')*Game->Get_Options('s'),				//������
		Menu->Descriptor,											//������������� ������������� ����
		NULL,														//������������� ����
		Descriptor_Program,											//������������� ���������� ���������
		NULL														//���������� �������������� ����������
	);
	if (!Game->Descriptor) PostQuitMessage(6);
	ShowWindow(Game->Descriptor, SW_SHOW);							//������� ���� �� ������ �� �����
	UpdateWindow(Game->Descriptor);									//������� ���������� ����
	return 0;
}
bool Settings_button_function(int N_button)
{
	RECT Rect;
	switch (N_button)
	{
		case 3:
			DestroyWindow(Settings->Descriptor);
			return 0;

		case 4:
		case 6:
			if(N_button==4) Game->Edit_Options('m', -1);
			else Game->Edit_Options('m', 1);
			sprintf_s(Settings->Button[5].text, "%i", Game->Get_Options('m'));
			Rect = { Settings->Button[5].x1 , Settings->Button[5].y1 , Settings->Button[5].x2 , Settings->Button[5].y2 };
			InvalidateRect(Settings->Descriptor, &Rect, TRUE);
			return 0;
		case 7:
		case 9:
			if(N_button==7)Game->Edit_Options('h', -1);
			else Game->Edit_Options('h', 1);
			sprintf_s(Settings->Button[8].text, "%i", Game->Get_Options('h'));
			Rect = { Settings->Button[8].x1 , Settings->Button[8].y1 , Settings->Button[8].x2 , Settings->Button[8].y2 };
			InvalidateRect(Settings->Descriptor, &Rect, TRUE);
			return 0;
		case 10:
		case 12:
			if(N_button ==10) Game->Edit_Options('l', -1);
			else Game->Edit_Options('l', 1);
			sprintf_s(Settings->Button[11].text, "%i", Game->Get_Options('l'));
			Rect = { Settings->Button[11].x1 , Settings->Button[11].y1 , Settings->Button[11].x2 , Settings->Button[11].y2 };
			InvalidateRect(Settings->Descriptor, &Rect, TRUE);
			return 0;
		default :break;
	}
	return 0;
}
bool Menu_button_function(int N_button)
{
	switch (N_button)
	{
		case 0:
			Open_Game();
			return 0;
		case 1:
			Open_Settings();
			return 0;
		case 2:
			PostQuitMessage(0);
			return 0;
		default: PostQuitMessage(1);
	}
	return 1;
}
LRESULT CALLBACK Function_WNDCLASS_Game(HWND Descriptor, UINT messge, WPARAM wParam, LPARAM lParam)//������� ����
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	if(Descriptor== Menu->Descriptor)
		switch (messge)
		{	case WM_PAINT:
				Menu->Draw();
				if (Settings->Descriptor != 0)InvalidateRect(Settings->Descriptor, NULL, FALSE);
				if (Game->Descriptor != 0)
				{
					InvalidateRect(Game->Descriptor, NULL, TRUE);
					if (Game->LoseDescriptor != 0)InvalidateRect(Game->LoseDescriptor, NULL, TRUE);
					if (Game->WinDescriptor != 0)InvalidateRect(Game->WinDescriptor, NULL, TRUE);
				}
				return 0;

			case WM_RBUTTONUP:
			case WM_LBUTTONUP:
				Menu->Click(LOWORD(lParam), HIWORD(lParam));
				return 0;
			
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			
			default: break;
		}
	else 
	if (Descriptor == Settings->Descriptor)
		switch (messge)
		{	case WM_PAINT:
				Settings->Draw();
				return 0;

			case WM_RBUTTONUP:
			case WM_LBUTTONUP:
				Settings->Click(LOWORD(lParam), HIWORD(lParam));
				return 0;
			
			case WM_DESTROY:
				return 0;	
			
			default: break;
		}
	else
	if (Descriptor == Game->Descriptor)
		switch (messge)
		{	case WM_PAINT:
				Game->Draw();
				return 0;

			case WM_RBUTTONUP:
				Game->Mark_Cell((int)(HIWORD(lParam) / Game->Get_Options('s')), (int)(LOWORD(lParam) / Game->Get_Options('s')));
				return 0;

			case WM_LBUTTONUP:
				Game->Open_Cell((int)(HIWORD(lParam) / Game->Get_Options('s')), (int)(LOWORD(lParam) / Game->Get_Options('s')));
				return 0;
		
			case WM_DESTROY:
				for (int N_c = 0; N_c < Game->Get_Options('h'); N_c++)
					delete[] Game->Field[N_c];
				delete[] Game->Field;
				RECT Rect;
				GetWindowRect(Menu->Descriptor, &Rect);
				MoveWindow(Menu->Descriptor, Rect.left, Rect.top, 500, 400, TRUE);
				return 0;
	
			default: break;
		}
	if ((Descriptor == Game->LoseDescriptor)||(Descriptor == Game->WinDescriptor))
		switch (messge)
		{	case WM_PAINT:
			if (Descriptor == Game->LoseDescriptor)
				Game->Draw_End(0, Descriptor);
			else
			if (Descriptor == Game->WinDescriptor)
				Game->Draw_End(1, Descriptor);
			return 0;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				DestroyWindow(Descriptor);
				DestroyWindow(Game->Descriptor);
				return 0;
			case WM_DESTROY:
				return 0;
			default:break;
		}
	return DefWindowProc(Descriptor, messge, wParam, lParam); //�������������
}