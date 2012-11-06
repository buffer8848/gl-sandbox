#include <windows.h>
#include <wingdi.h>
#include "GLShader.h"
#include "DbgConsole.h"
#include "GLRenderer.h"
#include <iostream>
#include <fstream>
const char g_szClassName[] = "myWindowClass";
GLRenderer renderer;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
		case WM_SIZE:
			renderer.Reshape(LOWORD(lParam), HIWORD(lParam));
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

bool CreateOpenGLContext(HWND hwnd);
bool SetupPixels(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	RedirectIOToConsole();

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindow(
        g_szClassName,
        "Shader Sandbox",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    if(!SetupPixels(hwnd))
	{
		return GetLastError();
	}

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	
    HDC dc = GetDC(hwnd);

	if(renderer.CreateGLContext(&dc))
	{
		std::cout << "Created gl context successfully\n";
	}else{
		std::cout << "Could not create gl context!\n";
	}

	renderer.PrepareScene();

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);

		renderer.DrawScene(&dc);
    }
    return Msg.wParam;
}

bool SetupPixels(HWND hwnd)
{
	    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                        //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                        //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC dc = GetDC(hwnd);
    int pixelFormat = ChoosePixelFormat(dc, &pfd);
	
	if(pixelFormat == 0)
		return false;

	return (SetPixelFormat(dc, pixelFormat, &pfd)) ? true : false;
}
bool CreateOpenGLContext(HWND hwnd)
{

	return true;
}
