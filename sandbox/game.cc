#include "game.hh"

EXPORT void gameInit(){
    clog("Hello from sandbox\n");
};
EXPORT void gameUpdate(){
    DrawRectangleLines( 10, 10, 250, 113, BLUE);
    DrawText("ok computer", 10, 10, 100, DARKBROWN);
};
EXPORT void gameUninit(){
    clog("uninit\n");
};