#include "pch.h"
#include "HookHandler.h"
#include "MainWindow.h"


//��ݼ� �����������˵��
//��깦��  
/*
 �����Ϣ���� int
    �������� 0x1000
    �������� 0x1001

 ������ 
*/


std::vector<KeyboardRegister> HookHandler::keyRegisters;

HookHandler::HookHandler() {

}

HookHandler& HookHandler::GetHookHandler()
{
    static HookHandler myHookHandler;
    return myHookHandler;
}

LRESULT HookHandler::ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    static std::vector<int>  pressedKeys;
    static LPARAM temp;
    if (nCode >= 0&&!keyRegisters.empty()) {
        switch (wParam)
        {
        case WM_KEYDOWN:
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            if (pressedKeys.empty())
            {
                pressedKeys.push_back(keycode->vkCode);
            }
            else if (pressedKeys.back() != keycode->vkCode)
            {
                pressedKeys.push_back(keycode->vkCode);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            pressedKeys.push_back(0x01);
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x01);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            pressedKeys.push_back(0x02);
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x02);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            MSLLHOOKSTRUCT* s=(MSLLHOOKSTRUCT*)lParam;
            int x = GET_WHEEL_DELTA_WPARAM(s->mouseData) / WHEEL_DELTA;
            if (x>0)
            {
                pressedKeys.push_back(0x1000);
            }
            else if (x < 0)
            {

                pressedKeys.push_back(0x1001);
                
            }
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, 0x1000);
            removeKey(pressedKeys, 0x1001);
            break;
        }
        case WM_KEYUP:
        {
            KBDLLHOOKSTRUCT* keycode = (KBDLLHOOKSTRUCT*)lParam;
            compareKeyList(pressedKeys, lParam);
            removeKey(pressedKeys, keycode->vkCode);
            break;
        }
        default:
            break;
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
    std::cout << "���Ӱ�װ�ɹ�" << std::endl;
    return true;
}

void HookHandler::UnistallHook() const
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



std::vector<KeyboardRegister>::iterator HookHandler::RegisterKeyboard(std::function<void(int,LPARAM)> callback, std::map<int, std::vector<int>> key) {
    static int id = 0;
    KeyboardRegister KeyRegister;
    KeyRegister.callback = callback;
    KeyRegister.keyBindings = key;
    KeyRegister.id = id++;

    return keyRegisters.insert(keyRegisters.end(), KeyRegister);
}

bool HookHandler::RemoveRegisterKeyboard(std::vector<KeyboardRegister>::iterator it)
{
    if (it == keyRegisters.end()) {
        // ��������Ч������ false
        return false;
    }

    keyRegisters.erase(it);
    return true;
}


void HookHandler::compareKeyList(const std::vector<int>& b,LPARAM lparm)
{
    if (!keyRegisters.empty() && !b.empty()) {
        for (auto& registerInfo : keyRegisters) {
            for (auto& binding : registerInfo.keyBindings) {
                if (binding.second.size() == b.size()) {
                    bool isMatch = true;
                    for (size_t i = 0; i < binding.second.size(); i++) {
                        if (binding.second[i] != b[i]) {
                            isMatch = false;
                            break; // �����ƥ�䣬�˳���ǰ�Ƚ�
                        }
                    }
                    if (isMatch) {
                        registerInfo.callback(binding.first, lparm);
                    }
                }
            }
        }
    }
}

void HookHandler::removeKey(std::vector<int>& keyValue, int valueToRemove)
{

    // ʹ�� std::remove ������ valueToRemove �Ƶ�����ĩβ����������ĩβ�ĵ�����
    auto newEnd = std::remove(keyValue.begin(), keyValue.end(), valueToRemove);

    // ʹ�� erase ɾ���� newEnd �� end ������Ԫ��
    keyValue.erase(newEnd, keyValue.end());
}

