#include <windows.h>
#include "../basic.hh"
#include "log.cc"
#include "code.cc"
#include "../include.hh"

#if(DBG)

u32 main(){
    HMODULE gameCode = code::cpyAndLoadTemp("bin/win/dbg/game.dll");
    if(!gameCode) return EXIT_FAILURE;
    if(!code::bindGameCode(gameCode)) goto CLEAUP;
    ray::InitWindow(1800, 900, "okc");
    auto gameInit = (VOIDPROC)GetProcAddress(gameCode, "gameInit");
    if(gameInit == NULL) clog("gameUninit is null");
    else gameInit();
    while(!ray::WindowShouldClose()){
        ray::BeginDrawing();
        ray::ClearBackground({10, 10, 10, 255});
        ray::EndDrawing();
    };
    ray::CloseWindow();
CLEAUP:
    if(gameCode){
        auto gameUninit = (VOIDPROC)GetProcAddress(gameCode, "gameUninit");
        if(gameInit == NULL) clog("gameUninit is null");
        FreeLibrary(gameCode);
    };
};

#endif