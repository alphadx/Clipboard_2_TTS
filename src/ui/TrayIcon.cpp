#include "pch.h"
#include "TrayIcon.h"
#include <shellapi.h>

CTrayIcon::CTrayIcon(HWND hMainWnd, std::shared_ptr<CStateMachine> stateMachine, std::shared_ptr<CContextMenu> contextMenu)
    : m_hMainWnd(hMainWnd), m_stateMachine(stateMachine), m_contextMenu(contextMenu) {
    memset(&m_nid, 0, sizeof(m_nid));
}

CTrayIcon::~CTrayIcon() {
    Shell_NotifyIconW(NIM_DELETE, &m_nid);
}

void CTrayIcon::Initialize() {
    m_nid.cbSize = sizeof(m_nid);
    m_nid.hWnd = m_hMainWnd;
    m_nid.uID = 1; // Unico ID
    m_nid.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
    m_nid.uCallbackMessage = WM_TRAYICON;
    
    // Set default icon (fallback to standard info icon if no custom icon provided)
    m_nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    wcscpy_s(m_nid.szTip, L"Clipboard TTS Monitor");

    Shell_NotifyIconW(NIM_ADD, &m_nid);
}

void CTrayIcon::UpdateTooltip(const std::wstring& text) {
    wcsncpy_s(m_nid.szTip, text.c_str(), _TRUNCATE);
    m_nid.uFlags = NIF_TIP;
    Shell_NotifyIconW(NIM_MODIFY, &m_nid);
}

void CTrayIcon::UpdateStateVisuals() {
    EAppState state = m_stateMachine->GetState();
    switch (state) {
        case EAppState::Inactive:
            UpdateTooltip(L"Clipboard TTS Monitor [INACTIVO]");
            break;
        case EAppState::Listening:
            UpdateTooltip(L"Clipboard TTS Monitor [ACTIVO]");
            break;
        case EAppState::Speaking:
            UpdateTooltip(L"Hablando...");
            break;
        case EAppState::Suspended:
            UpdateTooltip(L"Clipboard TTS Monitor [RECONFIGURANDO]");
            break;
    }
}

void CTrayIcon::HandleTrayMessage(WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(wParam);
    
    if (LOWORD(lParam) == WM_RBUTTONUP) {
        POINT pt;
        GetCursorPos(&pt);
        m_contextMenu->Show(pt.x, pt.y);
    }
}
