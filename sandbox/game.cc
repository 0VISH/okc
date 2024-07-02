#include "game.hh"

void gameReload(void *gameMem, u64 memSize){
    clog("ye %p\n", gameMem);
    mem::reload(gameMem, memSize);
};
EXPORT void gameInit(void *gameMem, u64 memSize){
    clog("Hello from sandbox\n");
    mem::init(gameMem, memSize); 
};
EXPORT void gameUpdate(){
    DrawRectangleLines( 10, 10, 250, 113, BLUE);
    DrawText("ok computer", 10, 10, 100, DARKBROWN);
};
EXPORT void gameUninit(){
    clog("uninit\n");
};
