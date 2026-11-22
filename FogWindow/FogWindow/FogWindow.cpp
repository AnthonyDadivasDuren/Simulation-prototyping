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

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

