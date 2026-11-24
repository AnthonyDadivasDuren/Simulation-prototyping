#include <iostream>
#include <Windows.h>
#include <vector>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int main()
{
    const wchar_t fogWindow[] = L"Fog Window";

    WNDCLASSW wc = {};

    wc.lpszClassName = fogWindow;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

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

        const int cellCount = gridWidth * gridHeight;

        int testRow = 3;
        int testColumn = 2;

        int testRow2 = 4;
        int testColumn2 = 1;

        std::vector<float> fog(cellCount, 1.0f);

        int testCell = testRow * gridWidth + testColumn;
        int testCell2 = testRow2 * gridWidth + testColumn2;
        fog[testCell] = 0.2f;
        fog[testCell2] = 0.5;

        PAINTSTRUCT paint{};

        HDC hdc = BeginPaint(hwnd, &paint);

        for (int row = 0; row < gridHeight; row++)
        {
            for (int column = 0; column < gridWidth; column++)
            {
                int top = row * cellSize;
                int left = column * cellSize;
                int right = cellSize + left;
                int bottom = cellSize + top;

                RECT square{ left, top, right, bottom };

                const int index = row * gridWidth + column;

                const float density = fog[index];

                int brushColor = static_cast<int>(density * 190.0f);


                HBRUSH hbrush = CreateSolidBrush(RGB(brushColor, brushColor, brushColor));

                FillRect(hdc, &square, hbrush);

                DeleteObject(hbrush);
            }
        }
        EndPaint(hwnd, &paint);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
    
    

