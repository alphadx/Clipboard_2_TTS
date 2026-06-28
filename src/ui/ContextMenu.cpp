#include "pch.h"
#include "ContextMenu.h"
#include "tts/VoiceEnumerator.h"

CContextMenu::CContextMenu(HWND hMainWnd, std::shared_ptr<CConfigManager> config, 
                           std::shared_ptr<CTTSEngine> tts, std::shared_ptr<CAudioLogger> audio)
    : m_hMainWnd(hMainWnd), m_config(config), m_tts(tts), m_audio(audio), m_hMenu(NULL) {
    CVoiceEnumerator::EnumerateInstalledVoices(m_availableVoices);
    BuildMenu();
}

CContextMenu::~CContextMenu() {
    if (m_hMenu) {
        DestroyMenu(m_hMenu);
    }
}

void CContextMenu::BuildMenu() {
    if (m_hMenu) DestroyMenu(m_hMenu);
    m_hMenu = CreatePopupMenu();

    // Voices Submenu
    HMENU hVoicesMenu = CreatePopupMenu();
    for (size_t i = 0; i < m_availableVoices.size(); ++i) {
        UINT flags = MF_STRING;
        if (m_config->GetTTSConfig().voiceTokenId == m_availableVoices[i].first) {
            flags |= MF_CHECKED;
        }
        AppendMenuW(hVoicesMenu, flags, CMD_VOICE_BASE + i, m_availableVoices[i].second.c_str());
    }
    AppendMenuW(m_hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hVoicesMenu), L"Voces");

    // Speed Submenu
    HMENU hSpeedMenu = CreatePopupMenu();
    INT currentSpeed = m_config->GetTTSConfig().speedLevel;
    for (int i = 1; i <= 20; ++i) {
        UINT flags = MF_STRING;
        if (i == currentSpeed) flags |= MF_CHECKED;
        std::wstring speedStr = L"Nivel " + std::to_wstring(i);
        AppendMenuW(hSpeedMenu, flags, CMD_SPEED_BASE + i, speedStr.c_str());
    }
    AppendMenuW(m_hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hSpeedMenu), L"Velocidad");

    // Queue Size Submenu
    HMENU hQueueMenu = CreatePopupMenu();
    INT currentQueueSize = m_config->GetTTSConfig().maxQueueSize;
    for (int i = 1; i <= 10; ++i) {
        UINT flags = MF_STRING;
        if (i == currentQueueSize) flags |= MF_CHECKED;
        std::wstring queueStr = std::to_wstring(i) + L" elemento" + (i > 1 ? L"s" : L"");
        AppendMenuW(hQueueMenu, flags, CMD_QUEUE_BASE + i, queueStr.c_str());
    }
    AppendMenuW(m_hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hQueueMenu), L"Tamaño de Cola");

    AppendMenuW(m_hMenu, MF_SEPARATOR, 0, NULL);
    
    // Recording toggle
    UINT recFlags = MF_STRING | (m_config->GetAudioConfig().recordingEnabled ? MF_CHECKED : MF_UNCHECKED);
    AppendMenuW(m_hMenu, recFlags, CMD_TOGGLE_RECORDING, L"Grabacion [ON/OFF]");
    
    AppendMenuW(m_hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(m_hMenu, MF_STRING, CMD_EXIT, L"Salir");
}

void CContextMenu::Show(int x, int y) {
    BuildMenu(); // Rebuild to update checkmarks
    SetForegroundWindow(m_hMainWnd);
    int cmd = TrackPopupMenu(m_hMenu, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTALIGN, x, y, 0, m_hMainWnd, NULL);
    if (cmd != 0) {
        HandleCommand(static_cast<WORD>(cmd));
    }
}

void CContextMenu::HandleCommand(WORD wId) {
    if (wId == CMD_EXIT) {
        PostQuitMessage(0);
    } else if (wId == CMD_TOGGLE_RECORDING) {
        SAudioConfig audioCfg = m_config->GetAudioConfig();
        audioCfg.recordingEnabled = !audioCfg.recordingEnabled;
        m_config->SetAudioConfig(audioCfg);
        m_config->SaveConfig();
    } else if (wId >= CMD_VOICE_BASE && wId < CMD_VOICE_BASE + m_availableVoices.size()) {
        int index = wId - CMD_VOICE_BASE;
        STTSConfig ttsCfg = m_config->GetTTSConfig();
        ttsCfg.voiceTokenId = m_availableVoices[index].first;
        m_config->SetTTSConfig(ttsCfg);
        m_config->SaveConfig();
        m_tts->ApplyConfig();
    } else if (wId > CMD_SPEED_BASE && wId <= CMD_SPEED_BASE + 20) {
        int speed = wId - CMD_SPEED_BASE;
        STTSConfig ttsCfg = m_config->GetTTSConfig();
        ttsCfg.speedLevel = speed;
        m_config->SetTTSConfig(ttsCfg);
        m_config->SaveConfig();
        m_tts->ApplyConfig();
    } else if (wId > CMD_QUEUE_BASE && wId <= CMD_QUEUE_BASE + 10) {
        int queueSize = wId - CMD_QUEUE_BASE;
        STTSConfig ttsCfg = m_config->GetTTSConfig();
        ttsCfg.maxQueueSize = queueSize;
        m_config->SetTTSConfig(ttsCfg);
        m_config->SaveConfig();
    }
}
