#include "game.hh"
#include "editor.c"
#include <stdlib.h>
mu_Context *ctx;

EXPORT void gameInit(){
    clog("Hello from sandbox\n");
    ctx = (mu_Context*)malloc(sizeof(mu_Context));
    mu_init(ctx);
    murl_setup_font(ctx);
};
EXPORT void gameUpdate(){
    DrawRectangleLines( 10, 10, 250, 113, BLUE);
    DrawText("ok computer", 10, 10, 100, DARKBROWN);
    murl_handle_input(ctx);

    mu_begin(ctx);
    if (mu_begin_window(ctx, "Hello", mu_rect(20, 20, 200, 150))) {
      mu_label(ctx, "Hello, raylib");

      if (mu_button(ctx, "The button")) {
        mu_open_popup(ctx, "popup");
      }

      if (mu_begin_popup(ctx, "popup")) {
        mu_label(ctx, "This is a popup");
        mu_end_popup(ctx);
      }

      mu_end_window(ctx);
    }

    mu_end(ctx);
    murl_render(ctx);
};
EXPORT void gameUninit(){
    clog("uninit\n");
    free(ctx);
};
