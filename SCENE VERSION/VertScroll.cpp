#include "VertScroll.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include "objbase.h"
#include "BitmapMan.h"

using namespace std;

CVertScroll::CVertScroll()
{
	m_yPos = 120;
	m_xPos = 500;
	m_currFont = NULL;
	m_texW = 0;
	m_texH = 0;

	m_scrollTex = 0;

	m_pressed = -1;
	m_counter = 0;
	m_y = 0;
}

CVertScroll::~CVertScroll()
{
}

void CVertScroll::drawTexScroll()
{
	glBindTexture(GL_TEXTURE_2D, m_scrollTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*_MIPMAP_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	for (float x = 0.0f; x < 360.0f; x++)
	{
		float X = m_y + x;
		float s0 = sin(X/180.0f * 3.14159f)*20; 
		float s1 = sin((X+1)/180.0f * 3.14159f)*20;
		glBegin(GL_QUADS);
		{
			glTexCoord2f(x/360.0f, 0); 	        glVertex3f(m_xPos + m_texW / 360.0f * (x + 0), m_yPos - m_texH + s0, 0.0f);
			glTexCoord2f((x+1) / 360.0f, 0); 	glVertex3f(m_xPos + m_texW / 360.0f * (x + 1), m_yPos - m_texH + s1, 0.0f);
			glTexCoord2f((x+1) / 360.0f, 1); 	glVertex3f(m_xPos + m_texW / 360.0f * (x + 1), m_yPos + s1, 0.0f);
			glTexCoord2f(x / 360.0f, 1); 	    glVertex3f(m_xPos + m_texW / 360.0f * (x + 0), m_yPos + s0, 0.0f);
		}
		glEnd();
	}
	/*
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); 	glVertex3f(m_xPos, m_yPos - m_texH + y, 0.0f);
		glTexCoord2f(1, 0); 	glVertex3f(m_xPos + m_texW, m_yPos - m_texH + y, 0.0f);
		glTexCoord2f(1, 1); 	glVertex3f(m_xPos + m_texW, m_yPos + y, 0.0f);
		glTexCoord2f(0, 1); 	glVertex3f(m_xPos, m_yPos + y, 0.0f);
	}
	glEnd();
	*/
	glDisable(GL_TEXTURE_2D);
}

void CVertScroll::draw()
{
	m_xPos -= 6;

	if (m_xPos + m_texW < 0)
		m_xPos = 500;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	m_yPos = 40;

	m_y = m_y + 4.0f;
	if (m_y > 360.0f)
		m_y -= 360.0f;

	glPushMatrix();
	/*glTranslatef(+250.0f, +120.0f / 2.0f, 0.0f);
	glRotatef(s-0.5f, 0, 0, 1);
	glTranslatef(-250.0f, -120.0f/2.0f, 0.0f);
	*/
	for (int y = 0; y < 7; y++)
	{
		switch (y)
		{
		case 0:
		case 6:
		case 1:
		case 5:
		case 2:
		case 4:
			glColor3f(1, 1, 1);
			break;
		case 3:
			glColor3f(1, 0.647059f, 0);
			break;
		}

		drawTexScroll();
		m_yPos += m_texH * 1.25f;
	}
	glPopMatrix();
	glDisable(GL_BLEND);
}

bool CVertScroll::setFont(CHAR fontName[32], int fontSize, BOOL italic, BOOL bold)
{
	LOGFONT log;
	log.lfHeight = fontSize;
	log.lfWidth = fontSize;
	log.lfEscapement = 0;
	log.lfOrientation = 0;
	log.lfWeight = (bold) ? FW_BOLD : 0;
	log.lfItalic = italic;
	log.lfUnderline = 0;
	log.lfStrikeOut = 0;
	log.lfCharSet = DEFAULT_CHARSET;
	log.lfOutPrecision = OUT_TT_PRECIS;
	log.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	//log.lfQuality        = PROOF_QUALITY;
	//log.lfQuality        = DEFAULT_QUALITY;
	log.lfQuality = ANTIALIASED_QUALITY;
	log.lfPitchAndFamily = 0;
	memcpy(log.lfFaceName, fontName, sizeof(CHAR) * 32);

	if (m_currFont)
		DeleteObject(m_currFont);
	m_currFont = CreateFontIndirect(&log);
	if (!m_currFont)
		return 0;
	return 1;
}

bool CVertScroll::buildTexture(COLORREF color)
{
	HDC hdc = CreateCompatibleDC(NULL);
	if (!hdc)
		return 0;
	static int m_iTextures = 0;


	if (m_iTextures == 0)
	{
		::CoInitialize(NULL);
		m_iTextures++;
	}
	//SetStretchBltMode(hdc, HALFTONE);
	//SetBkColor(hdc, RGB(0,0,0));
	SetBkMode(hdc, TRANSPARENT);
	//SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, 0);
	//SetDCPenColor(hdc, fg);
	SetTextColor(hdc, color);
	HFONT oldFont = (HFONT)SelectObject(hdc, m_currFont);

	TEXTMETRIC infoText;
	GetTextMetrics(hdc, &infoText);
	SIZE maxSize;
	maxSize.cx = 0;
	maxSize.cy = 0;

	for (unsigned int i = 0; i < m_s.size(); i++)
	{
		SIZE s;
		GetTextExtentPoint32(hdc, m_s[i].c_str(), m_s[i].length(), &s);
		if (s.cx > maxSize.cx)
			maxSize.cx = s.cx;
		maxSize.cy += s.cy;
	}

	maxSize.cx += 2;
	maxSize.cy += 2;

	// now, we are going to create a memory bitmap
	BITMAPINFO bmi;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biWidth = maxSize.cx;
	bmi.bmiHeader.biHeight = maxSize.cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biSizeImage = maxSize.cx*maxSize.cy * 4;	// 4 bytes per pixel

	//GlobalCompact(-1);
	BYTE * pixels = NULL;
	HBITMAP bmpHandle = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void **)&pixels, NULL, NULL);
	if (!bmpHandle)
		return 0;

	COLORREF *p = (COLORREF *)pixels;
	int nColors = bmi.bmiHeader.biSizeImage / 4;
	//COLORREF bk = bkColor.ToColorRef();

	//black but opaque
	for (int i = 0; i < nColors; i++, p++)
		//*p = 0;
		*p = *p | 0xff000000;


	HBITMAP pOldBmp = (HBITMAP)SelectObject(hdc, bmpHandle);

	int y0 = 0;
	int x0 = 0;
	SIZE textSize;
	for (unsigned int i = 0; i < m_s.size(); i++)
	{
		GetTextExtentPoint32(hdc, m_s[i].c_str(), m_s[i].length(), &textSize);
		TextOut(hdc, x0, y0, m_s[i].c_str(), m_s[i].length());
		y0 += textSize.cy;
	}

	p = (COLORREF *)pixels;

	for (int i = 0; i < nColors; i++, p++)
		*p = (*p & 0xff000000) ? 0 : *p | 0xffffffff;
		//*p = 0xff00ffff;
	//*p = (*p == 0) ? 0xff000000 : *p | 0xff000000;
