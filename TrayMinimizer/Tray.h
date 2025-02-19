#pragma once

#include <windows.h>

class TrayIcon {
public:
    TrayIcon();
    ~TrayIcon();

    // create tray icon with a context menu
    bool CreateTrayIcon(HWND hwnd, HINSTANCE hInstance);

    // remove tray icon
    void DestroyTrayIcon();

    // shows the context menu on right-click
    void ShowContextMenu(HWND hwnd);

    // returns the tray icon's notification message ID
    UINT GetCallbackMessage() const { return m_callbackMessage; }

private:
    NOTIFYICONDATA m_nid;
    HMENU m_hMenu;
    UINT m_callbackMessage; // typically WM_USER+1
};
