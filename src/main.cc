#include "basic.hh"
#include "trace.cc"
#include "platform.hh"
#include "raylib.h"
#include <stdlib.h>
#include "Game/config.hh"
//NOTE: create a file named "gamePath.hh" and define GAME_PATH
#include "gamePath.hh"

#if(DBG)

#define V(proc) (VOIDPROC)proc

VOIDPROC procs[] = {
    V(clog),
    V(malloc),
    V(free),
    #include "procArr.cc"
};

#define GAME_CODE "bin/win/dbg/game.dll"

u32 main(){
    void *gameCode = code::cpyAndLoadTemp(GAME_CODE);
    if(!gameCode) return 1;
    void *gameMem = malloc(GAME_MEM_SIZE);
    if(!code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs), gameMem)) goto CLEANUP;
    auto gameUpdate = (void(*)(f32))code::getProc(gameCode, "gameUpdate");
    if(!gameUpdate){
        clog("gameUpdate is null\n");
        goto CLEANUP;
    };
    auto gamePhyUpdate = (VOIDPROC)code::getProc(gameCode, "gamePhyUpdate");
    if(!gamePhyUpdate){
        clog("gamePhyUpdate is null\n");
        goto CLEANUP;
    };
    InitWindow(1800, 900, "okc");
    auto gameInit = (void(*)(void*))code::getProc(gameCode, "gameInit");
    if(gameInit == nullptr) clog("gameInit is null\n");
    else gameInit(gameMem);
    f32 phyDelta = 0;
    const f32 phyTimeStamp = (f32)1/PHY_HERTZ;
    while(!WindowShouldClose()){
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)){
            gameCode = code::reload(GAME_CODE, gameCode);
            code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs), gameMem);
        };
        f32 dt = GetFrameTime();
        phyDelta += dt;
        while(phyDelta >= phyTimeStamp){
            gamePhyUpdate();
            phyDelta -= phyTimeStamp;
        };
        gameUpdate(dt);
    };
    CloseWindow();
CLEANUP:
    if(gameCode){
        auto gameUninit = (VOIDPROC)code::getProc(gameCode, "gameUninit");
        if(gameUninit == nullptr) clog("gameUninit is null\n");
        else gameUninit();
        code::unload(gameCode);
    };
    free(gameMem);
};

#endif

#if(RLS)
#define alloc malloc
#define afree free
#include GAME_PATH

s32 main(){
    trace::traceFile = openFile(TRACE_FILE);
    InitWindow(1800, 900, "sandbox");
    void *gameMem = malloc(GAME_MEM_SIZE);
    gameInit(gameMem);
    f32 phyDelta = 0;
    const f32 phyTimeStamp = (f32)1/PHY_HERTZ;
    while(!WindowShouldClose()){
        f32 dt = GetFrameTime();
        phyDelta += dt;
        while(phyDelta >= phyTimeStamp){
            gamePhyUpdate();
            phyDelta -= phyTimeStamp;
        };
        gameUpdate(dt);
    };
    gameUninit();
    CloseWindow();
    free(gameMem);
    closeFile(trace::traceFile);
};
#endif
