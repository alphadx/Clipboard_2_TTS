#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <atlbase.h>    // CComPtr
#include <sapi.h>
#pragma warning(push)
#pragma warning(disable: 4996)
#include <sphelper.h>
#pragma warning(pop)
#include <shellapi.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlobj.h>

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
