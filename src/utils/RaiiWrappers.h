#pragma once

namespace RaiiWrappers {

/**
 * RAII Wrapper for GlobalLock / GlobalUnlock
 */
class AutoGlobalLock {
public:
    explicit AutoGlobalLock(HGLOBAL hMem) : m_hMem(hMem), m_pData(nullptr) {
        if (m_hMem) {
            m_pData = GlobalLock(m_hMem);
        }
    }
    
    ~AutoGlobalLock() {
        if (m_pData) {
            GlobalUnlock(m_hMem);
        }
    }
    
    LPVOID Get() const { return m_pData; }
    
    // Prohibit copy and assignment
    AutoGlobalLock(const AutoGlobalLock&) = delete;
    AutoGlobalLock& operator=(const AutoGlobalLock&) = delete;

private:
    HGLOBAL m_hMem;
    LPVOID m_pData;
};

/**
 * RAII Wrapper for OpenClipboard / CloseClipboard
 */
class AutoClipboard {
public:
    explicit AutoClipboard(HWND hWnd) : m_isOpen(false) {
        m_isOpen = (OpenClipboard(hWnd) != FALSE);
    }
    
    ~AutoClipboard() {
        if (m_isOpen) {
            CloseClipboard();
        }
    }
    
    bool IsOpen() const { return m_isOpen; }
    
    // Prohibit copy and assignment
    AutoClipboard(const AutoClipboard&) = delete;
    AutoClipboard& operator=(const AutoClipboard&) = delete;

private:
    bool m_isOpen;
};

/**
 * RAII Wrapper for COM Initialization
 */
class AutoComInitialize {
public:
    AutoComInitialize() : m_hr(E_FAIL) {
        m_hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    }
    
    ~AutoComInitialize() {
        if (SUCCEEDED(m_hr)) {
            CoUninitialize();
        }
    }
    
    HRESULT GetResult() const { return m_hr; }
    
    // Prohibit copy and assignment
    AutoComInitialize(const AutoComInitialize&) = delete;
    AutoComInitialize& operator=(const AutoComInitialize&) = delete;

private:
    HRESULT m_hr;
};

} // namespace RaiiWrappers
