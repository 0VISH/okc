#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.hh"
#include "basic.hh"

#if(DBG)

#include "ray.h"
#include "procPointers.cc"

typedef void (*VOIDPROC)();
void  (*clog)(const char*, ...);
void* (*alloc)(u64);
void  (*afree)(void*);
#define EXPORT extern "C" __declspec(dllexport)

#define BIND_PROC(PROC_DST)			                                                   \
    if(x >= len){clog("Binding %s failed as x(%d) >= len(%d)", #PROC_DST, x, len); return false;}; \
    PROC_DST = reinterpret_cast<decltype(PROC_DST)>(procs[x]);		                           \
    x += 1;							                                   \

void gameReload(void *);
EXPORT bool gameBind(VOIDPROC *procs, u32 len, void *gameMem){
    u32 x = 0;
    BIND_PROC(clog);
    BIND_PROC(alloc);
    BIND_PROC(afree);
    #include "procBind.cc"
#if(DBG)
    if(x < len){
        clog("Underbinding x(%d) < len(%d)", x, len);
	    return false;
    };
#endif
    gameReload(gameMem);
    return true;
};

#endif

#if(RLS)
#define EXPORT
#endif

#include "package.cc"
