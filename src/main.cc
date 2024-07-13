#include "basic.hh"
#include "log.cc"
#include "platform.hh"
#include "raylib.h"
#include <stdlib.h>

#define GAME_MEM_SIZE 100000

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
    void *gameMem = malloc(GAME_MEM_SIZE);
    if(!code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs), gameMem, GAME_MEM_SIZE)) goto CLEANUP;
    auto gameUpdate = (VOIDPROC)code::getProc(gameCode, "gameUpdate");
    if(!gameUpdate){
        clog("gameUpdate is null");
        goto CLEANUP;
    };
    InitWindow(1800, 900, "okc");
    auto gameInit = (void(*)(void*, u64))code::getProc(gameCode, "gameInit");
    if(gameInit == nullptr) clog("gameInit is null");
    else gameInit(gameMem, GAME_MEM_SIZE);
    while(!WindowShouldClose()){
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)){
            gameCode = code::reload(GAME_CODE, gameCode);
            code::bindGameCode(gameCode, procs, ARRAY_LENGTH(procs), gameMem, GAME_MEM_SIZE);
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
        else gameUninit();
        code::unload(gameCode);
    };
    free(gameMem);
};

#endif

#if(RLS)

//TARGET GAME GOES HERE
#include "../sandbox/game.cc"

s32 main(){
    log::logFile = openFile(LOG_FILE);
    InitWindow(1800, 900, "sandbox");
    void *gameMem = malloc(GAME_MEM_SIZE);
    gameInit(gameMem, GAME_MEM_SIZE);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground({10, 10, 10, 255});
        gameUpdate();
        EndDrawing();
    };
    gameUninit();
    CloseWindow();
    free(gameMem);
    closeFile(log::logFile);
};
#endif
