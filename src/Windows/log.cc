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
namespace log{
    static FILE *logFile;
};
void clog(const char *fmt, ...){
    va_list args;
    va_start(fmt, args);
    vfprintf(logFile, fmt, args);
    va_end(args);
};
#endif