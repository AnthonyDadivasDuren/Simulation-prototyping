#include <iostream>
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int main()
{
    const wchar_t fogWindow[] = L"Fog Window";

    WNDCLASSW wc = {};

    wc.lpszClassName = fogWindow;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);

    const ATOM registrationResult = RegisterClassW(&wc);

    if (registrationResult == 0)
    {
        std::cout << "Registration failed, error: " << GetLastError() << "\n";
        return 1;
    }

    HWND hwnd = CreateWindowExW(0, wc.lpszClassName, L"Fog Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, 640, 480, nullptr, nullptr, wc.hInstance, nullptr);

    if (hwnd == nullptr)
    {
        std::cout << "Window creation failed, error: " << GetLastError() << "\n";
        return 1;
    }


    ShowWindow(hwnd, SW_SHOW);

    MSG message{};
    int result;

    while ((result = GetMessageW(&message, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);

    }

    std::cout << "Message Loop ended.\n";
    
   
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    if (uMsg == WM_PAINT)
    {
        
        const int cellSize = 32;

        const int gridHeight = 8;
        const int gridWidth = 8;

        PAINTSTRUCT paint{};

        HDC hdc = BeginPaint(hwnd, &paint);

        HBRUSH hBrushLightGrey = CreateSolidBrush(RGB(190, 190, 190));
        HBRUSH hBrushDarkGrey = CreateSolidBrush(RGB(90, 90, 90));

        for (int row = 0; row < gridHeight; row++)
        {
            for (int column = 0; column < gridWidth; column++)
            {
                int top = row * cellSize;
                int left = column * cellSize;
                int right = cellSize + left;
                int bottom = cellSize + top;

                RECT square{ left, top, right, bottom };

                

                if ((row + column) % 2 == 0)
                {
                    FillRect(hdc, &square, hBrushLightGrey);
                }
                else
                {
                    FillRect(hdc, &square, hBrushDarkGrey);
                }

                
            }
        }

        DeleteObject(hBrushLightGrey);
        DeleteObject(hBrushDarkGrey);

        EndPaint(hwnd, &paint);
        return 0;
    }

    

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

