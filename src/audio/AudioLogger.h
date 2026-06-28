#pragma once
#include <windows.h>
#include <string>
#include <memory>
#include "AACEncoder.h"
#include "config/ConfigManager.h"

class CAudioLogger {
public:
    CAudioLogger(std::shared_ptr<CConfigManager> config);
    ~CAudioLogger() = default;

    HRESULT StartRecording();
    HRESULT StopRecording();

private:
    std::shared_ptr<CConfigManager> m_config;
    std::unique_ptr<CAACEncoder> m_encoder;
};
