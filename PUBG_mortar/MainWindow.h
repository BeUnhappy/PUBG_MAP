#pragma once
#include "Setting.h"

class MainWindow
{
public:
	static MainWindow& GteCreateWindow(HINSTANCE hinstance);

	HWND GethWnd() const;

	//��ֹʹ�ø��ƹ��캯������
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	static std::vector<POINT>& GetPoint();
	void updataDraw();

	//���� ���ڻ��������ı��� ,Ŀǰ�Ͻ�����������
	static int POINT_100M;
	static int distanceBetweenPoints;
	static int distanceBetweenPoints_Game;

	void AddIconTray();
	void RemoveTray();

	static RECT WindowSize;
	static bool DrawPoint;
private:
	//����
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND CreateMyWindow() const;
	MainWindow(HINSTANCE hinstance);
	const static void Draw(HDC &);

	static std::vector<POINT> pointList;
	static RECT BackGround;

	

	//���� 
	static NOTIFYICONDATA Tray;

	HINSTANCE hinstance;
	//����ȫ�ֱ���
	HWND hWnd;
	
	WNDCLASS wc;
	
};

