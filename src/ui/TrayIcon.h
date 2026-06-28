#pragma once
#include <windows.h>
#include <memory>
#include <string>
#include "core/StateMachine.h"
#include "ContextMenu.h"

constexpr UINT WM_TRAYICON = WM_USER + 1;

class CTrayIcon {
public:
    CTrayIcon(HWND hMainWnd, std::shared_ptr<CStateMachine> stateMachine, std::shared_ptr<CContextMenu> contextMenu);
    ~CTrayIcon();

    void Initialize();
    void UpdateTooltip(const std::wstring& text);
    void UpdateStateVisuals();
    
    void HandleTrayMessage(WPARAM wParam, LPARAM lParam);

private:
    HWND m_hMainWnd;
    std::shared_ptr<CStateMachine> m_stateMachine;
    std::shared_ptr<CContextMenu> m_contextMenu;
    NOTIFYICONDATAW m_nid;
};
