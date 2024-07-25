#include "game.hh"

void gameReload(void *gameMem){
    clog("ye %p\n", gameMem);
};
EXPORT void gameInit(void *gameMem){
    clog("Hello from sandbox\n");
};
EXPORT void gameUpdate(f32 dt){
    BeginDrawing();
    DrawRectangleLines( 10, 10, 250, 113, BLUE);
    DrawText("ok computer", 10, 10, 100, DARKBROWN);
    EndDrawing();
};
EXPORT void gameUninit(){
    clog("uninit\n");
};
EXPORT void gamePhyUpdate(){
};
