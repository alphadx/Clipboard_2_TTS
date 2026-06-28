#include "pch.h"
#include "main/Application.h"
#include "utils/RaiiWrappers.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    RaiiWrappers::AutoComInitialize comInit;
    if (FAILED(comInit.GetResult())) {
        return -1;
    }

    CApplication app(hInstance);
    return app.Run();
}