//BitmapMan b; b.SetRGBAData(maxSize.cy, maxSize.cx, pixels); b.SaveBMP("c:\\temp\\test.bmp");


	m_texW = maxSize.cx;
	m_texH = maxSize.cy;


	m_scrollTex = LoadTextureMN(m_texW, m_texH, pixels);
	//*p = 0xffffffff;
	//m_scrollTex = LoadTextureMN(12, 12, p);

	SelectObject(hdc, pOldBmp);
	SelectObject(hdc, oldFont);
	DeleteObject(bmpHandle);
	return true;
}

GLuint CVertScroll::LoadTextureMN(const int x, const int y, LPVOID pData)
{
	GLuint textureID; // = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_LINEAR*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, x, y, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	GLenum e = glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	e = glGetError();
	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();

	return textureID;
}

void CVertScroll::move(float dy)
{
	m_pressed = (dy < 0);
	m_counter = 10;

	m_yPos += dy;
	if (m_yPos - m_texH > 1024)
		m_yPos = m_yPos - m_texH - 1024;
	else if (m_yPos < 0)
		m_yPos = 0;

}

int CVertScroll::set(const char *s)
{
	m_s.clear();
	m_s.push_back(std::string(s));
	setFont("TERMINAL", FONT_SIZE, 0, 0);
	buildTexture(0xff000000);
	return 0;
}

