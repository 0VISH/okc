#include "game.hh"

void gameReload(void *gameMem, u64 memSize){
    clog("ye %p\n", gameMem);
    mem::reload(gameMem, memSize);
};
EXPORT void gameInit(void *gameMem, u64 memSize){
    clog("Hello from sandbox\n");
    mem::init(gameMem, memSize); 
};
EXPORT void gameUpdate(f32 dt){
    DrawRectangleLines( 10, 10, 250, 113, BLUE);
    DrawText("ok computer", 10, 10, 100, DARKBROWN);
};
EXPORT void gameUninit(){
    clog("uninit\n");
};
EXPORT void gamePhyUpdate(){
    clog("phy update\n");
};
