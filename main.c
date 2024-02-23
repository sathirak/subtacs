#include <windows.h>
#include <stdio.h>

HWND hwndNextViewer; // Handle to the next clipboard viewer

// Function to handle clipboard changes
LRESULT CALLBACK ClipboardViewerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DRAWCLIPBOARD) {
        // Clipboard content has changed\033[31mThis text is in red\033[0m
        printf("\n\n\033[36m_______________________________________\033[0m\n\n");

        // Open the clipboard
        if (OpenClipboard(hwnd)) {
            // Get the clipboard data handle
            HANDLE hClipboardData = GetClipboardData(CF_TEXT);

            if (hClipboardData != NULL) {
                // Lock the data to get a pointer to the text
                char* clipboardText = (char*)GlobalLock(hClipboardData);

                if (clipboardText != NULL) {

                    int length = strcspn(clipboardText, "\n");
                    length = (length > 30) ? 30 : length;

                    printf("\033[33m%.*s", length, clipboardText);

                    printf("\n______________\033[0m");
                    printf("\n\n %s", clipboardText);

                    // Unlock the data
                    GlobalUnlock(hClipboardData);
                }
            }

            // Close the clipboard
            CloseClipboard();
        }

        // Call the next viewer in the chain
        SendMessage(hwndNextViewer, uMsg, wParam, lParam);
    }

    // Call the default window procedure for other messages
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// Function to set up the clipboard viewer
// Function to set up the clipboard viewer
void SetupClipboardViewer() {
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
        NULL
    );

    // Set the clipboard viewer window procedure
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ClipboardViewerProc);

    // Set up as a clipboard viewer and get the handle to the next viewer
    hwndNextViewer = SetClipboardViewer(hwnd);
}


int main() {

    SetConsoleTitle("subtacs");
    // Set up the clipboard viewer
    SetupClipboardViewer();

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Remove the viewer before exiting
    ChangeClipboardChain(hwndNextViewer, NULL);

    return 0;
}
