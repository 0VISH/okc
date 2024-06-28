#include <stdarg.h>

#include "basic.hh"
#include "platform.hh"
#include "raylib.h"

#if(DBG)

#define V(proc) (VOIDPROC)proc

VOIDPROC procs[] = {
    V(clog),
    #include "procArr.cc"
};

#define GAME_CODE "bin/win/dbg/game.dll"

u32 main(){
    void *gameCode = code::cpyAndLoadTemp(GAME_CODE);
    if(!gameCode) return 1;
    if(!code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs))) goto CLEANUP;
    auto gameUpdate = (VOIDPROC)code::getProc(gameCode, "gameUpdate");
    if(!gameUpdate){
        clog("gameUpdate is null");
        goto CLEANUP;
    };
    InitWindow(1800, 900, "okc");
    auto gameInit = (VOIDPROC)code::getProc(gameCode, "gameInit");
    if(gameInit == nullptr) clog("gameInit is null");
    else gameInit();
    while(!WindowShouldClose()){
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)){
            gameCode = code::reload(GAME_CODE, gameCode);
            code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs));
        };
        BeginDrawing();
        ClearBackground({10, 10, 10, 255});
        gameUpdate();
        EndDrawing();
    };
    CloseWindow();
CLEANUP:
    if(gameCode){
        auto gameUninit = (VOIDPROC)code::getProc(gameCode, "gameUninit");
        if(gameUninit == nullptr) clog("gameUninit is null");
        code::unload(gameCode);
    };
};

#endif

#if(RLS)

//TARGET GAME GOES HERE
#include "../sandbox/game.cc"

namespace log{
    extern void *logFile;
};

u32 main(){
    log::logFile = openFile("runtime.log");
    InitWindow(1800, 900, "sandbox");
    gameInit();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground({10, 10, 10, 255});
        gameUpdate();
        EndDrawing();
    };
    gameUninit();
    CloseWindow();
    closeFile(log::logFile);
};
#endif