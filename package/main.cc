#include <windows.h> // NOTE: we can currently build packages on win32 only
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../src/basic.hh"

#define HEADER_END_MAGIC_NUMBER 0x6969

struct File{
    u64   size;
    char *name;
    char *mem;
};

struct PackageBuilder{
    std::vector<File> files;

    void addFile(char *fileName){
        FILE *f = fopen(fileName, "rb");
        if(!f){
            printf("[-]: Could not open %s\n", fileName);
            return;
        };
        File file;
        file.name = fileName;
        fseek(f, 0, SEEK_END);
	file.size = ftell(f);
	fseek(f, 0, SEEK_SET);
        file.mem = (char*)malloc(file.size);
        fread(file.mem, file.size, 1, f);
        fclose(f);
        files.push_back(file);
        printf("[+]: %s\n", fileName);
    };
    void addDirectory(char *directory){
        char searchPath[MAX_PATH];
        WIN32_FIND_DATA findFileData;
        HANDLE hFind;

        snprintf(searchPath, MAX_PATH, "%s\\*.*", directory);
        hFind = FindFirstFile(searchPath, &findFileData);

        if(hFind == INVALID_HANDLE_VALUE) {
            printf("[-]: Could not open directory %s\n", directory);
            return;
        };

        do {
            const char *fileOrDirName = findFileData.cFileName;
            if (strcmp(fileOrDirName, ".") != 0 && strcmp(fileOrDirName, "..") != 0) {
                char fullPath[MAX_PATH];
                snprintf(fullPath, MAX_PATH, "%s\\%s", directory, fileOrDirName);

                if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){addDirectory(fullPath);}
                else{addFile(fullPath);};
            };
        } while(FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    };
    void buildPackage(char *pkgName){
        FILE *pkg = fopen(pkgName, "wb");
        u64 offset = 0;
        for(const File &f : files){
            //HEADER
            u32 len = strlen(f.name);
            fwrite(&len, sizeof(len), 1, pkg);
            fwrite(f.name, len, 1, pkg);
            fwrite(&f.size, sizeof(f.size), 1, pkg);
            fwrite(&offset, sizeof(offset), 1, pkg);
            offset += f.size;
        };
        const u32 header_end_magic_number = HEADER_END_MAGIC_NUMBER;
        fwrite(&header_end_magic_number, sizeof(header_end_magic_number), 1, pkg);
        for(const File &f : files){
            //BODY
            fwrite(f.mem, f.size, 1, pkg);
            free(f.mem);
        };
        fclose(pkg);
        printf("[+]: Dumped %s package\n", pkgName);
    };
};

#define READ(T) *(T*)off; off += sizeof(T);

void infoPackage(char *pkgName){
    FILE *pkg = fopen(pkgName, "rb");
    if(!pkg){
        printf("[-]: Package %s does not exist\n", pkgName);
        return;
    };
    fseek(pkg, 0, SEEK_END);
    u64 size = ftell(pkg);
    fseek(pkg, 0, SEEK_SET);
    char *pkgMem = (char*)malloc(size);
    fread(pkgMem, size, 1, pkg);
    fclose(pkg);

    const char *dashes = "--------------------";
    printf("%s%s%s\n", dashes, pkgName, dashes);
    char *off = pkgMem;
    while(true){
        u32 magicNum = *(u32*)off;
        if(magicNum == HEADER_END_MAGIC_NUMBER) break;
        u32 len = READ(u32);
        printf("%.*s(%d) -> ", len, off, len);
        off += len;
        u64 size = READ(u64);
        u64 offset = READ(u64);
        printf("[size(%lld), rel_off(%lld)]\n", size, offset);
    };
    printf("%s[END]%s\n", dashes, dashes);
    free(pkgMem);
};

s32 main(){
    PackageBuilder pb;
    pb.addDirectory("assets");
    pb.buildPackage("assets.pkg");
    infoPackage("assets.pkg");
};
