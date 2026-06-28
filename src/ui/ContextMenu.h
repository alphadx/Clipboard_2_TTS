#pragma once
#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include "config/ConfigManager.h"
#include "tts/TTSEngine.h"
#include "audio/AudioLogger.h"

constexpr WORD CMD_EXIT = 1000;
constexpr WORD CMD_CONFIG_HOTKEYS = 1001;
constexpr WORD CMD_TOGGLE_RECORDING = 1002;
constexpr WORD CMD_VOICE_BASE = 2000;
constexpr WORD CMD_SPEED_BASE = 3000;
constexpr WORD CMD_QUEUE_BASE = 4000;

class CContextMenu {
public:
    CContextMenu(HWND hMainWnd, std::shared_ptr<CConfigManager> config, 
                 std::shared_ptr<CTTSEngine> tts, std::shared_ptr<CAudioLogger> audio);
    ~CContextMenu();

    void Show(int x, int y);
    void HandleCommand(WORD wId);

private:
    void BuildMenu();

    HWND m_hMainWnd;
    std::shared_ptr<CConfigManager> m_config;
    std::shared_ptr<CTTSEngine> m_tts;
    std::shared_ptr<CAudioLogger> m_audio;
    HMENU m_hMenu;
    
    std::vector<std::pair<std::wstring, std::wstring>> m_availableVoices;
};
