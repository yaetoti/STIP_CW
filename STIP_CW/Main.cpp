#include <Windows.h>
#include <Console.h>
#include <Utils.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	Console::GetInstance()->WPrintF(L"����� ��������� ����� ����.\n");
	Console::GetInstance()->Pause();

	return 0;
}
