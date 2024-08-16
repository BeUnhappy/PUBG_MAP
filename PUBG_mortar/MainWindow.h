#pragma once
#include "Setting.h"



class MainWindow
{
public:
	static MainWindow& GteWindow();
	static void SetHinstance(HINSTANCE hinstanceF);
	HWND CreateWin() const;

	//��ֹʹ�ø��ƹ��캯������
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	void updataDraw();
	void SetKeyValue(std::map<int, std::vector<int>>);

	//���� ���ڻ��������ı��� ,Ŀǰ�Ͻ�����������
	static std::vector<POINT> pointList;
	static MainWindowInfo info;
	static int Map_Size;
	//������� 
	void AddIconTray();
	//ɾ������
	void RemoveTray();
	static RECT WindowSize;
	static bool DrawPoint;
	static bool Map_open;
	


	//���������ļ���
	bool LoadConfigurationBinary(const std::string& filename);
	static void SaveConfigurationBinary(const std::string& filename);
private:
	//����
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//���� �Զ�����Ϣ
	static void CALLBACK messageProc(int message,LPARAM lpramr);

	static std::vector<KeyboardRegister>::iterator it;

	HWND CreateMyWindow() const;
	MainWindow();
	const static void Draw(HDC &);

	//���� 
	static NOTIFYICONDATA Tray;

	static HINSTANCE hinstance;
	//����ȫ�ֱ���
	HWND hWnd;
	
	WNDCLASS wc;
	
};

