#include <stdio.h>
#include <stdarg.h>

#if(DBG)
void clog(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
};
#endif

#if(RLS)
#if(ANDROID)
#include "Linux/linux.cc"
#define LOG_FILE "/sdcard/runtime.log"
#else
#define LOG_FILE "runtime.log"
#endif

namespace log{
    void *logFile;
};
void clog(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vfprintf((FILE*)log::logFile, fmt, args);
    va_end(args);
};
#endif
