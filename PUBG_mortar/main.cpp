#include "pch.h"
#include"MainWindow.h"
#include"HookHandler.h"


struct Myinfo {
    std::map<std::string, std::vector<int>> key;
    int POINT_100M=80;
};

std::string GetExecutableDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
std::string GetConfigFilePath() {
    std::string exeDir = GetExecutableDirectory();
    return exeDir + "\\config.dat"; // ʹ�����·��
}

bool LoadConfigurationBinary(const std::string& filename, Myinfo& info) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file for reading: " << filename << std::endl;
        return false;
    }

    try {
        // Read POINT_100M
        file.read(reinterpret_cast<char*>(&info.POINT_100M), sizeof(info.POINT_100M));

        // Read key size
        uint32_t keySize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

        for (uint32_t i = 0; i < keySize; ++i) {
            // Read key length
            uint32_t keyLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));

            // Read key
            std::string key(keyLength, '\0');
            file.read(&key[0], keyLength);

            // Read vector size
            uint32_t vecSize;
            file.read(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));

            // Read vector elements
            std::vector<int> values(vecSize);
            if (vecSize > 0) {
                file.read(reinterpret_cast<char*>(values.data()), vecSize * sizeof(int));
            }

            info.key[key] = values;
        }

        file.close(); // Ensure the file is properly closed
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred while reading the binary file: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void SaveConfigurationBinary(const std::string& filename, const Myinfo& info) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file for writing: " << filename << std::endl;
        MessageBox(NULL, L"Failed to open configuration file for writing:", L"", MB_OK);
        return;
    }

    try {
        // Write POINT_100M
        file.write(reinterpret_cast<const char*>(&info.POINT_100M), sizeof(info.POINT_100M));
        
        // Write key size
        uint32_t keySize = info.key.size();
        file.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));

        for (const auto& el : info.key) {
            // Write key length
            uint32_t keyLength = el.first.size();
            file.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));

            // Write key
            file.write(el.first.data(), keyLength);

            // Write vector size
            uint32_t vecSize = el.second.size();
            file.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

            // Write vector elements
            if (!el.second.empty()) {
                file.write(reinterpret_cast<const char*>(el.second.data()), vecSize * sizeof(int));
            }
        }

        file.flush();
        file.close(); // Ensure the file is properly closed
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred while writing the binary file: " << e.what() << std::endl;
        MessageBox(NULL, L"Exception occurred while writing the binary file:", L"", MB_OK);
    }
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    //��Ӧ��DPI ��Ե���Ⱦ
    SetProcessDPIAware();


    /*AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    std::cout << "Hello, console!\n " << std::endl;*/

    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
    {
        wprintf(L"����: MFC ��ʼ��ʧ��\n");
        return 1;
    }
    



    MainWindow& m= MainWindow::GteCreateWindow(hInstance);

    HWND hwnd = m.GethWnd();

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


    Myinfo info;
    std::string configFilePath = GetConfigFilePath();

    // ���Դ������ļ���������
    if (!LoadConfigurationBinary(configFilePath, info)) {
        info.key["��Ⱦ��ǵ�"] = { 0xA2 , 0x4D };
        info.key["��ǵ�1"] = { 0x31 };
        info.key["��ǵ�2"] = { 0x32 };
        info.key["���ӻ�׼ֵ"] = { 0xA2 ,0xBB };
        info.key["���ٻ�׼ֵ"] = { 0xA2 ,0xBD };
        info.key["������ĵ�1"] = {};
        info.key["������ĵ�2"] = {};
        info.POINT_100M = 80; // Ĭ��ֵ

        SaveConfigurationBinary(configFilePath, info);
    }

    HookHandler& h = HookHandler::GetHookHandler();
    h.SetQuickKey(info.key);
    h.installHook();

    m.AddIconTray();

    MSG msg = {};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (h.GetUpdateDraw()) {
            m.updataDraw();
            h.SetUpdateDraw(false);
            
        }
        if (msg.message == WM_QUIT)
            break;
    }
    info.key = h.GetQuickKey();
    info.POINT_100M = m.POINT_100M;
    SaveConfigurationBinary(configFilePath, info);


    h.UnistallHook();

    // ɾ������ͼ��
    m.RemoveTray();
    
    return 0;
}
