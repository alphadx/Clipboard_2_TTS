#include "pch.h"
#include "ClipboardListener.h"
#include "utils/RaiiWrappers.h"

CClipboardListener::CClipboardListener(HWND hMainWnd, std::shared_ptr<CStateMachine> stateMachine)
    : m_hMainWnd(hMainWnd), m_stateMachine(stateMachine), m_isListening(false) {
}

CClipboardListener::~CClipboardListener() {
    if (m_isListening && m_hMainWnd) {
        RemoveClipboardFormatListener(m_hMainWnd);
    }
}

void CClipboardListener::Initialize() {
    if (m_hMainWnd) {
        m_isListening = AddClipboardFormatListener(m_hMainWnd) != FALSE;
    }
}

void CClipboardListener::OnClipboardUpdate() {
    if (m_stateMachine->GetState() != EAppState::Listening) {
        return;
    }
    
    // Create worker thread
    HANDLE hThread = CreateThread(NULL, 0, ExtractTextAsync, reinterpret_cast<LPVOID>(m_hMainWnd), 0, NULL);
    if (hThread != NULL) {
        CloseHandle(hThread); // We don't need to wait for it here
    }
}

DWORD WINAPI CClipboardListener::ExtractTextAsync(LPVOID lpParam) {
    HWND hMainWnd = reinterpret_cast<HWND>(lpParam);
    
    // PASO 1: Retardo anti-deadlock
    Sleep(50); // Permite que la app origen libere el clipboard

    // PASO 2: Intentar abrir clipboard con timeout usando RAII
    std::unique_ptr<RaiiWrappers::AutoClipboard> clipboard;
    for (int attempt = 0; attempt < 5; ++attempt) {
        clipboard = std::make_unique<RaiiWrappers::AutoClipboard>(nullptr);
        if (clipboard->IsOpen()) {
            break;
        }
        clipboard.reset();
        Sleep(5); // Backoff incremental
    }
    
    if (!clipboard || !clipboard->IsOpen()) return 0;

    // PASO 3: Verificar formato disponible
    if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
        return 0; // AutoClipboard handle resource release
    }

    // PASO 4: Extraer datos
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == NULL) { 
        return 0; 
    }

    RaiiWrappers::AutoGlobalLock lock(hData);
    LPCWSTR pszText = static_cast<LPCWSTR>(lock.Get());
    if (pszText == NULL) { 
        return 0; 
    }

    // PASO 5: Copiar a buffer propio
    std::wstring* pText = new std::wstring(pszText);

    // PASO 6: Entregar al hilo principal via PostMessage
    // El hilo principal es responsable de hacer delete de pText
    PostMessageW(hMainWnd, WM_APP_CLIPBOARD_TEXT, 0, reinterpret_cast<LPARAM>(pText));
    
    return 0;
}
