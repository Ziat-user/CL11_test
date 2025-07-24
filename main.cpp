/*******************************************************************************
*
*	�^�C�g���F	�o�g���Q�[���@�G���J�E���^�[�@���C������	[ main.cpp ]
*	Copyright (c) 2025 HAL TOKTO, ALL RIGHTS RESERVED.
*
*	�쐬�ҁF							�g�`�k�����@�������@�y���G��
*	�쐬���F		2025/06/15  ver 1.0 �}�b�v�\��
*   �C�����F		2025/06/15	ver 1.1 �G�Q�b�g�A���ɖ߂��ǉ�
*   �C�����F		2025/06/15	ver 1.2 �}�b�v�Ƀ��[���z�[����ǉ�
*   �C�����F		2025/06/22	ver 1.21 �f�B�X�v���C�̏������֐��ǉ��A�}�b�v�X�V
* 
*******************************************************************************/

// *****************************************************************************
// Preprocessor �v���v���Z�b�T �K�v�ȃw�b�_�t�@�C�����C���N���[�h
// *****************************************************************************
#include <iostream>
#include <conio.h>
#include <windows.h>
//#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*******************************************************************************
�}�N����`
*******************************************************************************/
#define V_MAX			(15)		/* �c�� */
#define H_MAX			(40)		/* ���� */
#define	ENEMY_MAX		(1)			/* �G�̍ő吔 */
#define MAX_enemy_HP	(50)		/* �G�̍ő�̗� */
#define MAX_player_HP	(50)		/* �v���C���[�̍ő�̗� */
#define WARP_IN_X		(-1)		/* �v���C���[�p���[�v�C��X���W */ 
#define WARP_IN_Y		(7)			/* �v���C���[�p���[�v�C��Y���W */ 
#define WARP_OUT_X		(40)		/* �v���C���[�p���[�v�A�E�gX���W */ 
#define WARP_OUT_Y		(7)			/* �v���C���[�p���[�v�A�E�gY���W */ 
#define KEY_FUNC		(0xe0)
#define KEY_UP			(0x48)
#define KEY_DOWN		(0x50)
#define KEY_RIGHT		(0x4d)
#define KEY_LEFT		(0x4b)

// *****************************************************************************
// ���ꂼ��̕��֐��̒��Ŏg�p����萔��ϐ�������main�֐��╛�֐��̊O�ɒu���Ă���
// *****************************************************************************
// Define constants �萔�̒�`
const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 25;

// Define variables
bool rtn = 0;
int ch = 0;

// *****************************************************************************
// Function prototype declarations �֐��̃v���g�^�C�v�錾
// *****************************************************************************
void SetCursorVisibility(BOOL visible);		// �J�[�\���̕\��/��\����ݒ肷��֐�
void MoveCursorToTop();		                // �J�[�\������ʂ̍���Ɉړ�����֐�
void clearInputBuffer();                    // �L�[�{�[�h���̓o�b�t�@�̃N���A�֐�
bool changeConsoleSize(int x, int y);       // �R���\�[���T�C�Y��ύX����֐�
bool CL11Startup();                         // �R���\�[���̏������֐�
bool CL11Cleanup();                         // �R���\�[���̃N���[���A�b�v�֐�
void HideConsoleTitleBar();                 // �R���\�[���̃^�C�g���o�[���B���֐�
void ShowConsoleTitleBar();                 // �R���\�[���̃^�C�g���o�[��\������֐�
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY); // �R���\�[���t�H���g�̐ݒ�֐�
void centerConsoleWindow(int screenWidth, int screenHeight);    // �R���\�[���E�B���h�E�𒆉��ɔz�u����֐�
void EnableVTMode();						// VT�V�[�P���X���[�h��L����
void SetTextColorRGB(int r, int g, int b);	//�R���\�[���E�B���h�E�̕\���F��RGB�`���ŕύX
void SetBackgroundColorRGB(int r, int g, int b);	//�R���\�[���E�B���h�E�̔w�i�F��RGB�`���ŕύX

