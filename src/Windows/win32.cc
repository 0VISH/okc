#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../basic.hh"
#include "../platform.hh"

namespace code{
    void *cpyAndLoadTemp(char *filePath){
        char buff[1024];
        const u32 len  = (u32)strlen(filePath) - 4; //.dll
        char *tempDll  = "_temp.dll";
        memcpy(buff, filePath, len);
        memcpy(buff+len, tempDll, (u32)strlen(tempDll)+1);

        BOOL b = CopyFile(filePath, buff, false);
        if(!b){
            clog("Could not copy game code %s. Maybe %s does not exist?", filePath, filePath);
            return false;
        };
        return (void*)LoadLibraryA(buff);
    };
    bool bindGameCode(void* code, VOIDPROC *procs, u32 len, void *gameMem, u64 memSize){
        auto gameProcBind = (bool(*)(VOIDPROC*, u32, void*))GetProcAddress((HMODULE)code, "gameBind");
        return gameProcBind(procs, len, gameMem);
    };
    void* reload(char *filePath, void* code){
        FreeLibrary((HMODULE)code);
        return cpyAndLoadTemp(filePath);
    };
    void *getProc(void *code, char *name){return GetProcAddress((HMODULE)code, name);};
    void unload(void *code){FreeLibrary((HMODULE)code);};
};

void *openFile(char *name){return fopen(name, "w");};
void closeFile(void *file){fclose((FILE*)file);};
