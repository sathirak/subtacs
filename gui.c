#include <windows.h>

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global variable for the edit control
HWND g_hEdit;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const char CLASS_NAME[] = "SampleWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Set background color
    wc.style = CS_HREDRAW | CS_VREDRAW;            // Redraw on resize

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED,               // Enable layered window
        CLASS_NAME,                  // Window class
        "subtacss",                  // Window text
        WS_OVERLAPPED |              // Non-resizable window style
            WS_CAPTION | WS_SYSMENU, // Include caption and system menu
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 500, // Set width and height here
        NULL,                                   // Parent window
        NULL,                                   // Menu
        hInstance,                              // Instance handle
        NULL                                    // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Set the window to be translucent with Microsoft Azure Windows blue color
    SetLayeredWindowAttributes(hwnd, RGB(21, 163, 226), 240, LWA_COLORKEY | LWA_ALPHA);

    // Create the edit control to display sentences
    g_hEdit = CreateWindow(
        "EDIT",                                                             // Predefined class; Unicode assumed
        "",                                                                 // Edit control text
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, // Styles
        10, 10, 300, 400,                                                   // Position and size
        hwnd,                                                               // Parent window
        NULL,                                                               // No menu
        hInstance,                                                          // Instance handle
        NULL                                                                // No additional data
    );

    // Create a button that appends sentences when clicked
    CreateWindow(
        "BUTTON",                                              // Predefined class; Unicode assumed
        "Click me",                                            // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        10, 420, 100, 30,                                      // Position and size
        hwnd,                                                  // Parent window
        (HMENU)1,                                              // Button identifier (used in WM_COMMAND)
        hInstance,                                             // Instance handle
        NULL                                                   // No additional data
    );

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) // Button click event
        {
            // Append a sentence to the edit control
            SendMessage(g_hEdit, EM_SETSEL, -1, -1);                                         // Set selection to the end
            SendMessage(g_hEdit, EM_REPLACESEL, 0, (LPARAM) "IMagine this a clipboard\r\n"); // Append text
        }
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
