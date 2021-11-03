#include <stdlib.h>
#include <string>
#include <cmath>
#include "rng.h"

#if defined(_WIN32)
#define LOG(...)                 \
{                                \
    char cad[512];               \
    sprintf_s(cad, __VA_ARGS__); \
    OutputDebugString(cad);      \
}
#else
#define LOG
#endif
