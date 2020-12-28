#pragma once

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include <vector>

#define FONT_SIZE 12
using namespace std;

class CVertScroll
{
public:
	CVertScroll();
	~CVertScroll();

	int set(const char *s);
	void buildFont(HDC hDC, bool textured=true);								// Build Our Bitmap Font
	void glPrint(const char *text);								// Custom GL "Print" Routine
	void draw();
	void drawTexScroll();
	
	bool setFont(CHAR fontName[32], int fontSize, BOOL italic, BOOL bold);
	bool buildTexture(COLORREF color);
	void move(float dy);
	GLuint LoadTextureMN(const int x, const int y, LPVOID pData);

	float m_xPos, m_yPos;
	int   m_pressed;
	int   m_counter;

	vector<string> m_s;
	HFONT m_currFont;
	GLuint m_scrollTex;
	int m_texW, m_texH;
	float m_y, m_incY;
};

