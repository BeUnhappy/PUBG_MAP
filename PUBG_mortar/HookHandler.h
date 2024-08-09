#pragma once

//struct KeyboardRegister {
//	int id;
//	std::function<void(int,LPARAM)> callback;
//	std::map<int, std::vector<int>>  keyBindings;
//};


class HookHandler
{
public:
	//Ψһ���� �����뷵��
	static HookHandler& GetHookHandler();
	static LRESULT CALLBACK ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

	bool installHook(HOOKPROC proc = ALLMessageProc);
	void UnistallHook() const;
	//ע���ݼ� �Լ������ݼ����� 
	std::vector<KeyboardRegister>::iterator RegisterKeyboard(std::function<void(int,LPARAM)>, std::map<int, std::vector<int>>);

	bool RemoveRegisterKeyboard(std::vector<KeyboardRegister>::iterator it);


private:
	HookHandler();
	HHOOK MouseHook=nullptr;
	HHOOK KeyboardHook=nullptr;

	//��ǰ�����б�
	static std::vector<KeyboardRegister> keyRegisters;

	
	//�Ƚϰ���ֵ
	static void compareKeyList(const std::vector<int>& b,LPARAM lparm);
	//ɾ����ǰ̧��� ����  
	static void removeKey(std::vector<int>& keyValue, int valueToRemove);
};
