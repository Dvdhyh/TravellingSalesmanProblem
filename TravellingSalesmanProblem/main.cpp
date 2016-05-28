#include "AllHeaderFiles.h"

//Just timing how long it takes per cycles
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#define TIMERID 9003

void SetupInitialPop(TheEngine& _Engine) {
	_Engine._GA.Clear();

	for (int a = 0; a < _Engine._GA.iPoints; a++)
		_Engine._GA.vec_temp.emplace_back(a);

	_Engine._GA.RandomPointSet();
	_Engine.bSSButtonFirstClicked = false;
	_Engine.bResume = false;
	_Engine.iAppState = Pause;

	if (_Engine._GA.iPopulation <= 0) {
		_Engine.iAppState = NotReady;
		::MessageBox(NULL, L"Population must be above 1", L"Invalid Value Error", NULL);
	}
	if (_Engine._GA.iMutateRate <= -1) {
		_Engine.iAppState = NotReady;
		::MessageBox(NULL, L"Mutation rate must be 0 or above", L"Invalid Value Error", NULL);
	}
	if (_Engine._GA.iPoints <= 2) {
		_Engine.iAppState = NotReady;
		::MessageBox(NULL, L"Number of Points must be 3 or above", L"Invalid Value Error", NULL);
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT iCmdShow)
{
	WNDCLASS wcex;
	//wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wcex);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HWND hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		0,            // initial y position
		CW_USEDEFAULT,            // initial x size
		0,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(9001));

	hInstance = hInstance;

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TheEngine _Engine;
	static GdiplusStartupInput gdiplusStartupInput;
	static ULONG_PTR           gdiplusToken;
	static RECT clientRect;

	switch (message) {
	case WM_CREATE: {
		MoveWindow(hWnd, 0, 0, 900, 800, false);

		GetClientRect(hWnd, &clientRect);

		_Engine.iAppState = NotReady;

		SetTimer(hWnd, TIMERID, NULL, NULL);

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		break;
	}
	case WM_SIZE:
		GetClientRect(hWnd, &clientRect);
		break;
	case WM_TIMER: {
		switch (wParam) {
		case TIMERID:
			//Perform calculations
			if (_Engine.iAppState == Active) {
				auto t1 = Clock::now();

				_Engine._GA.GetPopFitness();
				//Takes 10ms

				auto t2 = Clock::now();

				_Engine._GA.CreateGenePool();
				// 0 ms

				auto t3 = Clock::now();

				_Engine._GA.CreateOffspring();
				// 1 ms

				auto t4 = Clock::now();

				_Engine._GA.iGeneration++;

				/*WCHAR qwee[20] = { 0 };
				wsprintf(qwee, L"%i %i %i", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count(), 
				std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count(), 
				std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count());
				MessageBox(NULL, qwee, L"Population - Genepool - Offspring", NULL);*/

				InvalidateRect(hWnd, NULL, false);
			}
			
			break;
		default:
			break;
		}

		break;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case 'R':{
			//Clearing all data and setting states to default
			SetupInitialPop(_Engine);
			_Engine.bRandomButtonClicked = true;
			break;
		}
		case 'T':{
			_Engine._GA.iPopulation--;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case 'Y':{
			_Engine._GA.iPopulation++;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case 'G':{
			_Engine._GA.iMutateRate--;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case 'H':{
			_Engine._GA.iMutateRate++;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case 'B':{
			_Engine._GA.iPoints--;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case 'N':{
			_Engine._GA.iPoints++;
			SetupInitialPop(_Engine);
			_Engine.iAppState = NotReady;
			break;
		}
		case VK_RETURN:{
			// The Start/Stop button was clicked
			if (_Engine.iAppState == NotReady)
				break;

			if (_Engine.bResume == false) {
				_Engine._GA.CreatePopulation();
				_Engine.bResume = true;
			}


			if (_Engine.iAppState == Active) {
				_Engine.iAppState = Pause;
			}
			else {
				_Engine.bSSButtonFirstClicked = true;

				_Engine.iAppState = Active;
			}
			break;
		}
		}

		InvalidateRect(hWnd, NULL, true);

		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		_Engine.DisplayOnScreen(hdc, clientRect);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		GdiplusShutdown(gdiplusToken);
		KillTimer(hWnd, 9003);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}