#include "pch.h"
#include "MainWindow.h"
#include "Setting.h"

std::vector<POINT> MainWindow::pointList;
RECT MainWindow::BackGround;
int MainWindow::POINT_100M=80;
int MainWindow::distanceBetweenPoints;
int MainWindow::distanceBetweenPoints_Game;
NOTIFYICONDATA MainWindow::Tray;
bool MainWindow::DrawPoint = false;
RECT MainWindow::WindowSize;

MainWindow& MainWindow::GteCreateWindow(HINSTANCE hinstance)
{
    static MainWindow my(hinstance);
    POINT p1{10,10}, p2{20,20};
    pointList.push_back(p1);
    pointList.push_back(p2);
    
    return my;
}



HWND MainWindow::GethWnd() const
{
    return hWnd;
}

//���ڹ����봴��
MainWindow::MainWindow(HINSTANCE hinstance)
{

    //����Ĭ�ϵĴ�����Ϊ
    wc = {};
    wc.lpfnWndProc = this->WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = TEXT("My_window");
    //ע�ᴰ��
    RegisterClass(&wc);
    //����ȫ��Histance
    this->hinstance = hinstance;
    //��������
    hWnd = CreateMyWindow();
    if (hWnd==nullptr)
    {
        exit(0);
    }
    //
    BackGround = { 10, 10, 400, 70 };
}

//���ڹ��̴�����


HWND MainWindow::CreateMyWindow() const
{
    WindowSize = { 0,0 };
    HWND desktop = GetDesktopWindow();
    GetClientRect(desktop, &WindowSize);

    return CreateWindowEx(
        WS_EX_TRANSPARENT | WS_EX_LAYERED,  // ��չ������ʽ��ʹ�� WS_EX_LAYERED �� WS_EX_TRANSPARENT
        wc.lpszClassName,                   // ��������
        TEXT("Transparent Window"),         // ���ڱ���
        WS_POPUP,                           // ������ʽ
        0, 0,                               // Ĭ��λ��
        WindowSize.right, WindowSize.bottom,                         // ���ڿ�Ⱥ͸߶�
        nullptr,                            // ������
        nullptr,                            // �˵�
        hinstance,                          // Ӧ�ó���ʵ�����
        nullptr                             // ���Ӳ���
    );

    
}

std::vector<POINT>& MainWindow::GetPoint()
{
    return pointList;
}

void MainWindow::updataDraw() 
{
    InvalidateRect(hWnd, NULL, true);
    UpdateWindow(hWnd);
}

void MainWindow::AddIconTray()
{
    ZeroMemory(&Tray, sizeof(Tray));

    Tray.cbSize = sizeof(NOTIFYICONDATA);
    Tray.hWnd = GethWnd();
    Tray.uID = 1; // ͼ��� ID�������Զ���
    Tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Tray.uCallbackMessage = WM_USER + 1; // �Զ�����Ϣ
    Tray.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // IDI_TRAY_ICON ��������Դ����ӵ�ͼ�� ID
    lstrcpy(Tray.szTip, _T("����ͼ����ʾ�ı�"));

    Shell_NotifyIcon(NIM_ADD,&Tray);
}

void MainWindow::RemoveTray()
{
    Shell_NotifyIcon(NIM_DELETE, &Tray);
}

LRESULT MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_USER + 1:
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
            HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
            if (hMenu == NULL) {
                MessageBox(hwnd, TEXT("Failed to load menu resource"), TEXT("Error"), MB_OK);
                
                return 0;
            }

            HMENU hSubMenu = GetSubMenu(hMenu, 0);
            if (hSubMenu == NULL) {
                MessageBox(hwnd, TEXT("Failed to get submenu"), TEXT("Error"), MB_OK);
                DestroyMenu(hMenu);
                return 0;
            }


            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);

            TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);

            DestroyMenu(hMenu);
            break;
        }
    }
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case ID_SETTING:
        {
            Setting s;
            s.DoModal();
        } 
            break;
        case ID_EXIT:
        {
            PostQuitMessage(0);
        }
            break;
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // ����һ�����ݵ��ڴ�DC
        HDC memDC = CreateCompatibleDC(hdc);
        //����DC��С��ͬ��λͼ
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        //��λͼѡ��DC
        SelectObject(memDC, memBitmap);

        RECT rect = { 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top };
        FillRect(memDC, &rect, (HBRUSH)GetStockObject(NULL_BRUSH)); //��͸��������ȫ�����DC
        //��ͼ����
        Draw(memDC);


        //��ͼ��������
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
       
    }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

const void MainWindow::Draw(HDC & hdc)
{
    if (DrawPoint)
    {
        HBRUSH brushGreen = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdc, brushGreen);
        Ellipse(hdc, pointList[0].x - 5, pointList[0].y - 5, pointList[0].x + 5, pointList[0].y + 5);
        // ������ɫ��ˢ�����Ƶڶ�����
        HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, brushRed);
        Ellipse(hdc, pointList[1].x - 5, pointList[1].y - 5, pointList[1].x + 5, pointList[1].y + 5);
        // ɾ����ˢ����
        DeleteObject(brushGreen);
        DeleteObject(brushRed);
    }
    HBRUSH brushRed = CreateSolidBrush(RGB(255, 0, 255));
    FillRect(hdc,&BackGround , brushRed);

    std::wstring str1 = L"����״̬�� ";
    if (DrawPoint)
    {
        str1 += L"����";
    }
    else {
        str1 += L"�ر�";
    }
    std::wstring str2 = L"��ͼ100������������ ";
    std::wstring str3 = L"2��֮������������ ";
    std::wstring str4 = L"��Ϸ�ھ��룺 ";
    str2 += std::to_wstring(POINT_100M);
    str3 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2))));
    str4 += std::to_wstring((int)(std::sqrt(std::pow(pointList[0].x - pointList[1].x, 2) + std::pow(pointList[0].y - pointList[1].y, 2)) * ((double)100 / POINT_100M)));
    TextOut(hdc, BackGround.left, BackGround.top, str1.c_str(), str1.length());
    TextOut(hdc, BackGround.left, BackGround.top+15, str2.c_str(), str2.length());
    TextOut(hdc, BackGround.left, BackGround.top+30, str3.c_str(), str3.length());
    TextOut(hdc, BackGround.left, BackGround.top+45, str4.c_str(), str4.length());
    
    DeleteObject(brushRed);
}