/*******************************************************************************
���C���֐�
*******************************************************************************/
int main(void)
{
	//std::cout << "Change console window settings for CL11" << std::endl;
	//std::cout << "Press any key to start changing settings...\n" << std::endl;
	//ch = _getch();

	rtn = CL11Startup();
	//if (rtn != 1) {
	//    perror("CL11Startup error");
	//    return 0;  // FALSE
	//}

	// �J�[�\����\��
	SetCursorVisibility(FALSE);

	/* �ϐ��̐錾 */
	char map[V_MAX][H_MAX + 1] = 			/* ����+1���Ă���͕̂�����NULL�̕� */
	{
		//   0123456789012345678901234567890123456789	
			"****************************************",	// 0
			"*           *        *          *      *",	// 1
			"* ********  *        *          *      *",	// 2
			"*        *    *      *      ****       *",	// 3
			"*             * **** *           *     *",	// 4
			"*                                *******",	// 5
			"**   ****************          *      **",	// 6
			"                               *        ",	// 7
			"**         *                   *      **",	// 8
			"*          *                   *       *",	// 9
			"*          *  ****************         *",	// 0
			"*          *       *                   *",	// 1
			"*          *     * * *                 *",	// 2
			"*                *   *                 *",	// 3
			"****************************************",	// 4
	};
	int i;
	int player_x, player_y, old_x, old_y;
	int player_HP = MAX_player_HP;
	int player_damage = 0;
	int player_potion = 0;
	int enemy_x[ENEMY_MAX], enemy_y[ENEMY_MAX];
	int enemy_HP[ENEMY_MAX];
	int enemy_damage[ENEMY_MAX];
	int enemy_move;
	int enemy_get_count = 0;
	int key;

	/* �����_���p�����ݒ� */
	srand((unsigned)time(NULL));

	/* �v���C���[�̏����ݒ� */
	player_HP = MAX_player_HP - (rand() % 5 + 1); /* �v���C���[�̗̑͂�45�`50�̊� */
	player_x = 1;
	player_y = 1;
	map[player_y][player_x] = 'P';

	/* �G�̏����ݒ� */
	for (i = 0; i < ENEMY_MAX; i++)
	{
		enemy_x[i] = rand() % (H_MAX - 8) + 1;
		enemy_y[i] = rand() % (V_MAX - 6) + 5;
		map[enemy_y[i]][enemy_x[i]] = 'E';
		enemy_HP[i] = MAX_enemy_HP - (rand() % 5 + 1); /* �G�̗̑͂�45�`50�̊� */
		enemy_damage[i] = 0;
	}

	std::cout << "*****************************************************" << std::endl;
	std::cout << " �q�d�F�G�l�~�[�f�n�I ";
	std::cout << " -- �S���̓G���Q�b�g�������I --" << std::endl;
	std::cout << std::endl;
	std::cout << " Copyright(C) 2025 HAL TOKYO, All rights reserved." << std::endl;
	std::cout << std::endl;
	std::cout << "*****************************************************" << std::endl;
	std::cout << std::endl;
	std::cout << " E:�G  P:�v���C���[ �� �� �����ňړ�" << std::endl;
	std::cout << std::endl;
	std::cout << " �G���^�[�L�[�ŃX�^�[�g" << std::endl;
	EnableVTMode();
	// Set text color to white and background color to black �R���\�[���̃e�L�X�g�F��ΐݒ�
	SetTextColorRGB(0, 255, 0);

	while (1)
	{
		/* �L�[���͔��� */
		if (_kbhit())
		{
			// �L�[���͂��擾�i2�o�C�g�R�[�h�Ή��F����L�[�̏ꍇ��2��Ăяo���j
			key = _getch();
			if ((key == 0) || (key == KEY_FUNC))
			{
				key = _getch();
			}

			/* �O��̕\���ʒu�̏���*/
			old_x = player_x;
			old_y = player_y;
			map[player_y][player_x] = ' ';
			for (i = 0; i < ENEMY_MAX; i++)
			{
				map[enemy_y[i]][enemy_x[i]] = ' ';
			}

			/* �v���C���[�̈ʒu���� */
			switch (key)
			{
			case 'a':
			case KEY_LEFT:
			{
				player_x--;
				if ((player_x == WARP_IN_X) && (player_y == WARP_IN_Y))
				{
					player_x = WARP_OUT_X - 1;
					player_y = WARP_OUT_Y;
				}
				break;
			}

			case 'd':
			case KEY_RIGHT:
			{
				player_x++;
				if ((player_x == WARP_OUT_X) && (player_y == WARP_OUT_Y))
				{
					player_x = WARP_IN_X + 1;
					player_y = WARP_IN_Y;
				}
				break;
			}

			case 'w':
			case KEY_UP:
			{
				player_y--;
				break;
			}

			case 's':
			case KEY_DOWN:
			{
				player_y++;
				break;
			}

			default:
			{
				break;
			}
			} /* end of switch */

			// �ړ��������NG�ȏ��H
			if (map[player_y][player_x] == '*')
			{	/* NG�Ȃ�ړ��O�̏ꏊ�ɖ߂� */
				player_x = old_x;
				player_y = old_y;
			}

			/* �v���C���[�̈ʒu�X�V */
			map[player_y][player_x] = 'P';

			//*************************************************************************************
			/* �G�̈ړ��ʒu�X�V */
			for (i = 0; i < ENEMY_MAX; i++)
			{
				old_x = enemy_x[i];
				old_y = enemy_y[i];
				enemy_move = rand() % 5;	/* �ړ��������܂߂�̂łT */
				switch (enemy_move)
				{
				case 0:
				{
					enemy_x[i]--;
					if ((enemy_x[i] == WARP_IN_X) && (enemy_y[i] == WARP_IN_Y))
					{
						enemy_x[i] = WARP_OUT_X - 1;
						enemy_y[i] = WARP_OUT_Y;
					}
					break;
				}
				case 1:
				{
					enemy_x[i]++;
					if ((enemy_x[i] == WARP_OUT_X) && (enemy_y[i] == WARP_OUT_Y))
					{
						enemy_x[i] = WARP_IN_X + 1;
						enemy_y[i] = WARP_IN_Y;
					}
					break;
				}
				case 2:
				{
					enemy_y[i]--;
					break;
				}
				case 3:
				{
					enemy_y[i]++;
					break;
				}
				default:
				{
					break;
				}
				} /* end of switch */

				// �ړ��������NG�ȏ��H
				if (map[enemy_y[i]][enemy_x[i]] == '*')
				{	/* NG�Ȃ�ړ��O�̏ꏊ�ɖ߂� */
					enemy_x[i] = old_x;
					enemy_y[i] = old_y;
				}
				map[enemy_y[i]][enemy_x[i]] = 'E';
			}
			/* ��ʃN���A */
			std::cout << "\033[2J\033[H";

			// �f�o�b�O�p�@�v���C���[�ƓG�̍��W�\��
			 printf( " Player x:%2d y:%2d\n" , player_x, player_y );
			 for( i = 0 ; i < ENEMY_MAX ; i++ )
			 {
			 	 printf( " Enemy%d x:%2d y:%2d\n" , i , enemy_x[ i ] , enemy_y[ i ] );
			 }

			/* �}�b�v�`�揈�� */
			for (i = 0; i < V_MAX; i++)
			{
				std::cout << map[i] << std::endl;
			}
		}
		for (i = 0; i < ENEMY_MAX; i++)
		{
			if (player_x == enemy_x[i] && player_y == enemy_y[i])
			{
				std::cout << " �G" << i << " ����!!";
				std::cout << " �v���C���[�g�o:" << player_HP << "  �G" << i << "�̂g�o:" << enemy_HP[i] << std::endl;

				while ((player_HP >= 0) && (enemy_HP[i] >= 0))
				{
					printf(" �v���C���[�̃^�[�� �G%d ���U��", i);
					enemy_damage[i] = rand() % 10 + 11;
					enemy_HP[i] = enemy_HP[i] - enemy_damage[i];

					if (enemy_HP[i] <= 0)
					{
						printf(" �G%d �� %d �̃_���[�W���󂯂�\n", i, enemy_damage[i]);
						printf(" �G%d ��|���� �Q�b�g������!\n", i);
						enemy_get_count = enemy_get_count + 1;			/* �|�����G�̐����J�E���g�A�b�v */
						map[enemy_y[i]][enemy_x[i]] = ' ';		/*�|�����G�̌��݈ʒu��E������ */
						enemy_x[i] = 36;								/*�Q�b�g�����G���P�[�W�ɕۊ� */
						enemy_y[i] = 2;
						map[enemy_y[i]][enemy_x[i]] = 'E';		/*�P�[�W�̈ʒu����E���������� */

						printf(" �v���C���[�͓�̃|�[�V���������� ");
						player_potion = rand() % 30 + 1;				/* ��̖�̌��\��1�`30�@*/
						player_HP = player_HP + player_potion;
						printf(" �g�o�� %d �߂���", player_potion);
						printf(" �g�o:%d\n", player_HP);
						break;
					}
					else
					{
						printf(" �G%d �� %d �̃_���[�W���󂯂� �c��g�o:%d\n", i, enemy_damage[i], enemy_HP[i]);
						_getch();
					}

					printf(" �G%d �̃^�[�� �v���C���[���U��", i);
					player_damage = rand() % 10 + 11;
					player_HP = player_HP - player_damage;

					if (player_HP <= 0)
					{
						printf(" �v���C���[�� %d �̃_���[�W���󂯂�\n", player_damage);
						printf(" �v���C���[�͗͐s�����c\n");
						printf(" ���ɖ߂肾�c�c\n");

						player_damage = 0;
						player_HP = MAX_player_HP - (rand() % 5 + 1);
						enemy_HP[i] = MAX_enemy_HP - (rand() % 5 + 1);
						enemy_damage[i] = 0;
						map[player_y][player_x] = ' ';
						player_x = 1;
						player_y = 1;
						map[player_y][player_x] = 'P';
						break;
					}
					else
					{
						printf(" �v���C���[�� %d �̃_���[�W���󂯂� �c��g�o:%d\n", player_damage, player_HP);
						_getch();
					}
				}
				break;
			}
		}

		if (enemy_get_count == ENEMY_MAX)
		{
			rewind(stdin);
			getchar();

			// ��ʃN���A
			//system("cls");	
			std::cout << "\033[2J\033[H";

			std::cout << "*****************************************************" << std::endl;
			std::cout << std::endl;
			std::cout << " ���߂łƂ��I�~�b�V�����N���A�I" << std::endl;
			std::cout << std::endl;
			std::cout << " G A M E  O V E R" << std::endl;
			std::cout << std::endl;
			std::cout << "*****************************************************" << std::endl;
			std::cout << " �G���^�[�L�[�ŏI��" << std::endl;
			break;
		}
		/* �������x���� */
		/* _sleep(100); */
	}

	// MoveCursorToTop();
	system("cls");
	SetTextColorRGB(255, 255, 255);
	std::cout << "Press any key to revert console window settings to default" << std::endl;
	std::cout << "Revert console window settings to default" << std::endl;
	std::cout << "Press any key to start changing default settings...\n" << std::endl;
	ch = _getch();
	system("cls");
	rtn = CL11Cleanup();
	//if (rtn != 1) {
	//    perror("CL11Cleanup error");
	//    return 0; // FALSE
	//}

	rewind(stdin);
	getchar();
	return 0;
}

