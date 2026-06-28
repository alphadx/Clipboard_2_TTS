#pragma once
#include <atomic>
#include <windows.h>

enum class EAppState : UINT {
    Inactive = 0, // Hook desactivado, hotkeys activos
    Listening = 1, // Monitoreando portapapeles
    Speaking = 2, // Reproduciendo TTS
    Suspended = 3, // Reconfiguracion temporal
};

class CStateMachine {
public:
    CStateMachine(EAppState initialState = EAppState::Inactive);
    ~CStateMachine() = default;

    EAppState GetState() const;
    void SetState(EAppState newState);

    bool TransitionTo(EAppState newState);

private:
    std::atomic<EAppState> m_state;
};
