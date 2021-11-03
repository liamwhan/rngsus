#if !defined(APP_H)
#define VERSION "3.2.4"

#include <stdint.h>
#if !defined(APP_IMGUI)
#include "imgui.h"
#define APP_IMGUI
#endif

#define internal static
#define local_persist static
#define global_variable static

#if defined(_WIN32)
#define BASE_DPI 96.f
#else
#define BASE_DPI 72.f
#endif

#if RNG_SLOW
#if defined(_WIN32)
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;} // Trying to write to 0 will cause the program to crash at the line the assert is on
#elif defined(__APPLE__)
#include <Foundation/Foundation.h>
#define Assert(Expression) NSCAssert(Expression)
#endif
#else
#define Assert(Expression)
#endif



#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)


typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef float real32;
typedef double real64;

// NOTE(liam): using this instead of bool is to prevent compile-time type coercion
typedef int32 bool32;

typedef struct read_file_result 
{
    uint32 ContentsSize;
    void *Contents;

} read_file_result;

#define SHOW_NONE               0
#define SHOW_DEMO               1
#define EXPAND_NONE             0

typedef struct file_filter
{
    const wchar_t *Filter;
    const wchar_t *Description;
} file_filter;


typedef struct ui_state
{
    uint8 ShowFlags;
    uint8 ExpandFlags;
    
    float ClearColorF[4];
    ImVec4 ClearColor;
    ImVec2 ButtonSize;
    ImVec2 MainPanelSize;

    // OS specific handles
    void *WindowHandle;
    void *NSWindowHandle;
    void *Device;
    void *RenderPassDescriptor;
    void *CommandQueue;

    float DpiScale;
    
    
} ui_state;


// Cross platform function define macros
#define PLATFORM_READ_ENTIRE_FILE(name) read_file_result name(char *Filename)
typedef PLATFORM_READ_ENTIRE_FILE(platform_read_entire_file);

#define PLATFORM_WRITE_ENTIRE_FILE(name) bool32 name(char *Filename, uint32 MemorySize, void *Memory)
typedef PLATFORM_WRITE_ENTIRE_FILE(platform_write_entire_file);

#define PLATFORM_FREE(name) void name(void *Memory)
typedef PLATFORM_FREE(platform_free);

#define PLATFORM_ALLOC(name) void* name(uint32 Bytes)
typedef PLATFORM_ALLOC(platform_alloc);

#define PLATFORM_CREATE_WINDOW(name) void* name()
typedef PLATFORM_CREATE_WINDOW(platform_create_window);

#define PLATFORM_IMGUI_INIT_BACKEND(name) void name(void* WindowHandle)
typedef PLATFORM_IMGUI_INIT_BACKEND(platform_imgui_init_backend);

#define PLATFORM_IMGUI_START_FRAME(name) void name()
typedef PLATFORM_IMGUI_START_FRAME(platform_imgui_start_frame);

#define PLATFORM_IMGUI_RENDER(name) void name(ui_state *State)
typedef PLATFORM_IMGUI_RENDER(platform_imgui_render);

#define PLATFORM_IMGUI_SHUTDOWN(name) void name()
typedef PLATFORM_IMGUI_SHUTDOWN(platform_imgui_shutdown);

#define PLATFORM_SHOW_FILE_OPEN_DIALOG(name) char* name(ui_state *State, file_filter *Filter)
typedef PLATFORM_SHOW_FILE_OPEN_DIALOG(platform_show_file_open_dialog);

#define PLATFORM_SHOW_FILE_SAVE_DIALOG(name) char* name(ui_state *State, file_filter *Filter, wchar_t *SaveExtension)
typedef PLATFORM_SHOW_FILE_SAVE_DIALOG(platform_show_file_save_dialog);

#define PLATFORM_OPEN_FILE_MANAGER(name) void name(char *Filepath, ui_state *State)
typedef PLATFORM_OPEN_FILE_MANAGER(platform_open_file_manager);

inline uint32 SafeTruncateUInt64(uint64 Value)
{
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32)Value;
    return(Result);
}


// external Forward Declarations

// These just create externed forward declarations for functions
extern PLATFORM_READ_ENTIRE_FILE(PlatformReadEntireFile);
extern PLATFORM_WRITE_ENTIRE_FILE(PlatformWriteEntireFile);
extern PLATFORM_ALLOC(PlatformAlloc);
extern PLATFORM_FREE(PlatformFree);
extern PLATFORM_CREATE_WINDOW(PlatformCreateWindow);
extern PLATFORM_IMGUI_INIT_BACKEND(PlatformImguiInitBackend);
extern PLATFORM_IMGUI_START_FRAME(PlatformImguiStartFrame);
extern PLATFORM_IMGUI_RENDER(PlatformImguiRender);
extern PLATFORM_SHOW_FILE_OPEN_DIALOG(PlatformShowFileOpenDialog);
extern PLATFORM_SHOW_FILE_SAVE_DIALOG(PlatformShowFileSaveDialog);
extern PLATFORM_OPEN_FILE_MANAGER(PlatformOpenFileManager);
extern PLATFORM_IMGUI_SHUTDOWN(PlatformImguiShutdown);


#define APP_H
#endif
