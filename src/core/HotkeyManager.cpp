#include "pch.h"
#include "HotkeyManager.h"

CHotkeyManager::CHotkeyManager(HWND hMainWnd, std::shared_ptr<CConfigManager> config)
    : m_hMainWnd(hMainWnd), m_config(config), m_atomSkip(0), m_atomStopAll(0), m_atomPauseResume(0) {
    m_atomSkip = GlobalAddAtomW(L"ClipboardTTS_Skip");
    m_atomStopAll = GlobalAddAtomW(L"ClipboardTTS_StopAll");
    m_atomPauseResume = GlobalAddAtomW(L"ClipboardTTS_PauseResume");
}

CHotkeyManager::~CHotkeyManager() {
    UnregisterAllHotkeys();
    if (m_atomSkip) GlobalDeleteAtom(m_atomSkip);
    if (m_atomStopAll) GlobalDeleteAtom(m_atomStopAll);
    if (m_atomPauseResume) GlobalDeleteAtom(m_atomPauseResume);
}

void CHotkeyManager::RegisterAllHotkeys() {
    SHotkeyConfig skip = m_config->GetSkipHotkey();
    RegisterHotKey(m_hMainWnd, m_atomSkip, skip.fsModifiers, skip.vkCode);

    SHotkeyConfig stopAll = m_config->GetStopAllHotkey();
    RegisterHotKey(m_hMainWnd, m_atomStopAll, stopAll.fsModifiers, stopAll.vkCode);

    SHotkeyConfig pauseResume = m_config->GetPauseResumeHotkey();
    RegisterHotKey(m_hMainWnd, m_atomPauseResume, pauseResume.fsModifiers, pauseResume.vkCode);
}

void CHotkeyManager::UnregisterAllHotkeys() {
    UnregisterHotKey(m_hMainWnd, m_atomSkip);
    UnregisterHotKey(m_hMainWnd, m_atomStopAll);
    UnregisterHotKey(m_hMainWnd, m_atomPauseResume);
}

void CHotkeyManager::ReconfigureHotkeys() {
    UnregisterAllHotkeys();
    RegisterAllHotkeys();
}
