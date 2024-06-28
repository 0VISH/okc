#include <windows.h>
#include "../basic.hh"
#include "log.cc"
#include "../include.hh"
#include "code.cc"

#if(DBG)

#define GAME_CODE "bin/win/dbg/game.dll"

u32 main(){
    HMODULE gameCode = code::cpyAndLoadTemp(GAME_CODE);
    if(!gameCode) return EXIT_FAILURE;
    if(!code::bindGameCode(gameCode)) goto CLEANUP;
    auto gameUpdate = (VOIDPROC)GetProcAddress(gameCode, "gameUpdate");
    if(!gameUpdate){
        clog("gameUpdate is null")
        goto CLEANUP;
    };
    ray::InitWindow(1800, 900, "okc");
    auto gameInit = (VOIDPROC)GetProcAddress(gameCode, "gameInit");
    if(gameInit == nullptr) clog("gameInit is null");
    else gameInit();
    while(!ray::WindowShouldClose()){
        if(ray::IsKeyDown(ray::KEY_LEFT_CONTROL) && ray::IsKeyPressed(ray::KEY_R)) gameCode = code::reload(GAME_CODE, gameCode);
        ray::BeginDrawing();
        ray::ClearBackground({10, 10, 10, 255});
        gameUpdate();
        ray::EndDrawing();
    };
    ray::CloseWindow();
CLEANUP:
    if(gameCode){
        auto gameUninit = (VOIDPROC)GetProcAddress(gameCode, "gameUninit");
        if(gameUninit == nullptr) clog("gameUninit is null");
        FreeLibrary(gameCode);
    };
};

#endif