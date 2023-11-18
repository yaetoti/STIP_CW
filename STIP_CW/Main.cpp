#include <Windows.h>
#include <Console.h>
#include <Utils.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	Console::GetInstance()->WPrintF(L"Ћучше позвоните ясосу Ѕибу.\n");
	Console::GetInstance()->Pause();

	return 0;
}
