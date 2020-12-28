#include <windows.h>
#include "resource.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdlib.h>
#include <stdio.h> 
#include "VertScroll.h"
#include "minif.h"

#pragma comment(lib, "minifmod.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

HINSTANCE myhInstance;
HDC hDC1;
HWND hEdit1;
HGLRC hglrc1;
unsigned int g_tex1 = 0;
CVertScroll g_scroll;
miniF fMod;
// Set up the pixel format

int MySetPixelFormat(HDC hdc)
{


	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
		1,                                // version number 
		PFD_DRAW_TO_WINDOW |              // support window 
		PFD_SUPPORT_OPENGL |              // support OpenGL 
		PFD_DOUBLEBUFFER,                 // double buffered 
		PFD_TYPE_RGBA,                    // RGBA type 
		24,                               // 24-bit color depth 
		0, 0, 0, 0, 0, 0,                 // color bits ignored 
		0,                                // no alpha buffer 
		0,                                // shift bit ignored 
		0,                                // no accumulation buffer 
		0, 0, 0, 0,                       // accum bits ignored 
		32,                               // 32-bit z-buffer     
		0,                                // no stencil buffer 
		0,                                // no auxiliary buffer 
		PFD_MAIN_PLANE,                   // main layer 
		0,                                // reserved 
		0, 0, 0                           // layer masks ignored 
	};

	int  iPixelFormat;

	// get the device context's best, available pixel format match 
	if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox(NULL, "ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	// make that match the device context's current pixel format 
	if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}

HDC hdcDialog = NULL;


LRESULT onNcPaint(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// draw Frame
	//     HBRUSH mBrush = CreateSolidBrush( RGB(0,113,201) );
	HDC hdc = GetWindowDC(hwnd);
	//     HDC hdc = GetDCEx(hwnd, (HRGN)wParam, DCX_WINDOW);//|DCX_INTERSECTRGN);

	RECT mRect, wndRect;
	GetWindowRect(hwnd, &mRect);
	wndRect = mRect;
	mRect.right -= mRect.left;
	mRect.bottom -= mRect.top;
	mRect.left = 0;
	mRect.top = 0;

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); /* White Brush */
	FillRect(hdcDialog, &mRect, brush); /* Fills the toolbar's background white */





	// draw window text
	char wndText[100];
	RECT textRect;
	textRect.left = 9 + 16 + 9;
	textRect.top = 0;
	textRect.right = 1000;
	textRect.bottom = 32;
	GetWindowText(hwnd, wndText, 99);
	//int oldMode = SetBkMode(hdc, TRANSPARENT);
	//int oldMode = SetBkMode(memDC, TRANSPARENT);
	SetBkMode(hdc, TRANSPARENT);
	
	HFONT oldFont, hfont0 = CreateFont(-13, 0, 0, 0, 0, FALSE, FALSE, FALSE, 1, 0, 0, 0, 0, "Ms Shell Dlg");
	oldFont = (HFONT)SelectObject(hdc, hfont0);
	DrawText(hdc, wndText, -1, &textRect, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	SelectObject(hdc, oldFont);
	DeleteObject(hfont0);

	//   BitBlt(hdc, 0,0, mRect.right,mRect.bottom, memDC, 0,0, SRCCOPY);

	ReleaseDC(hwnd, hdc);
	//   ValidateRgn(hwnd, (HRGN)wParam);
	return 0;
}

unsigned int LoadTextureMN(const int x, const int y, LPVOID pData)
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

