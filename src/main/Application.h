#pragma once
#include <windows.h>
#include <memory>
#include "config/ConfigManager.h"
#include "core/StateMachine.h"
#include "core/ClipboardListener.h"
#include "core/HotkeyManager.h"
#include "tts/TTSEngine.h"
#include "audio/AudioLogger.h"
#include "ui/TrayIcon.h"
#include "ui/ContextMenu.h"

class CApplication {
public:
    CApplication(HINSTANCE hInstance);
    ~CApplication();

    int Run();

private:
    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    HWND m_hMainWnd;
    
    std::shared_ptr<CConfigManager> m_config;
    std::shared_ptr<CStateMachine> m_stateMachine;
    std::shared_ptr<CAudioLogger> m_audioLogger;
    std::shared_ptr<CTTSEngine> m_ttsEngine;
    
    std::unique_ptr<CHotkeyManager> m_hotkeyManager;
    std::unique_ptr<CClipboardListener> m_clipboardListener;
    
    std::shared_ptr<CContextMenu> m_contextMenu;
    std::unique_ptr<CTrayIcon> m_trayIcon;
};
