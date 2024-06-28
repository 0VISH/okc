typedef void (*VOIDPROC)();

#define V(proc) (VOIDPROC)proc

VOIDPROC procs[] = {
    V(clog),
    #include "procArr.cc"
};