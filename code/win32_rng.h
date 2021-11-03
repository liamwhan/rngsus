#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <PathCch.h>
#include <shellapi.h>
#include <shobjidl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stringapiset.h>
#include <tchar.h>
#include <WinBase.h>
#include <winuser.h>
#include <AudioClient.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>


#define LOG(...)                     \
    {                                \
        char cad[512];               \
        sprintf_s(cad, __VA_ARGS__); \
        OutputDebugString(cad);      \
    }

#include "rng.cpp"
#include "rng_gui.cpp"



#define TOCHAR(name) char* name(wchar_t *input)
TOCHAR(ToChar)
{

    if (!input)
        return ((char *)0);
    uint64 origsize = wcslen(input) + 1; // Add 1 to account for terminating NULL char
    uint64 newsize = origsize * 2;
    uint64 convertedChars = 0;

    char *Result = new char[newsize];
    wcstombs_s(&convertedChars, Result, newsize, input, _TRUNCATE);
    return Result;
}
#define TOWCHAR(name) wchar_t *name(char *input)
inline TOWCHAR(ToWideChar)
{
    if (!input)
        return (wchar_t *)0;

    uint32 RequiredSizeChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input, -1, NULL, 0);
    wchar_t *Result = (wchar_t *)PlatformAlloc(RequiredSizeChars * sizeof(wchar_t));

    if (MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            input,
            -1,
            Result,
            RequiredSizeChars))
    {
        return Result;
    }

    uint32 ErrorCode = GetLastError();
    switch (ErrorCode)
    {
    case ERROR_INSUFFICIENT_BUFFER:
        LOG("Insufficient Buffer Size");
        break;
    case ERROR_INVALID_FLAGS:
        LOG("Invalid Flags");
        break;
    case ERROR_INVALID_PARAMETER:
        LOG("Invalid Parameter");
        break;
    case ERROR_NO_UNICODE_TRANSLATION:
        LOG("Inputs string could not be converted to unicode");
        break;
    }
    return (wchar_t *)0;
}

std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
    {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

internal void OutputLastError()
{
    OutputDebugString(GetLastErrorAsString().c_str());
    OutputDebugString("\n");
}

inline void HandleHResult(HRESULT hr, char* error)
{
    if (FAILED(hr))
    {
        OutputLastError();
        Assert(!error);
    }
}

// Forward decs
LRESULT WINAPI
WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

internal char *
ShowFileOpenDialog(HWND hWnd);

internal char *
ShowFileSaveDialog(HWND hWnd);

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
