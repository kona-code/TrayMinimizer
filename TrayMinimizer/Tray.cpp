#include "Tray.h"
#include "Resource.h"
#include <cstring>

TrayIcon::TrayIcon() : m_callbackMessage(WM_USER + 1) {
    memset(&m_nid, 0, sizeof(m_nid));
    m_hMenu = nullptr;
}

TrayIcon::~TrayIcon() {
    DestroyTrayIcon();
}

bool TrayIcon::CreateTrayIcon(HWND hwnd, HINSTANCE hInstance) {

    m_nid.cbSize = sizeof(m_nid);
    m_nid.hWnd = hwnd;
    m_nid.uID = 1;
    m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    m_nid.uCallbackMessage = m_callbackMessage;
    m_nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    wcscpy_s(m_nid.szTip, L"Tray Minimizer");
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    if (!Shell_NotifyIcon(NIM_ADD, &m_nid)) {
        return false;
    }

    HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));

    m_nid.hIcon = icon;
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    // create a context menu for the tray icon.
    m_hMenu = CreatePopupMenu();
    AppendMenu(m_hMenu, MF_STRING, 3, L"Hide Console");
    AppendMenu(m_hMenu, MF_STRING, 1, L"Show Console");
    AppendMenu(m_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(m_hMenu, MF_STRING, 4, L"Exit");    // option to exit


    return true;
}


void TrayIcon::DestroyTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
    if (m_hMenu) {
        DestroyMenu(m_hMenu);
        m_hMenu = nullptr;
    }
}

void TrayIcon::ShowContextMenu(HWND hwnd) {
    POINT p;
    GetCursorPos(&p);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(m_hMenu, TPM_BOTTOMALIGN | TPM_LEFTBUTTON, p.x, p.y, 0, hwnd, NULL);
}

void TrayIcon::UpdateTrayMenu() {
    // destroy the old menu if it exists
    if (m_hMenu) {
        DestroyMenu(m_hMenu);
        m_hMenu = nullptr;
    }

    // create a new menu
    m_hMenu = CreatePopupMenu();

    // add dynamic menu items
    AppendMenu(m_hMenu, MF_STRING, 6, L"Author");
    AppendMenu(m_hMenu, MF_STRING, 4, L"About and Source Code");
    AppendMenu(m_hMenu, MF_STRING, 5, L"Latest Executable Source");
    AppendMenu(m_hMenu, MF_SEPARATOR, 0, NULL);      // separator
    // add dynamic items (based on some condition)
    AppendMenu(m_hMenu, MF_STRING, 3, L"Hide Console");
    AppendMenu(m_hMenu, MF_STRING, 1, L"Show Console");
    AppendMenu(m_hMenu, MF_SEPARATOR, 0, NULL);      // separator

    // always include the "Exit" option
    AppendMenu(m_hMenu, MF_STRING, 7, L"Exit");

}