#pragma once
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <string>
#include <atlbase.h>

class CAACEncoder {
public:
    CAACEncoder();
    ~CAACEncoder();

    HRESULT Initialize(const std::wstring& outputFile);
    HRESULT Finalize();

private:
    CComPtr<IMFSinkWriter> m_pSinkWriter;
    DWORD m_streamIndex;
};
