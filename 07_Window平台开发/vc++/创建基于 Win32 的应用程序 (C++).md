创建基于 Win32 的应用程序 (C++)

1.在“文件”菜单上，单击“新建”，然后单击“项目”。

2.在“新建项目”对话框的左侧窗格中，单击“已安装的模板”，单击“Visual C++”，然后选择“Win32”。 在中间窗格中，选择“Win32 项目”。 

在“名称”框中，键入项目名称，例如 win32app。 单击“确定”。 

3.在“Win32 应用程序向导”的“欢迎”页上，单击“下一步”。

4.在“应用程序设置”页上的在“应用程序类型”下，选择“Windows 应用程序”。 在“附加选项”下，选择“空项目”。 单击“完成”创建项目。 

5.在“解决方案资源管理器”中，右击 Win32app 项目，单击“添加”，然后单击“新建项”。 在“添加新项”对话框中选择“C++ 文件(.cpp)”。 在“名称”框中，键入文件名称，例如 GT_HelloWorldWin32.cpp。 单击“添加”。 

启动基于 Win32 的应用程序
1.就像每个 C 应用程序和 C++ 应用程序都以 main 函数作为起点那样，每个基于 Win32 的应用程序同样必须要有 WinMain 函数。 WinMain 具有以下语法。 

复制代码 int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow);有关此函数的参数和返回值的信息，请参见 WinMain 函数。

2.由于应用程序代码必须使用现有定义，因此应将 include 语句添加到文件中。

复制代码 #include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>3.除 WinMain 函数外，每个基于 Win32 的应用程序还必须具有一个窗口过程函数。 此函数通常名为 WndProc。 WndProc 具有以下语法。 

复制代码 LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);此函数处理应用程序从操作系统接收的许多消息。 例如，在具有对话框（该对话框中有一个“确定”按钮）的应用程序中，如果用户单击该按钮，操作系统就会向该应用程序发送一条消息，告知已单击该按钮。 WndProc 负责响应该事件。 在此示例中，相应的响应可能是关闭该对话框。 

有关更多信息，请参见窗口过程。

向 WinMain 函数添加功能
1.在 WinMain 函数中，创建 WNDCLASSEX 类型的窗口类结构。 此结构包含有关该窗口的信息，例如，应用程序图标、窗口的背景色、要在标题栏中显示的名称、窗口过程函数的名称等等。 下面的示例演示一个典型 WNDCLASSEX 结构。 

复制代码     WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));有关此结构的各字段的信息，请参见 WNDCLASSEX。

2.现在您已经创建一个窗口类，接下来必须将其注册。 使用 RegisterClassEx 函数，并将窗口类结构作为参数进行传递。 

复制代码     if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }3.现在可以创建一个窗口。 使用 CreateWindow 函数。 

复制代码 static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

// The parameters to CreateWindow explained:
// szWindowClass: the name of the application
// szTitle: the text that appears in the title bar
// WS_OVERLAPPEDWINDOW: the type of window to create
// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
// 500, 100: initial size (width, length)
// NULL: the parent of this window
// NULL: this application does not have a menu bar
// hInstance: the first parameter from WinMain
// NULL: not used in this application
HWND hWnd = CreateWindow(
    szWindowClass,
    szTitle,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    500, 100,
    NULL,
    NULL,
    hInstance,
    NULL
);
if (!hWnd)
{
    MessageBox(NULL,
        _T("Call to CreateWindow failed!"),
        _T("Win32 Guided Tour"),
        NULL);

    return 1;
}此函数返回 HWND，它是某个窗口的句柄。 有关更多信息，请参见 Windows 数据类型。 

4.现在，使用下列代码来显示窗口。

复制代码 // The parameters to ShowWindow explained:
// hWnd: the value returned from CreateWindow
// nCmdShow: the fourth parameter from WinMain
ShowWindow(hWnd,
    nCmdShow);
UpdateWindow(hWnd);此时，所显示的窗口不会有太多内容，因为您尚未实现 WndProc 函数。

5.现在添加一个消息循环以侦听操作系统发送的消息。 如果应用程序收到一条消息，则此循环会将该消息调度至 WndProc 函数以接受处理。 消息循环类似于下列代码。 

复制代码     MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;有关消息循环中各结构和函数的更多信息，请参见 MSG、GetMessage、TranslateMessage 和 DispatchMessage。

此时，WinMain 函数应与下列代码类似。

复制代码 int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application dows not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}向 WndProc 函数添加功能
1.若要启用 WndProc 函数来处理应用程序所收到的消息，请实现 switch 语句。

要处理的第一条消息是 WM_PAINT 消息。 如果必须更新所显示的应用程序窗口的一部分，该应用程序就会收到此消息。 （首次显示该窗口时，必须将其全部更新。） 

若要处理 WM_PAINT 消息，请首先调用 BeginPaint，然后处理用于布局该窗口中的文本、按钮和其他控件的所有逻辑，再调用 EndPaint。 对于此应用程序，开始调用和结束调用之间的逻辑会在窗口中显示字符串“Hello, World!”。 在下列代码中，请注意 TextOut 函数用于显示该字符串。 

复制代码 PAINTSTRUCT ps;
HDC hdc;
TCHAR greeting[] = _T("Hello, World!");

switch (message)
{
case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);

    // Here your application is laid out.
    // For this introduction, we just print out "Hello, World!"
    // in the top left corner.
    TextOut(hdc,
        5, 5,
        greeting, _tcslen(greeting));
    // End application-specific layout section.

    EndPaint(hWnd, &ps);
    break;
}2.应用程序通常会处理许多其他消息，例如 WM_CREATE 和 WM_DESTROY。 下列代码展示了一个基本但完整的 WndProc 函数。 

复制代码 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, World!");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, World!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}  示例 
生成此示例
1.创建本演练中之前“创建基于 Win32 的项目”中的基于 Win32 的项目。

2.复制这些步骤之后的代码，然后将其粘贴到 GT_HelloWorldWin32.cpp 源文件中。

3.在“生成”菜单上，单击“生成解决方案”。

4.若要运行该应用程序，请按 F5。 在显示屏的左上角应出现一个窗口，窗口中含有文本“Hello World!”。 

代码
复制代码 // GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, World!");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, World!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}