#include "pch.h"
#include "Application.h"

CApplication::CApplication(HINSTANCE hInstance) : m_hInstance(hInstance), m_hMainWnd(NULL) {
    // Initialize core components
    m_config = std::make_shared<CConfigManager>();
    m_config->LoadConfig();

    m_stateMachine = std::make_shared<CStateMachine>(EAppState::Listening); // Start listening by default
    
    m_audioLogger = std::make_shared<CAudioLogger>(m_config);
    m_ttsEngine = std::make_shared<CTTSEngine>(m_config, m_stateMachine);
}

CApplication::~CApplication() {
    if (m_hMainWnd) {
        DestroyWindow(m_hMainWnd);
    }
}

int CApplication::Run() {
    // Register window class
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEXW) };
    wcex.lpfnWndProc   = WndProcStatic;
    wcex.hInstance     = m_hInstance;
    wcex.lpszClassName = L"ClipboardTTSMonitorClass";
    RegisterClassExW(&wcex);

    // Create message-only window
    m_hMainWnd = CreateWindowExW(
        0, 
        L"ClipboardTTSMonitorClass", 
        L"ClipboardTTSMonitor", 
        0, 0, 0, 0, 0, 
        HWND_MESSAGE, NULL, m_hInstance, this
    );

    if (!m_hMainWnd) {
        return -1;
    }

    // Initialize remaining components requiring HWND
    m_clipboardListener = std::make_unique<CClipboardListener>(m_hMainWnd, m_stateMachine);
    m_hotkeyManager = std::make_unique<CHotkeyManager>(m_hMainWnd, m_config);
    m_contextMenu = std::make_shared<CContextMenu>(m_hMainWnd, m_config, m_ttsEngine, m_audioLogger);
    m_trayIcon = std::make_unique<CTrayIcon>(m_hMainWnd, m_stateMachine, m_contextMenu);

    // Initialize specific module logic
    m_ttsEngine->Initialize(m_hMainWnd);
    m_hotkeyManager->RegisterAllHotkeys();
    m_clipboardListener->Initialize();
    m_trayIcon->Initialize();
    m_trayIcon->UpdateStateVisuals();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK CApplication::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    CApplication* pApp = nullptr;
    if (message == WM_NCCREATE) {
        LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pApp = reinterpret_cast<CApplication*>(pcs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
    } else {
        pApp = reinterpret_cast<CApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pApp) {
        return pApp->WndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLIPBOARDUPDATE:
        if (m_clipboardListener) m_clipboardListener->OnClipboardUpdate();
        break;

    case WM_APP_CLIPBOARD_TEXT:
        {
            std::wstring* pText = reinterpret_cast<std::wstring*>(lParam);
            if (pText) {
                if (pText->length() > 40) {
                    m_ttsEngine->SpeakText(*pText);
                    m_trayIcon->UpdateStateVisuals();
                }
                delete pText;
            }
        }
        break;

    case WM_APP_TTS_EVENT:
        if (m_ttsEngine) {
            m_ttsEngine->OnSpeechEnd();
            m_trayIcon->UpdateStateVisuals();
        }
        break;

    case WM_HOTKEY:
        if (m_hotkeyManager) {
            ATOM atomId = static_cast<ATOM>(wParam);
            if (atomId == m_hotkeyManager->GetSkipAtomId()) {
                m_ttsEngine->SkipCurrent();
                m_trayIcon->UpdateStateVisuals();
            } else if (atomId == m_hotkeyManager->GetStopAllAtomId()) {
                m_ttsEngine->StopAll();
                m_trayIcon->UpdateStateVisuals();
            } else if (atomId == m_hotkeyManager->GetPauseResumeAtomId()) {
                m_ttsEngine->TogglePauseResume();
                m_trayIcon->UpdateStateVisuals();
            }
        }
        break;

    case WM_TRAYICON:
        if (m_trayIcon) m_trayIcon->HandleTrayMessage(wParam, lParam);
        break;

    case WM_COMMAND:
        if (m_contextMenu) m_contextMenu->HandleCommand(LOWORD(wParam));
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
