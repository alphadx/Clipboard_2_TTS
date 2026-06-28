#include "pch.h"
#include "AudioLogger.h"
#include "utils/StringUtils.h"

CAudioLogger::CAudioLogger(std::shared_ptr<CConfigManager> config) : m_config(config) {
}

HRESULT CAudioLogger::StartRecording() {
    SAudioConfig config = m_config->GetAudioConfig();
    if (!config.recordingEnabled) {
        return S_FALSE; // Not enabled
    }

    std::wstring filename = config.outputDirectory + L"\\" + StringUtils::FormatIso8601() + L"_TTS.m4a";
    
    m_encoder = std::make_unique<CAACEncoder>();
    return m_encoder->Initialize(filename);
}

HRESULT CAudioLogger::StopRecording() {
    if (m_encoder) {
        HRESULT hr = m_encoder->Finalize();
        m_encoder.reset();
        return hr;
    }
    return S_FALSE;
}
