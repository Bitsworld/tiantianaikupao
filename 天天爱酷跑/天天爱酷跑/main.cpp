//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE		L"���찮����"	//Ϊ���ڱ��ⶨ��ĺ�

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC				g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;      //ȫ���豸�������������ȫ���ڴ�DC���
HBITMAP		g_hSprite, g_hBackGround = NULL;								//��������λͼ�����һ�����ڴ洢����ͼ��һ�����ڴ洢����ͼ
DWORD		g_tPre = 0, g_tNow = 0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int					g_iNum = 0, g_iX = 0, g_iY = 0;                //g_iNum������¼ͼ��,g_iX,g_iY�ֱ��¼��ͼ�ĺ�������
int x = 800;
//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL						Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID							Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL						Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������
BOOL                      jump = false;
int a = 5, v0;
//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"ttkp.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

																//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

											//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	PlaySound(L"����.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

																	 //��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	while (msg.message != WM_QUIT)		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			g_tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
			if (g_tNow - g_tPre >= 50)        //���˴�ѭ���������ϴλ�ͼʱ�����0.1��ʱ�ٽ����ػ����
				Game_Paint(hwnd);
		}

	}

	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)						//switch��俪ʼ
	{
	case WM_KEYDOWN:
		switch (wParam)// ���Ǽ��̰�����Ϣ
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		case VK_UP:

			if (jump == false)
			{
				jump = true;
				v0 = -36;
			}
			break;
		case VK_DOWN:
			break;
		case VK_LEFT:
			x -= 10;
			if (g_iX < 0)
				g_iX = 0;
			break;
		case VK_RIGHT:
			x -= 20;
			if (g_iX > WINDOW_WIDTH - 123)
				g_iX = WINDOW_WIDTH - 123;
			break;
		}
		break;//������switch���
	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	HBITMAP bmp;
	g_hdc = GetDC(hwnd);  //��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);  //����һ����hdc���ݵ��ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//�ٴ���һ����hdc���ݵĻ���DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����
//	wchar_t   filename[20];//����λͼ����

	SelectObject(g_mdc, bmp);
	//�����������λͼ
	/*for (int i = 0; i<12; i++)
	{
	memset(filename, 0, sizeof(filename));  //filename�ĳ�ʼ��
	swprintf_s(filename, L"jqxs%d.bmp", i);  //����swprintf_s����������װ������Ӧ��ͼƬ�ļ�����
	g_hSprite[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 123, 105, LR_LOADFROMFILE);
	}*/
	g_hSprite = (HBITMAP)LoadImage(NULL, L"����.bmp", IMAGE_BITMAP, 2952, 105, LR_LOADFROMFILE);//����·��ͼ
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"����1.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);//���ر���ͼ

	//-----��λͼ�����Ĳ���֮������������DC��-----
	//g_mdc = CreateCompatibleDC(g_hdc);    //���������豸�������ڴ�DC
	g_iNum = 0;//����ͼ����
	g_iX = 50;	//��ͼ��ʼX����
	g_iY = 200;    //��ͼ��ʼY����

	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)//xΪ������ͼ��ʼλ��
{
	if (x < 0)
		x = WINDOW_WIDTH;
	if (g_iNum == 12)               //�ж��Ƿ񳬹����ͼ�ţ����������ͼ�š�10��������ʾͼ������Ϊ"0"��
		g_iNum = 0;
	SelectObject(g_bufdc, g_hBackGround);
	//BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);
	//����ѭ����ͼ����
	BitBlt(g_mdc, 0, 0, x, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH - x, 0, SRCCOPY);
	BitBlt(g_mdc, x, 0, WINDOW_WIDTH - x, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	//TransparentBlt(g_mdc, 0, 200, x, 130, g_bufdc, WINDOW_WIDTH -x, 580, x, 130, RGB(255, 255, 255));
	//TransparentBlt(g_mdc, x, 200, WINDOW_WIDTH - x, 130, g_bufdc, 0, 200, WINDOW_WIDTH - x, 130, RGB(255, 255, 255));
	//BitBlt(g_mdc, 0, 200, WINDOW_WIDTH, 130, g_bufdc, 0, 580, SRCCOPY);
	SelectObject(g_bufdc, g_hSprite);//g_hSprite����λͼ

	switch (jump)
	{
	case true://��Ծ����
		g_iY = g_iY + v0;
		v0 = v0 + a;
		if (g_iY <200)
		{
			BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123 + 1476, 0, SRCAND);
			BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, SRCPAINT);
		}
		else
		{
			jump = false;
			g_iY = 200;
		}
		break;
	case false://��������
		BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123 + 1476, 0, SRCAND);
		BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, SRCPAINT);
		break;
	default:
		break;
	}
	/*if (jump)
	{
	g_iY = g_iY + v0;
	v0 = v0 + a;
	if(g_iY <200)
	{
	BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123 + 1476, 0, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, SRCPAINT);
	}
	else
	{
	jump = false;
	g_iY = 200;
	}

	}
	//TransparentBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, 123, 105, RGB(255, 255, 255));//����TransparentBlt͸��ɫ�ʷ�
	else
	{

	BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123 + 1476, 0, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, SRCPAINT);
	//BitBlt(g_mdc, g_iX, g_iY, 123, 105, g_bufdc, g_iNum * 123, 0, SRCCOPY); //��Ŀǰͼ�Ž��д���
	}*/

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);//����ͼ�ڼ���dc mdc�ϵ�ͼ����hdc����
	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ�䣬���´���Ϸѭ�����ж��Ƿ��Ѿ��ﵽ������²����趨��ʱ������
	g_iNum++;
	x -= 20;
	//����g_iNum��ֵ��1��Ϊ��һ��Ҫ��ʾ��ͼ��
	/*g_iX += 10;					   //�����´���ͼ������
	//�����ͼ���곬�����ڱ�Ե������������Ϊ-60
	if (g_iX >= WINDOW_WIDTH)
	g_iX = -60;*/
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	//for (int i = 0; i<12; i++)
	//DeleteObject(g_hSprite[i]);
	DeleteObject(g_hSprite);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}



