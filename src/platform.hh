#pragma once

typedef void (*VOIDPROC)();

void clog(const char *fmt, ...);

namespace code{
    void *cpyAndLoadTemp(char *filePath);
    bool bindGameCode(void *code, VOIDPROC *procs, u32 len);
    void *reload(char *filePath, void *code);
    void *getProc(void *code, char *name);
    void unload(void *code);
};
void *openFile(char *name);
void closeFile(void *file);