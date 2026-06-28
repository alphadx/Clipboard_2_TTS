#pragma once
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <windows.h>
#include <atlbase.h>
#include <sapi.h>
#include <cmath>
#include "config/ConfigManager.h"
#include "core/StateMachine.h"

// Forward declaration of AudioLogger
class CAudioLogger;

struct SHistoryEntry {
    std::wstring text;
    SYSTEMTIME timestamp;
    std::wstring voiceId;
};
constexpr UINT WM_APP_TTS_EVENT = WM_APP + 2;

class CTTSEngine {
public:
    CTTSEngine(std::shared_ptr<CConfigManager> config, std::shared_ptr<CStateMachine> stateMachine);
    ~CTTSEngine();

    HRESULT Initialize(HWND hMainWnd);
    HRESULT SpeakText(const std::wstring& text);
    HRESULT StopSpeaking();
    HRESULT RepeatLast();

    HRESULT SkipCurrent();
    HRESULT StopAll();
    HRESULT TogglePauseResume();
    void OnSpeechEnd();
    
    void ApplyConfig();

    inline INT MapSpeedLevelToSAPIRate(INT level) const {
        if (level < 1) level = 1;
        if (level > 20) level = 20;
        return static_cast<INT>(std::round((level - 1) * 20.0 / 19.0 - 10.0));
    }

private:
    HRESULT SetVoiceFromToken(const std::wstring& tokenId);

    std::shared_ptr<CConfigManager> m_config;
    std::shared_ptr<CStateMachine> m_stateMachine;
    
    CComPtr<ISpVoice> m_spVoice;
    
    std::vector<SHistoryEntry> m_history;
    std::mutex m_historyMutex;

    std::queue<std::wstring> m_textQueue;
    std::mutex m_queueMutex;
    bool m_isPaused;
};
