#include "../code.cc"

namespace code{
    HMODULE cpyAndLoadTemp(char *filePath){
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
        return LoadLibraryA(buff);
    };
    bool bindGameCode(HMODULE code){
        auto gameProcBind = (bool(*)(VOIDPROC*, u32))GetProcAddress(code, "gameBind");
        return gameProcBind(procs, ARRAY_LENGTH(procs));
    };
    HMODULE reload(char *filePath, HMODULE code){
        FreeLibrary(code);
        code = cpyAndLoadTemp(filePath);
        if(!bindGameCode(code)) return NULL;
        return code;
    };
};