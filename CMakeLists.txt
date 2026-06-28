cmake_minimum_required(VERSION 3.20)
project(ClipboardTTSMonitor VERSION 1.0.0 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure UNICODE
add_compile_definitions(UNICODE _UNICODE)

# MSVC specific options
if(MSVC)
    # W4 and treat warnings as errors
    add_compile_options(/W4 /WX)
endif()

set(SOURCES
    src/main/main.cpp
    src/main/Application.cpp
    src/main/Application.h
    
    src/ui/TrayIcon.cpp
    src/ui/TrayIcon.h
    src/ui/ContextMenu.cpp
    src/ui/ContextMenu.h
    
    src/core/HotkeyManager.cpp
    src/core/HotkeyManager.h
    src/core/ClipboardListener.cpp
    src/core/ClipboardListener.h
    src/core/StateMachine.cpp
    src/core/StateMachine.h
    
    src/tts/TTSEngine.cpp
    src/tts/TTSEngine.h
    src/tts/VoiceEnumerator.cpp
    src/tts/VoiceEnumerator.h
    
    src/audio/AudioLogger.cpp
    src/audio/AudioLogger.h
    src/audio/AACEncoder.cpp
    src/audio/AACEncoder.h
    
    src/config/ConfigManager.cpp
    src/config/ConfigManager.h
    
    src/utils/StringUtils.cpp
    src/utils/StringUtils.h
    src/utils/RaiiWrappers.h
)

set(RESOURCES
    res/resources.rc
)

add_executable(ClipboardTTSMonitor WIN32 ${SOURCES} ${RESOURCES})

# Precompiled Headers
target_precompile_headers(ClipboardTTSMonitor PRIVATE include/pch.h)

# Include directories
target_include_directories(ClipboardTTSMonitor PRIVATE
    src
    include
)

# Link against required Windows libraries
target_link_libraries(ClipboardTTSMonitor PRIVATE
    sapi.lib
    shell32.lib
    user32.lib
    mfreadwrite.lib
    mfuuid.lib
    mfplat.lib
    advapi32.lib
    ole32.lib
    oleaut32.lib
)

# Install rules for CPack
install(TARGETS ClipboardTTSMonitor DESTINATION bin)

# CPack configuration for WiX
set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_NAME "ClipboardTTSMonitor")
set(CPACK_PACKAGE_VENDOR "ClipboardTTS")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Clipboard Text-to-Speech Monitor")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_WIX_UPGRADE_GUID "98c6b758-038c-42cc-a42e-131e5f3f4a7c") # Generated GUID for stable upgrades

include(CPack)
