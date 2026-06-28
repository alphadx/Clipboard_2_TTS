#include "pch.h"
#include "StateMachine.h"

CStateMachine::CStateMachine(EAppState initialState) : m_state(initialState) {
}

EAppState CStateMachine::GetState() const {
    return m_state.load();
}

void CStateMachine::SetState(EAppState newState) {
    m_state.store(newState);
}

bool CStateMachine::TransitionTo(EAppState newState) {
    // Basic transition validation can be done here if needed.
    // For now we just set the new state.
    m_state.store(newState);
    return true;
}
