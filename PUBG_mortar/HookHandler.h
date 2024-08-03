#pragma once
struct QuickKeyMod
{
	std::string function;
	std::vector<int> KeyValueList;
};

class HookHandler
{
public:
	//Ψһ���� �����뷵��
	static HookHandler& GetHookHandler();
	static LRESULT CALLBACK ALLMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

	bool installHook(HOOKPROC proc = ALLMessageProc);
	void UnistallHook();
	
	void updataKey(std::string&);

	//
	static void removeKeyValue(std::vector<int>& keyValue, int valueToRemove);
	static BOOL isKey(std::vector<int>&, std::vector<int>&);
	static std::vector<int> GetNowKey();
	//��ȡ��ݼ����
	static const std::map<std::string, std::vector<int>> GetQuickKey();
	static std::map<std::string, std::vector<int>> QuickKey;
	//���ÿ�ݼ����
	BOOL SetQuickKey(std::map<std::string, std::vector<int>>&);
	HookHandler(const HookHandler&) = delete;
	HookHandler& operator=(HookHandler&) = delete;

	//������ʱ����ڸ��»���ĺ�����������ѭ��������
	BOOL GetUpdateDraw();
	void SetUpdateDraw(BOOL);
private:
	HookHandler();
	HHOOK MouseHook=nullptr;
	HHOOK KeyboardHook=nullptr;
	static std::vector<POINT>* MainWindowPoint;

	
};
