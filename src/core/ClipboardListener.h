#pragma once
#include <windows.h>
#include <memory>
#include "StateMachine.h"

// Custom message for when clipboard text is ready
constexpr UINT WM_APP_CLIPBOARD_TEXT = WM_APP + 1;

class CClipboardListener {
public:
    CClipboardListener(HWND hMainWnd, std::shared_ptr<CStateMachine> stateMachine);
    ~CClipboardListener();

    void Initialize();
    void OnClipboardUpdate();

private:
    static DWORD WINAPI ExtractTextAsync(LPVOID lpParam);

    HWND m_hMainWnd;
    std::shared_ptr<CStateMachine> m_stateMachine;
    bool m_isListening;
};
