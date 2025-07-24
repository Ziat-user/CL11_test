/*******************************************************************************
*
*	タイトル：	バトルゲーム　エンカウンター　メイン処理	[ main.cpp ]
*	Copyright (c) 2025 HAL TOKTO, ALL RIGHTS RESERVED.
*
*	作成者：							ＨＡＬ東京　教務部　土居秀顕
*	作成日：		2025/06/15  ver 1.0 マップ表示
*   修正日：		2025/06/15	ver 1.1 敵ゲット、死に戻りを追加
*   修正日：		2025/06/15	ver 1.2 マップにワームホールを追加
*   修正日：		2025/06/22	ver 1.21 ディスプレイの初期化関数追加、マップ更新
* 
*******************************************************************************/

// *****************************************************************************
// Preprocessor プリプロセッサ 必要なヘッダファイルをインクルード
// *****************************************************************************
#include <iostream>
#include <conio.h>
#include <windows.h>
//#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*******************************************************************************
マクロ定義
*******************************************************************************/
#define V_MAX			(15)		/* 縦幅 */
#define H_MAX			(40)		/* 横幅 */
#define	ENEMY_MAX		(1)			/* 敵の最大数 */
#define MAX_enemy_HP	(50)		/* 敵の最大体力 */
#define MAX_player_HP	(50)		/* プレイヤーの最大体力 */
#define WARP_IN_X		(-1)		/* プレイヤー用ワープインX座標 */ 
#define WARP_IN_Y		(7)			/* プレイヤー用ワープインY座標 */ 
#define WARP_OUT_X		(40)		/* プレイヤー用ワープアウトX座標 */ 
#define WARP_OUT_Y		(7)			/* プレイヤー用ワープアウトY座標 */ 
#define KEY_FUNC		(0xe0)
#define KEY_UP			(0x48)
#define KEY_DOWN		(0x50)
#define KEY_RIGHT		(0x4d)
#define KEY_LEFT		(0x4b)

// *****************************************************************************
// それぞれの副関数の中で使用する定数や変数だからmain関数や副関数の外に置いてある
// *****************************************************************************
// Define constants 定数の定義
const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 25;

// Define variables
bool rtn = 0;
int ch = 0;

// *****************************************************************************
// Function prototype declarations 関数のプロトタイプ宣言
// *****************************************************************************
void SetCursorVisibility(BOOL visible);		// カーソルの表示/非表示を設定する関数
void MoveCursorToTop();		                // カーソルを画面の左上に移動する関数
void clearInputBuffer();                    // キーボード入力バッファのクリア関数
bool changeConsoleSize(int x, int y);       // コンソールサイズを変更する関数
bool CL11Startup();                         // コンソールの初期化関数
bool CL11Cleanup();                         // コンソールのクリーンアップ関数
void HideConsoleTitleBar();                 // コンソールのタイトルバーを隠す関数
void ShowConsoleTitleBar();                 // コンソールのタイトルバーを表示する関数
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY); // コンソールフォントの設定関数
void centerConsoleWindow(int screenWidth, int screenHeight);    // コンソールウィンドウを中央に配置する関数
void EnableVTMode();						// VTシーケンスモードを有効化
void SetTextColorRGB(int r, int g, int b);	//コンソールウィンドウの表示色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b);	//コンソールウィンドウの背景色をRGB形式で変更

