#pragma once
#include <windows.h>
#include <memory>
#include "config/ConfigManager.h"

class CHotkeyManager {
public:
    CHotkeyManager(HWND hMainWnd, std::shared_ptr<CConfigManager> config);
    ~CHotkeyManager();

    void RegisterAllHotkeys();
    void UnregisterAllHotkeys();
    void ReconfigureHotkeys();

    ATOM GetSkipAtomId() const { return m_atomSkip; }
    ATOM GetStopAllAtomId() const { return m_atomStopAll; }
    ATOM GetPauseResumeAtomId() const { return m_atomPauseResume; }

private:
    HWND m_hMainWnd;
    std::shared_ptr<CConfigManager> m_config;

    ATOM m_atomSkip;
    ATOM m_atomStopAll;
    ATOM m_atomPauseResume;
};
