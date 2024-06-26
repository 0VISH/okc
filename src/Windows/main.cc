#include <stdio.h>
#include "../basic.hh"
namespace ray{
    #include "raylib.h"
};

u32 main(){
    printf("hello, world! okc");
    ray::InitWindow(800, 800, "okc");
};