/*******************************************************************************
メイン関数
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

	// カーソル非表示
	SetCursorVisibility(FALSE);

	/* 変数の宣言 */
	char map[V_MAX][H_MAX + 1] = 			/* 横を+1しているのは文末のNULLの分 */
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

	/* ランダム用初期設定 */
	srand((unsigned)time(NULL));

	/* プレイヤーの初期設定 */
	player_HP = MAX_player_HP - (rand() % 5 + 1); /* プレイヤーの体力は45～50の間 */
	player_x = 1;
	player_y = 1;
	map[player_y][player_x] = 'P';

	/* 敵の初期設定 */
	for (i = 0; i < ENEMY_MAX; i++)
	{
		enemy_x[i] = rand() % (H_MAX - 8) + 1;
		enemy_y[i] = rand() % (V_MAX - 6) + 5;
		map[enemy_y[i]][enemy_x[i]] = 'E';
		enemy_HP[i] = MAX_enemy_HP - (rand() % 5 + 1); /* 敵の体力は45～50の間 */
		enemy_damage[i] = 0;
	}

	std::cout << "*****************************************************" << std::endl;
	std::cout << " ＲＥ：エネミーＧＯ！ ";
	std::cout << " -- 全部の敵をゲットだぜっ！ --" << std::endl;
	std::cout << std::endl;
	std::cout << " Copyright(C) 2025 HAL TOKYO, All rights reserved." << std::endl;
	std::cout << std::endl;
	std::cout << "*****************************************************" << std::endl;
	std::cout << std::endl;
	std::cout << " E:敵  P:プレイヤー ← → ↑↓で移動" << std::endl;
	std::cout << std::endl;
	std::cout << " エンターキーでスタート" << std::endl;
	EnableVTMode();
	// Set text color to white and background color to black コンソールのテキスト色を緑設定
	SetTextColorRGB(0, 255, 0);

	while (1)
	{
		/* キー入力判定 */
		if (_kbhit())
		{
			// キー入力を取得（2バイトコード対応：特殊キーの場合は2回呼び出し）
			key = _getch();
			if ((key == 0) || (key == KEY_FUNC))
			{
				key = _getch();
			}

			/* 前回の表示位置の消去*/
			old_x = player_x;
			old_y = player_y;
			map[player_y][player_x] = ' ';
			for (i = 0; i < ENEMY_MAX; i++)
			{
				map[enemy_y[i]][enemy_x[i]] = ' ';
			}

			/* プレイヤーの位置判定 */
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

			// 移動した先はNGな所？
			if (map[player_y][player_x] == '*')
			{	/* NGなら移動前の場所に戻す */
				player_x = old_x;
				player_y = old_y;
			}

			/* プレイヤーの位置更新 */
			map[player_y][player_x] = 'P';

			//*************************************************************************************
			/* 敵の移動位置更新 */
			for (i = 0; i < ENEMY_MAX; i++)
			{
				old_x = enemy_x[i];
				old_y = enemy_y[i];
				enemy_move = rand() % 5;	/* 移動無しも含めるので５ */
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

				// 移動した先はNGな所？
				if (map[enemy_y[i]][enemy_x[i]] == '*')
				{	/* NGなら移動前の場所に戻す */
					enemy_x[i] = old_x;
					enemy_y[i] = old_y;
				}
				map[enemy_y[i]][enemy_x[i]] = 'E';
			}
			/* 画面クリア */
			std::cout << "\033[2J\033[H";

			// デバッグ用　プレイヤーと敵の座標表示
			 printf( " Player x:%2d y:%2d\n" , player_x, player_y );
			 for( i = 0 ; i < ENEMY_MAX ; i++ )
			 {
			 	 printf( " Enemy%d x:%2d y:%2d\n" , i , enemy_x[ i ] , enemy_y[ i ] );
			 }

			/* マップ描画処理 */
			for (i = 0; i < V_MAX; i++)
			{
				std::cout << map[i] << std::endl;
			}
		}
		for (i = 0; i < ENEMY_MAX; i++)
		{
			if (player_x == enemy_x[i] && player_y == enemy_y[i])
			{
				std::cout << " 敵" << i << " 発見!!";
				std::cout << " プレイヤーＨＰ:" << player_HP << "  敵" << i << "のＨＰ:" << enemy_HP[i] << std::endl;

				while ((player_HP >= 0) && (enemy_HP[i] >= 0))
				{
					printf(" プレイヤーのターン 敵%d を攻撃", i);
					enemy_damage[i] = rand() % 10 + 11;
					enemy_HP[i] = enemy_HP[i] - enemy_damage[i];

					if (enemy_HP[i] <= 0)
					{
						printf(" 敵%d は %d のダメージを受けた\n", i, enemy_damage[i]);
						printf(" 敵%d を倒した ゲットだぜっ!\n", i);
						enemy_get_count = enemy_get_count + 1;			/* 倒した敵の数をカウントアップ */
						map[enemy_y[i]][enemy_x[i]] = ' ';		/*倒した敵の現在位置のEを消す */
						enemy_x[i] = 36;								/*ゲットした敵をケージに保管 */
						enemy_y[i] = 2;
						map[enemy_y[i]][enemy_x[i]] = 'E';		/*ケージの位置情報にEを書き込む */

						printf(" プレイヤーは謎のポーションを飲んだ ");
						player_potion = rand() % 30 + 1;				/* 謎の薬の効能は1～30　*/
						player_HP = player_HP + player_potion;
						printf(" ＨＰが %d 戻った", player_potion);
						printf(" ＨＰ:%d\n", player_HP);
						break;
					}
					else
					{
						printf(" 敵%d は %d のダメージを受けた 残りＨＰ:%d\n", i, enemy_damage[i], enemy_HP[i]);
						_getch();
					}

					printf(" 敵%d のターン プレイヤーを攻撃", i);
					player_damage = rand() % 10 + 11;
					player_HP = player_HP - player_damage;

					if (player_HP <= 0)
					{
						printf(" プレイヤーは %d のダメージを受けた\n", player_damage);
						printf(" プレイヤーは力尽きた…\n");
						printf(" 死に戻りだ……\n");

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
						printf(" プレイヤーは %d のダメージを受けた 残りＨＰ:%d\n", player_damage, player_HP);
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

			// 画面クリア
			//system("cls");	
			std::cout << "\033[2J\033[H";

			std::cout << "*****************************************************" << std::endl;
			std::cout << std::endl;
			std::cout << " おめでとう！ミッションクリア！" << std::endl;
			std::cout << std::endl;
			std::cout << " G A M E  O V E R" << std::endl;
			std::cout << std::endl;
			std::cout << "*****************************************************" << std::endl;
			std::cout << " エンターキーで終了" << std::endl;
			break;
		}
		/* 処理速度調整 */
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
// 画面制御用の副関数
// *****************************************************************************

// カーソル制御関数( TRUE:表示 FALSE:非表示)
void SetCursorVisibility(BOOL visible) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// カーソルを先頭に戻す関数
void MoveCursorToTop() {
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to initialize the console コンソールの初期化関数
bool CL11Startup() {
	// Change console size コンソールサイズの変更
	if (!changeConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT)) {
		return FALSE;
	}
	// Hide the console title bar コンソールのタイトルバーを隠す
	HideConsoleTitleBar();

	// Get screen width and height 画面の幅と高さを取得
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(0, screen);

	// Set console font コンソールフォントの設定
	setConsoleFont(CONSOLE_WIDTH, CONSOLE_HEIGHT, screenWidth, screenHeight, dpiX, dpiY);
	// Center the console window コンソールウィンドウを中央に配置
	centerConsoleWindow(screenWidth, screenHeight);
	return TRUE;
}

// Function to set the console font コンソールフォントの設定関数
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY) {
	int fontSizeX = (screenWidth / width) * (dpiX / 96);
	int fontSizeY = (screenHeight / height) * (dpiY / 96);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 0}, FF_DONTCARE, FW_NORMAL, L"MS Gothic" };
	DISPLAY_DEVICE dd = { sizeof(dd) };
	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm = { sizeof(dm) };
	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	// Set font size フォントサイズの設定
	if (dm.dmPelsHeight > dm.dmPelsWidth) {
		cfi.dwFontSize.X = static_cast<SHORT>(fontSizeX);
		cfi.dwFontSize.Y = static_cast<SHORT>(floor(fontSizeX * 2));
	}
	else {
		cfi.dwFontSize.X = static_cast<SHORT>(floor(fontSizeY / 2));
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSizeY);
	}

	// Apply console font コンソールフォントの適用
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width) - 1, static_cast<SHORT>(height) - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