// *****************************************************************************
// ��ʐ���p�̕��֐�
// *****************************************************************************

// �J�[�\������֐�( TRUE:�\�� FALSE:��\��)
void SetCursorVisibility(BOOL visible) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// �J�[�\����擪�ɖ߂��֐�
void MoveCursorToTop() {
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to initialize the console �R���\�[���̏������֐�
bool CL11Startup() {
	// Change console size �R���\�[���T�C�Y�̕ύX
	if (!changeConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT)) {
		return FALSE;
	}
	// Hide the console title bar �R���\�[���̃^�C�g���o�[���B��
	HideConsoleTitleBar();

	// Get screen width and height ��ʂ̕��ƍ������擾
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(0, screen);

	// Set console font �R���\�[���t�H���g�̐ݒ�
	setConsoleFont(CONSOLE_WIDTH, CONSOLE_HEIGHT, screenWidth, screenHeight, dpiX, dpiY);
	// Center the console window �R���\�[���E�B���h�E�𒆉��ɔz�u
	centerConsoleWindow(screenWidth, screenHeight);
	return TRUE;
}

// Function to set the console font �R���\�[���t�H���g�̐ݒ�֐�
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY) {
	int fontSizeX = (screenWidth / width) * (dpiX / 96);
	int fontSizeY = (screenHeight / height) * (dpiY / 96);

	// Set font information �t�H���g���̐ݒ�
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 0}, FF_DONTCARE, FW_NORMAL, L"MS Gothic" };
	DISPLAY_DEVICE dd = { sizeof(dd) };
	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm = { sizeof(dm) };
	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	// Set font size �t�H���g�T�C�Y�̐ݒ�
	if (dm.dmPelsHeight > dm.dmPelsWidth) {
		cfi.dwFontSize.X = static_cast<SHORT>(fontSizeX);
		cfi.dwFontSize.Y = static_cast<SHORT>(floor(fontSizeX * 2));
	}
	else {
		cfi.dwFontSize.X = static_cast<SHORT>(floor(fontSizeY / 2));
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSizeY);
	}

	// Apply console font �R���\�[���t�H���g�̓K�p
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width) - 1, static_cast<SHORT>(height) - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

