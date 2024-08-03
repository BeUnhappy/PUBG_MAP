#include "pch.h"
#include "HookHandler.h"
#include "MainWindow.h"

//��ݼ� �����������˵��
//��깦��  
std::vector<POINT>* HookHandler::MainWindowPoint = &MainWindow::GetPoint();
std::map<std::string, std::vector<int>> HookHandler::QuickKey;
static BOOL upMainWindow;
static std::vector<int>  KeyValue;

BOOL HookHandler::GetUpdateDraw()
{
    return upMainWindow;
}

void HookHandler::SetUpdateDraw(BOOL flag)
{
    upMainWindow = flag;
}

HookHandler::HookHandler()
{
    upMainWindow = false;
}


HookHandler& HookHandler::GetHookHandler()
{
    static HookHandler myHookHandler;
    return myHookHandler;
}

LRESULT HookHandler::ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    
    if (nCode >= 0) {
        if (wParam==WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (KeyValue.empty())
            {
                KeyValue.push_back(keycode->vkCode);
            }
            else if (KeyValue.back()!= keycode->vkCode)
            {
                KeyValue.push_back(keycode->vkCode);
            }            
        }
        else if (wParam == WM_LBUTTONDOWN )
        {
            MSLLHOOKSTRUCT* mouseCode = (MSLLHOOKSTRUCT*)lParam;
            if ((!KeyValue.empty())&& !QuickKey.empty()&&!MainWindowPoint->empty())
            {
                if (KeyValue == QuickKey["��ǵ�1"])
                {
                    MainWindowPoint->at(0) = mouseCode->pt;
                }
                else if (KeyValue == QuickKey["��ǵ�2"]) 
                {
                    MainWindowPoint->at(1) = mouseCode->pt;
                }
                upMainWindow = true;
            }
            
        }
        //����̧���� ��ϼ�����
        else if (wParam== WM_KEYUP)
        {
            
            


            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (isKey(QuickKey["��Ⱦ��ǵ�"], KeyValue))
            {
                MainWindow::DrawPoint = !MainWindow::DrawPoint;
            }
            else if (isKey(KeyValue,QuickKey["������ĵ�1"]))
            {

                std::cout << "������ĵ�1"<<std::endl;
                std::cout << "��" << MainWindow::WindowSize.right;
                std::cout << "��" << MainWindow::WindowSize.bottom;
                MainWindowPoint->at(0).x = MainWindow::WindowSize.right / 2;
                MainWindowPoint->at(0).y = MainWindow::WindowSize.bottom / 2;
            }
            else if (isKey(KeyValue, QuickKey["������ĵ�2"]))
            {
                MainWindowPoint->at(1).x = MainWindow::WindowSize.right / 2;
                MainWindowPoint->at(1).y = MainWindow::WindowSize.bottom / 2;
            }
            else if (isKey(KeyValue , QuickKey["���ӻ�׼ֵ"]))
            {
                MainWindow::POINT_100M += 5;
            }
            else if (KeyValue == QuickKey["���ٻ�׼ֵ"])
            {
                MainWindow::POINT_100M -= 5;
            }

            //ɾ��̧���ֵ
            removeKeyValue(KeyValue, keycode->vkCode);

            //���û��濪ʼ����
            upMainWindow = true;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool HookHandler::installHook(HOOKPROC proc)
{
    //��װ���� ��ʧ�ܾ��˳�
    MouseHook = SetWindowsHookEx(WH_MOUSE_LL, proc, NULL, 0);
    if (MouseHook==nullptr)
    {
        std::cout << "���Ӱ�װʧ��" << std::endl;

        return false;
    }
    KeyboardHook=SetWindowsHookEx(WH_KEYBOARD_LL, proc, NULL, 0);
    if (KeyboardHook==nullptr)
    {
        std::cout << "���Ӱ�װʧ��" << std::endl;
        UnhookWindowsHookEx(MouseHook);
        return false;
    }
    return true;
}

void HookHandler::UnistallHook()
{
    if (MouseHook!=nullptr)
    {
        UnhookWindowsHookEx(MouseHook);
    }
    if (KeyboardHook!=nullptr)
    {
        UnhookWindowsHookEx(KeyboardHook);
    }
}


void HookHandler::removeKeyValue(std::vector<int>& keyValue, int valueToRemove)
{

    // ʹ�� std::remove ������ valueToRemove �Ƶ�����ĩβ����������ĩβ�ĵ�����
    auto newEnd = std::remove(keyValue.begin(), keyValue.end(), valueToRemove);

    // ʹ�� erase ɾ���� newEnd �� end ������Ԫ��
    keyValue.erase(newEnd, keyValue.end());
}

BOOL HookHandler::isKey(std::vector<int>& a, std::vector<int>& b)
{
    if (!a.empty() && (a.size()==b.size()))
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a.at(i) != b.at(i))
            {
                return false;
            }
            
        }
        return true;
    }

    return false;
}

std::vector<int> HookHandler::GetNowKey()
{
    return KeyValue;
}

const std::map<std::string, std::vector<int>> HookHandler::GetQuickKey()
{
    return QuickKey;
}

BOOL HookHandler::SetQuickKey(std::map<std::string, std::vector<int>>& tempQuickKey)
{
    std::cout << "���ÿ�ݼ�" << std::endl;
    if (!tempQuickKey.empty())
    {
        QuickKey = tempQuickKey;
        return true;
    }
    return false;
}

