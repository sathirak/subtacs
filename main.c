#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"

struct clipboard_container container_in;

HWND hwndNextViewer; // Handle to the next clipboard viewer


BOOL open_clipboard(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Open the clipboard
    if (OpenClipboard(hwnd))
    {
        struct clipboard_container CargoClip;
        // Check for HTML format
        UINT htmlFormat = RegisterClipboardFormat(TEXT("HTML Format"));

        HANDLE hHtmlData = GetClipboardData(htmlFormat);

        if (hHtmlData != NULL)
        {
            // Lock the data to get a pointer to the HTML text
            char *htmlText = (char *)GlobalLock(hHtmlData);

            if (htmlText != NULL)
            {
                // Find and print HTML content
                check_html(htmlText);

                // Unlock the data
                GlobalUnlock(hHtmlData);
            }
        }

        // Handle plain text format (CF_TEXT)
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);

        if (hClipboardData != NULL)
        {
            // Lock the data to get a pointer to the text
            char *clipboardText = (char *)GlobalLock(hClipboardData);

            if (clipboardText != NULL)
            {
                // process the plain text content
                process_container_in(clipboardText);

                // Unlock the data
                GlobalUnlock(hClipboardData);
            }
        }

        // Close the clipboard
        CloseClipboard();

        return TRUE;
    }

    return FALSE;
}

LRESULT CALLBACK view_clipboard(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DRAWCLIPBOARD)
    {
        // Open the clipboard
        if (uMsg == WM_DRAWCLIPBOARD)
        {
            // Open the clipboard
            if (open_clipboard(hwnd, uMsg, wParam, lParam))
            {
                // Call the next viewer in the chain
                SendMessage(hwndNextViewer, uMsg, wParam, lParam);
            }
        }
        // Call the next viewer in the chain
        SendMessage(hwndNextViewer, uMsg, wParam, lParam);
    }
    // Call the default window procedure for other messages
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to set up the clipboard viewer
void SetupClipboardViewer()
{
    // Create an invisible window to handle clipboard messages
    HWND hwnd = CreateWindowW(
        L"STATIC",
        L"Clipboard Viewer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        NULL,
        NULL);

    // Set the clipboard viewer window procedure
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)view_clipboard);

    // Set up as a clipboard viewer and get the handle to the next viewer
    hwndNextViewer = SetClipboardViewer(hwnd);
}

int main()
{

    SetConsoleTitle("subtacs");
    // Set up the clipboard viewer
    SetupClipboardViewer();

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Remove the viewer before exiting
    ChangeClipboardChain(hwndNextViewer, NULL);

    return 0;
}