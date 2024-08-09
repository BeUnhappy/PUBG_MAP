#include"pch.h"
#include"MainWindow.h"
#include"HookHandler.h"


struct Myinfo {
    std::map<std::string, std::vector<int>> key;
    int POINT_100M=80;
};



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
    {
        wprintf(L"����: MFC ��ʼ��ʧ��\n");
        return 1;
    }


    /*AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    std::cout << "Hello, console!\n " << std::endl;*/


    
    MainWindow& m = MainWindow::GteWindow();
    m.SetHinstance(hInstance);
    m.CreateWin();
    HWND hwnd = m.CreateWin();
    std::cout << "�������" << std::endl;
    if (hwnd == nullptr) {
        return 0;
    }



    // ʹ����͸��
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    // ���� DWM ͸����������
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);


    // ��ʾ����
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    HookHandler& h = HookHandler::GetHookHandler();
    h.installHook();
    
    m.AddIconTray();
    std::cout << "�������" << std::endl;
    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            break;
    }
    m.RemoveTray();

    h.UnistallHook();
    return 0;
}