// Function to center the console window コンソールウィンドウを中央に配置する関数
void centerConsoleWindow(int screenWidth, int screenHeight) {
	HWND consoleWindow = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleWindow, &rect);
	MoveWindow(consoleWindow, (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// Function to clean up the console コンソールのクリーンアップ関数
bool CL11Cleanup() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Set console mode コンソールモードの設定
	SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);

	// Set console buffer size コンソールバッファサイズの設定
	COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	SetConsoleScreenBufferSize(hOut, bufferSize);
	CONSOLE_CURSOR_INFO cursorInfo = { 25, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);
	SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 16}, FF_DONTCARE, FW_NORMAL, L"Consolas" };
	SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	// Reset console window position コンソールウィンドウの位置をリセット
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowConsoleTitleBar();

	return TRUE;
}

// Function to change the console size コンソールサイズを変更する関数
bool changeConsoleSize(int x, int y) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SMALL_RECT rectConsoleSize = { 0, 0, 1, 1 };

	// Temporarily reduce console window size コンソールウィンドウサイズの一時的な縮小
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);
	if (!SetConsoleScreenBufferSize(hStdout, coord)) {
		return FALSE;
	}
	rectConsoleSize.Right = x - 1;
	rectConsoleSize.Bottom = y - 1;
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);

	return TRUE;
}

// Function to hide the console title bar コンソールのタイトルバーを隠す関数
void HideConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// Function to show the console title bar コンソールのタイトルバーを表示する関数
void ShowConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// キーボード入力バッファのクリア関数
void clearInputBuffer() {
	while (_kbhit()) {
		_getch();
	}
}

// *****************************************************************************
// VTシーケンスモードを有効化
void EnableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

//コンソールウィンドウの表示色をRGB形式で変更
void SetTextColorRGB(int r, int g, int b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

//コンソールウィンドウの背景色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
}