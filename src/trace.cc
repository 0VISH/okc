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
#define TRACE_FILE "/sdcard/runtime.trc"
#else
#define TRACE_FILE "runtime.trc"
#endif

namespace trace{
    void *traceFile;
};
void clog(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    vfprintf((FILE*)trace::traceFile, fmt, args);
    va_end(args);
};
#endif