// Function to center the console window �R���\�[���E�B���h�E�𒆉��ɔz�u����֐�
void centerConsoleWindow(int screenWidth, int screenHeight) {
	HWND consoleWindow = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleWindow, &rect);
	MoveWindow(consoleWindow, (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// Function to clean up the console �R���\�[���̃N���[���A�b�v�֐�
bool CL11Cleanup() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Set console mode �R���\�[�����[�h�̐ݒ�
	SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);

	// Set console buffer size �R���\�[���o�b�t�@�T�C�Y�̐ݒ�
	COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	SetConsoleScreenBufferSize(hOut, bufferSize);
	CONSOLE_CURSOR_INFO cursorInfo = { 25, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);
	SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// Set font information �t�H���g���̐ݒ�
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 16}, FF_DONTCARE, FW_NORMAL, L"Consolas" };
	SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	// Reset console window position �R���\�[���E�B���h�E�̈ʒu�����Z�b�g
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowConsoleTitleBar();

	return TRUE;
}

// Function to change the console size �R���\�[���T�C�Y��ύX����֐�
bool changeConsoleSize(int x, int y) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SMALL_RECT rectConsoleSize = { 0, 0, 1, 1 };

	// Temporarily reduce console window size �R���\�[���E�B���h�E�T�C�Y�̈ꎞ�I�ȏk��
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);
	if (!SetConsoleScreenBufferSize(hStdout, coord)) {
		return FALSE;
	}
	rectConsoleSize.Right = x - 1;
	rectConsoleSize.Bottom = y - 1;
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);

	return TRUE;
}

// Function to hide the console title bar �R���\�[���̃^�C�g���o�[���B���֐�
void HideConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// Function to show the console title bar �R���\�[���̃^�C�g���o�[��\������֐�
void ShowConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// �L�[�{�[�h���̓o�b�t�@�̃N���A�֐�
void clearInputBuffer() {
	while (_kbhit()) {
		_getch();
	}
}

// *****************************************************************************
// VT�V�[�P���X���[�h��L����
void EnableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

//�R���\�[���E�B���h�E�̕\���F��RGB�`���ŕύX
void SetTextColorRGB(int r, int g, int b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

//�R���\�[���E�B���h�E�̔w�i�F��RGB�`���ŕύX
void SetBackgroundColorRGB(int r, int g, int b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
}