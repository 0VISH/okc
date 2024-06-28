#include "basic.hh"

#if(DBG)

#include "ray.h"
#include "procPointers.cc"

typedef void (*VOIDPROC)();
void (*clog)(const char*, ...);
#define EXPORT extern "C" __declspec(dllexport)

#define BIND_PROC(PROC_DST)						                                                   \
    if(x >= len){clog("Binding %s failed as x(%d) >= len(%d)", #PROC_DST, x, len); return false;}; \
    PROC_DST = reinterpret_cast<decltype(PROC_DST)>(procs[x]);		                               \
    x += 1;								                                                           \

EXPORT bool gameBind(VOIDPROC *procs, u32 len){
    u32 x = 0;
    BIND_PROC(clog);
    #include "procBind.cc"
#if(DBG)
    if(x < len){
        clog("Underbinding x(%d) < len(%d)", x, len);
	    return false;
    };
#endif
    return true;
};

#endif

#if(RLS)
#define EXPORT
#endif