#include "pch.h"
#include "AACEncoder.h"

CAACEncoder::CAACEncoder() : m_streamIndex(0) {
    MFStartup(MF_VERSION, MFSTARTUP_FULL);
}

CAACEncoder::~CAACEncoder() {
    MFShutdown();
}

HRESULT CAACEncoder::Initialize(const std::wstring& /*outputFile*/) {
    // Stub implementation as WMF setup is out of scope for Phase 1
    return S_OK;
}

HRESULT CAACEncoder::Finalize() {
    // Stub implementation
    return S_OK;
}
