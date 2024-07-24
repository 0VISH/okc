#include <windows.h> // NOTE: we can currently build packages on win32 only
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../src/basic.hh"
#include "package.hh"

struct File{
    u64   size;
    char  name[100];
    char *mem;
};

struct PackageBuilder{
    std::vector<File> files;
    u32 fileNameLen = 0;

    void addFile(char *fileName){
        FILE *f = fopen(fileName, "rb");
        if(!f){
            printf("[-]: Could not open %s\n", fileName);
            return;
        };
        File file;
        u32 strLen = strlen(fileName) + 1;
        fileNameLen += strLen;
        memcpy(file.name, fileName, strLen);
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
        fwrite(&fileNameLen, sizeof(fileNameLen), 1, pkg);
        u32 fileCount = files.size();
        fwrite(&fileCount, sizeof(fileCount), 1, pkg);
        for(const File &f : files){
            //HEADER
            u32 len = strlen(f.name);
            fwrite(&len, sizeof(len), 1, pkg);
            fwrite(f.name, len, 1, pkg);
            fwrite(&f.size, sizeof(f.size), 1, pkg);
            fwrite(f.mem, f.size, 1, pkg);
            free(f.mem);
        };
        const u32 pkg_end_magic_number = PKG_END_MAGIC_NUMBER;
        fwrite(&pkg_end_magic_number, sizeof(pkg_end_magic_number), 1, pkg);
        fclose(pkg);
        printf("[+]: Dumped %s package\n", pkgName);
    };
};

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
    READ(u32);
    u32 fileCount = READ(u32);
    printf("file_count: %d\n", fileCount);
    while(true){
        u32 magicNum = *(u32*)off;
        if(magicNum == PKG_END_MAGIC_NUMBER) break;
        u32 len = READ(u32);
        printf("%.*s(%d) -> ", len, off, len);
        off += len;
        u64 size = READ(u64);
        printf("size(%lld)\n", size);
        off += size;
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
