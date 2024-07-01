#include "../platform.hh"
#include <stdio.h>

#if(RLS)
void *openFile(char *name){return fopen(name, "w");};
void closeFile(void *file){fclose((FILE*)file);};
#endif
