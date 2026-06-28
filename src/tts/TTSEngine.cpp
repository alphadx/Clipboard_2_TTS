#include "pch.h"
#include "TTSEngine.h"
#include "VoiceEnumerator.h"

CTTSEngine::CTTSEngine(std::shared_ptr<CConfigManager> config, std::shared_ptr<CStateMachine> stateMachine) 
    : m_config(config), m_stateMachine(stateMachine), m_isPaused(false) {
}

CTTSEngine::~CTTSEngine() {
    if (m_spVoice) {
        m_spVoice.Release();
    }
}

HRESULT CTTSEngine::Initialize(HWND hMainWnd) {
    HRESULT hr = m_spVoice.CoCreateInstance(CLSID_SpVoice);
    if (FAILED(hr)) {
        OutputDebugStringW(L"Failed to CoCreateInstance ISpVoice\n");
        return hr;
    }
    
    // Set up events
    if (hMainWnd) {
        m_spVoice->SetInterest(SPFEI(SPEI_END_INPUT_STREAM), SPFEI(SPEI_END_INPUT_STREAM));
        m_spVoice->SetNotifyWindowMessage(hMainWnd, WM_APP_TTS_EVENT, 0, 0);
    }
    
    ApplyConfig();
    return S_OK;
}

void CTTSEngine::ApplyConfig() {
    if (!m_spVoice) return;
    
    STTSConfig config = m_config->GetTTSConfig();
    
    m_spVoice->SetVolume(config.volume);
    m_spVoice->SetRate(MapSpeedLevelToSAPIRate(config.speedLevel));
    
    if (!config.voiceTokenId.empty()) {
        SetVoiceFromToken(config.voiceTokenId);
    }
}

HRESULT CTTSEngine::SetVoiceFromToken(const std::wstring& tokenId) {
    CComPtr<IEnumSpObjectTokens> spEnum;
    HRESULT hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &spEnum);
    if (FAILED(hr)) return hr;

    CComPtr<ISpObjectToken> spToken;
    ULONG celtFetched = 0;
    while (spEnum->Next(1, &spToken, &celtFetched) == S_OK && celtFetched == 1) {
        LPWSTR pszTokenId = NULL;
        if (SUCCEEDED(spToken->GetId(&pszTokenId))) {
            std::wstring currentToken(pszTokenId);
            CoTaskMemFree(pszTokenId);
            
            if (currentToken == tokenId) {
                return m_spVoice->SetVoice(spToken);
            }
        }
        spToken.Release();
    }
    return E_FAIL;
}

HRESULT CTTSEngine::SpeakText(const std::wstring& text) {
    if (!m_spVoice) return E_FAIL;
    
    // Store in history
    {
        std::lock_guard<std::mutex> lock(m_historyMutex);
        SHistoryEntry entry;
        entry.text = text;
        GetLocalTime(&entry.timestamp);
        entry.voiceId = m_config->GetTTSConfig().voiceTokenId;
        m_history.push_back(entry);
        if (m_history.size() > 50) { // Keep last 50
            m_history.erase(m_history.begin());
        }
    }
    
    SPVOICESTATUS status;
    m_spVoice->GetStatus(&status, NULL);

    // If currently speaking, queue it if there is space
    if (status.dwRunningState == SPRS_IS_SPEAKING || m_isPaused) {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        if (m_textQueue.size() < static_cast<size_t>(m_config->GetTTSConfig().maxQueueSize)) {
            m_textQueue.push(text);
        }
        return S_OK; // Return OK even if discarded because queue is full
    }
    
    m_stateMachine->SetState(EAppState::Speaking);
    
    // Asynchronous speech
    DWORD flags = SPF_ASYNC | SPF_IS_NOT_XML | SPF_PURGEBEFORESPEAK;
    HRESULT hr = m_spVoice->Speak(text.c_str(), flags, NULL);
    
    return hr;
}

HRESULT CTTSEngine::StopSpeaking() {
    if (!m_spVoice) return E_FAIL;
    
    HRESULT hr = m_spVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
    m_stateMachine->SetState(EAppState::Listening);
    m_isPaused = false;
    return hr;
}

HRESULT CTTSEngine::RepeatLast() {
    std::wstring lastText;
    {
        std::lock_guard<std::mutex> lock(m_historyMutex);
        if (m_history.empty()) return S_FALSE;
        lastText = m_history.back().text;
    }
    return SpeakText(lastText);
}

HRESULT CTTSEngine::SkipCurrent() {
    if (!m_spVoice) return E_FAIL;
    // Purge stops the current speech, which triggers OnSpeechEnd to play the next one
    return m_spVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
}

HRESULT CTTSEngine::StopAll() {
    if (!m_spVoice) return E_FAIL;
    
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        std::queue<std::wstring> empty;
        std::swap(m_textQueue, empty);
    }
    
    m_isPaused = false;
    m_spVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
    m_stateMachine->SetState(EAppState::Listening);
    return S_OK;
}

HRESULT CTTSEngine::TogglePauseResume() {
    if (!m_spVoice) return E_FAIL;

    if (m_isPaused) {
        m_spVoice->Resume();
        m_isPaused = false;
    } else {
        SPVOICESTATUS status;
        m_spVoice->GetStatus(&status, NULL);
        if (status.dwRunningState == SPRS_IS_SPEAKING) {
            m_spVoice->Pause();
            m_isPaused = true;
        }
    }
    return S_OK;
}

void CTTSEngine::OnSpeechEnd() {
    std::wstring nextText;
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        if (!m_textQueue.empty()) {
            nextText = m_textQueue.front();
            m_textQueue.pop();
        }
    }

    if (!nextText.empty()) {
        m_stateMachine->SetState(EAppState::Speaking);
        DWORD flags = SPF_ASYNC | SPF_IS_NOT_XML | SPF_PURGEBEFORESPEAK;
        m_spVoice->Speak(nextText.c_str(), flags, NULL);
        if (m_isPaused) {
            m_spVoice->Resume();
            m_isPaused = false;
        }
    } else {
        m_stateMachine->SetState(EAppState::Listening);
        m_isPaused = false;
    }
}