BOOL LoadTexture()
{
	/*	GLuint textureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	GLuint Status = FALSE;									// Status Indicator
	myhInstance = GetModuleHandle(NULL);

	HRSRC hrsrc = FindResource(myhInstance, MAKEINTRESOURCE(IDR_RAW1), "RAW");
	HGLOBAL hGlob = LoadResource(myhInstance, hrsrc);
	LPVOID pData = LockResource(hGlob);
	if (!pData)
		Status = FALSE;

	if (g_tex1 = LoadTextureMN(400, 200, pData)) 
	{
		Status = TRUE;									// Set The Status To TRUE
	}

	return Status;										// Return The Status
}

void NCPaint(HWND hWnd, HRGN hRgn)
{
	HDC hDC;
	char s[] = "RED";

	hDC = GetWindowDC(hWnd);
	TextOut(hDC, 0, 0, s, strlen(s));
	ReleaseDC(hWnd, hDC);
}

int rol(int x, int n) 
{
	int y = x << n | x >> (32 - n);

	for (int i = 0; i < n; i++)
		x = ((x & 0x80) ? 0x01 : 0x00) | (x << 1);

	x = x & 0xFFFF0000;
	y = y & 0x0000FFFF;

	x = x + y;
	return x;
}

void computeCode(HWND hWnd)
{

	int nCode = 0;
	HWND h = GetDlgItem(hWnd, IDC_EDIT2);
	char text[101];
	GetWindowText(h, text, 100);
	int len = strlen(text);
	if (len > 0)
		nCode = atoi(text);

	if (nCode == 0)
	{
		h = GetDlgItem(hWnd, IDC_EDIT3);
		SetWindowText(h, "none");
	}
	else
	{
		int nAnswer = rol(0x75BCD17 * (~(nCode & 0xABD13D59) & ~(~nCode & 0x542EC2A6)), 16);
		nAnswer = 0xDFB38D3u * nAnswer % 0x5F5E100;

		// converting nAnswer into a number
		sprintf(text, "%d", nAnswer);
		h = GetDlgItem(hWnd, IDC_EDIT3);
		SetWindowText(h, text);
	}
}

// +----------------------------------------------------------------------------
// | -DialogProc()-
// | This is the callback function that processes the messages sent to our
// | dialog box. Most everything of consequence in such a program is either
// | done here or called from here.
// +----------------------------------------------------------------------------
BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// check message type
	switch (uMsg)
	{
	case WM_COMMAND:
		// here's where you would process commands sent to the dialog box
		switch (LOWORD(wParam))
		{
		case IDOK:
			// OK button clicked - perform data validation and such here
			EndDialog(hWnd, TRUE);
			break;

		case IDCANCEL:
			// Cancel button clicked
			EndDialog(hWnd, FALSE);
			break;
		case IDC_EDIT2:
			if (HIWORD(wParam) == EN_CHANGE)
				computeCode(hWnd);
			break;
		}

	//case WM_NCPAINT:
	//{
		//DefWindowProc(hWnd, uMsg, wParam, lParam);
		//NCPaint(hWnd, (HRGN)wParam);
		//return 1;
		/*
			HDC hdc = ::GetWindowDC(hWnd);

			::SetTextColor(hdc, RGB(0, 0, 0));
			::SetBkColor(hdc, RGB(255, 255, 255));
			HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); 
			RECT r = { 0,-100,500,600 };
			FillRect(hdc, &r, brush); 
			ReleaseDC(hWnd, hdc);
			return 0;*/
	//}
		//  printf("WM_NCPAINT -");
		//  printf("wParam: 0x%08d lParam 0x%08x\n", wParam, lParam);
		//onNcPaint(hWnd, wParam, lParam);
		//return 0;
	case  WM_KEYDOWN:
	case  WM_KEYUP:
	{
		//HWND e2 = GetDlgItem(hWnd, IDC_EDIT2);
		//SetFocus(e2);
		//SendDlgItemMessage(hWnd, IDC_EDIT2, WM_SETFOCUS, 0, 0);
		//SendMessage(e2, uMsg, wParam, lParam);
		break;
	}

	case WM_INITDIALOG:
	{

		HICON hIcon = LoadIcon(myhInstance, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		if (!fMod.LoadMusic(IDR_MODFILE1))
			printf("\n Error load File .xm\n");
		else
			fMod.Play();

		//int extendedStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		//SetWindowLong(hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

		HWND e2 = GetDlgItem(hWnd, IDC_EDIT2);

		//int extendedStyle = GetWindowLong(e2, GWL_EXSTYLE);
		//SetWindowLong(e2, GWL_EXSTYLE, extendedStyle);

		SetWindowText(e2, "<enter code here>");
		//ShowWindow(e2, SWP_HIDEWINDOW);
		HWND e3 = GetDlgItem(hWnd, IDC_EDIT3);
		SetWindowText(e3, "none");
		int extendedStyle = GetWindowLong(e3, GWL_EXSTYLE);
		SetWindowLong(e3, GWL_EXSTYLE, extendedStyle| ES_READONLY);

		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		RECT r;
		GetWindowRect(hWnd, &r);
		int xlen = r.right - r.left;
		int ylen = r.bottom - r.top;
		if (ylen < 0)
			ylen = -ylen;
		int x = (w - xlen) >> 1;
		int y = (h - ylen) >> 1;

		SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//EnableWindow(e3, 0);
		//SendMessage(e3, /*EM_SETCUEBANNER*/0x1501, 0, (LPARAM)L"<answer>");
		
		//long gwl = GetWindowLong(hWnd, GWL_STYLE);
		//gwl = gwl & ~WS_SYSMENU;
		//SetWindowLong(hWnd, GWL_STYLE, gwl);

		//HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
		//SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)brush);
		hdcDialog = GetDC(hWnd);

		
		//int aElements[2] = { COLOR_WINDOW, COLOR_ACTIVECAPTION };
		//DWORD aOldColors[2];
		//DWORD aNewColors[2];

		//aOldColors[0] = GetSysColor(aElements[0]);
		//aOldColors[1] = GetSysColor(aElements[1]);
		//aNewColors[0] = RGB(0xff, 0xff, 0xff);  // light gray 
		//aNewColors[1] = RGB(0xA0, 0xA0, 0xA0);  // dark purple 

		//SetSysColors(2, aElements, aNewColors);
		//SetSysColors(2, aElements, aOldColors);

		//SetBkColor(hdcDialog, RGB(0, 0, 0));
		//SendMessage(hWnd, WM_ERASEBKGND, (WPARAM)hdcDialog, NULL);
		// perform any necessary initialization here - return FALSE if you
		// set the focus to a control yourself, otherwise just break and
		// allow the function to return TRUE to indicate that the keyboard
		// focus should be set automatically
		// First, let's get the handle to all the window we created in the Dialog
		// with the resource editor
		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);

		// For every window, we get the HDC
		hDC1 = GetDC(hEdit1);

		// OpenGL Initialization

		MySetPixelFormat(hDC1);

		hglrc1 = wglCreateContext(hDC1);
		wglMakeCurrent(hDC1, hglrc1);

		LoadTexture();

		g_scroll.set(" . . . . .     REVERSE ENGiNEER'S DREAM proudly presents another fine release                                       Cutlet Maker v1.0f *keygen*                                        Credits de cette petite CrackTro . . . . . .       LOGO by Xspider/RED   . . . .       Muzaxx 'Anarchy_menu-06' by 4-mat  . . . .       Code by Xylitol/RED       Greetings to  . . . . . .                 BytePlayeR                 Xspider                 Bispoo                 qpt^j                 KKR                 MiSSiNG iN ByTES                 Encrypto                 fr334life                                   . . . . h o p . . . .                                       . . . . h o p . . .                                       . . . . h o p . . .                            Teams greeting now . . . . . . .                 AT4RE                 SND                 CiM                 FFF                 URET                 TSRh                 OMB                 DARKSiDERS             ");

		SetTimer(hWnd, 1, 30, NULL);

		break;
	}
	case WM_CTLCOLORDLG:
	{
		return (INT_PTR)GetStockObject(WHITE_BRUSH);
	}

	case WM_ERASEBKGND:
	{
		HPEN pen;
		HBRUSH	brush;
		RECT rect;

		pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
		brush = CreateSolidBrush(RGB(0, 0, 0));
		HGDIOBJ  oPen = SelectObject((HDC)wParam, pen);
		HGDIOBJ  oBrush = SelectObject((HDC)wParam, brush);

		GetClientRect(hWnd, &rect);

		Rectangle((HDC)wParam, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject((HDC)wParam, oPen);
		SelectObject((HDC)wParam, oBrush);
		DeleteObject(pen);
		DeleteObject(brush);

		// here, draw bitmap
		HBITMAP hBitmap = LoadBitmap(myhInstance, MAKEINTRESOURCE(IDB_BITMAP1)); // from "resource.rc/Bitmap/IDB_KEYBD2"
		HDC hMemDC = CreateCompatibleDC((HDC)wParam);
		HANDLE prev = SelectObject(hMemDC, hBitmap);

		RECT r;
		
		HWND hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
		GetWindowRect(hEdit2, &r);


		//BitBlt((HDC)wParam, 0, 500, 235, 49, hMemDC, 0, 0, SRCCOPY);
		BitBlt((HDC)wParam, 150, 330, 235, 80, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, prev);
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);

		return TRUE;
		break;
	}

	
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcEdit = (HDC)wParam; //Get handles
		SetTextColor(hdcEdit, RGB(255, 255, 255)); // Text color
		SetBkMode(hdcEdit, TRANSPARENT); // EditBox Backround Mode (note: OPAQUE can be used)
		static HBRUSH hbrushEditBox = CreateSolidBrush(RGB(0, 0, 0));
		SetBkColor(hdcEdit, (LONG)hbrushEditBox); // Backround color for EditBox
		return (LONG)hbrushEditBox; // Paint it
		break;
	}
	
	case WM_CTLCOLOREDIT:
	{
		HDC hdcEdit = (HDC)wParam; //Get handles
		SetTextColor(hdcEdit, RGB(255, 255, 255)); // Text color
		SetBkMode(hdcEdit, TRANSPARENT); // EditBox Backround Mode (note: OPAQUE can be used)
		static HBRUSH hbrushEditBox = CreateSolidBrush(RGB(0, 0, 0));
		SetBkColor(hdcEdit, (LONG)hbrushEditBox); // Backround color for EditBox
		return (LONG)hbrushEditBox; // Paint it
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); /* White Brush */

		//SendMessage(hWnd, WM_ERASEBKGND, brush, NULL); /* Erases the background */

		//GetClientRect(hWnd_toolbar, rectangle); /* Gets the toolbar's area */

		//FillRect(GetDC(hWnd_toolbar), &rectangle, whitebrush); /* Fills the toolbar's background white */

		EndPaint(hWnd, &ps);
		/*
		HWND s1 = GetDlgItem(hWnd, IDC_STATIC1);
		hdc = GetDC(s1);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		SendMessage(s1, WM_ERASEBKGND, 0, 0);
		ReleaseDC(s1, hdc);
		*/

		break;
	}
	case WM_TIMER:
	{

		int captionHeight = 25;
		//SendDlgItemMessage(hWnd, IDC_EDIT2, WM_SETFOCUS, 0, 0);
		wglMakeCurrent(hDC1, hglrc1);
		RECT r;
		GetWindowRect(hEdit1, &r);
		glViewport(0, 0, r.right-r.left, r.bottom-r.top);


		glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					// Black Background
		glClearDepth(1.0f);							// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
		//glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/*
		glViewport(0, r.bottom - r.top - captionHeight, r.right - r.left, r.bottom - r.top);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, r.right - r.left, 0, captionHeight, -1, 1);
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		{
			glVertex3f(0, 0, 0);
			glVertex3f(r.right - r.left, 0, 0);
			glVertex3f(r.right - r.left, captionHeight, 0);
			glVertex3f(0, captionHeight, 0);
		}
		glEnd();



		glViewport(0, 0, r.right - r.left, r.bottom - r.top- captionHeight);
		*/

		glViewport(0, 0, r.right - r.left, r.bottom - r.top);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, r.right - r.left, 0, r.bottom - r.top, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, g_tex1);
		glEnable(GL_TEXTURE_2D);

		static float step = 0.0f;

		float y2[2] = { 150.0f,  350.0f };
		float y1[2] = { -200.0f, 150.0f};
		float y [2];

		// linear interpolation
		y[0] = (1.0f - step) * y1[0] + step * y2[0];
		y[1] = (1.0f - step) * y1[1] + step * y2[1];

		// updating step
		const int logoAnimationFrames = 120;
		step = step + 1.0f / logoAnimationFrames;
		if (step > 1.0f)
			step = 1.0f;
		glColor3f(1, 1, 1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3f(20, y[0], 0);			

			glTexCoord2f(1, 1);
			glVertex3f(r.right - r.left-20, y[0], 0.0f);	//left

			glTexCoord2f(1, 0);
			glVertex3f(r.right - r.left-20, y[1], 0.0f);	//left
			
			glTexCoord2f(0, 0);
			glVertex3f(20, y[1], 0.0f);	//left
		glEnd();
		glDisable(GL_BLEND);

		g_scroll.draw();
		

		SwapBuffers(hDC1);

		break;
	}

	default:
		// return zero if we do not process this message
		return FALSE;
	}

	// return nonzero if we did process the message
	return TRUE;
}

// +----------------------------------------------------------------------------
// | -WinMain()-
// | Program entry point. Normally you'd find a message loop in WinMain(), but
// | for a dialog-based application, all we have to do is create the dialog and
// | then quit. You should probably add some kind of error-checking facility in
// | case the call to DialogBox() fails. You might also wish to do something
// | with the return value of DialogBox() if it's important. Finally, you could 
// | use DialogBoxParam() instead of DialogBox() if you wanted to pass a 
// | parameter to the dialog box.
// +----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	::myhInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, DialogProc);	
	return 0;
}

