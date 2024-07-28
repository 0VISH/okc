#include "../../package/package.hh"

struct PackageManager{
    char *nameMem;
    char *nameMemOff;
    char **keys;
    char **values;
    u64  *sizes;
    u32   len;
    char *pkgMem;

    void uninit(){
#if(RLS)
	afree(pkgMem);
	afree(nameMem);
	afree(keys);
	afree(values);
	afree(sizes);
#endif
    };
    u32 hashFunc(const char *key, u32 len){
	//fnv_hash_1a_32
	u32 h = 0x811c9dc5;
	for(u32 i=0; i<len; i+=1){h = (h^key[i]) * 0x01000193;};
	return h;
    };
    void insert(char *name, u32 size, u64 fileSize, void *mem){
	memcpy(nameMemOff, name, size);
	name = nameMemOff;
	nameMemOff += size;
	u32 hash = hashFunc(name, size) % len;
	while(keys[hash]){
	    hash += 1;
	    if(hash >= len){hash = 0;};
	};
	keys[hash] = name;
	values[hash] = (char*)mem;
	sizes[hash] = fileSize;
    };
    void *getFile(char *name, s32 &size){
#if(DBG)
	return LoadFileData(name, (s32*)&size);
#endif
#if(RLS)
	u32 strLen = strlen(name);
	u32 startHash = hashFunc(name, strLen) % len;
	u32 hash = startHash;
	while(keys[hash]){
	    if(memcmp(name, keys[hash], strLen) == 0){
		size = sizes[hash];
		return values[hash];
	    };
	    hash += 1;
	    if(hash >= len){hash = 0;};
	    if(hash == startHash){return nullptr;};
	};
	return nullptr;
#endif
    };
    void init(char *pkgName){
#if(RLS)
	FILE *pkg = fopen(pkgName, "rb");
	if(!pkg){
	    clog("[-]: Package %s does not exist\n", pkgName);
	    return;
	};
	fseek(pkg, 0, SEEK_END);
	size = (s32)ftell(pkg);
	fseek(pkg, 0, SEEK_SET);
	pkgMem = (char*)alloc(size);
	fread(pkgMem, size, 1, pkg);
	fclose(pkg);
	char *off = pkgMem;
	u32 fileNameLen = READ(u32);
	u32 fileCount = READ(u32);
	len = fileCount;
	nameMem = (char*)alloc(fileNameLen);
	nameMemOff = nameMem;
	keys = (char**)alloc(sizeof(char*) * fileCount);
	values = (char**)alloc(sizeof(char*) * fileCount);
	sizes = (u64*)alloc(sizeof(u64) * fileCount);
	memset(keys, NULL, sizeof(char*) * fileCount);
	while(true){
	    u32 magicNum = *(u32*)off;
	    if(magicNum == PKG_END_MAGIC_NUMBER) break;
	    u32 lenStr = READ(u32);
	    char *str = off;
	    off += lenStr;
	    u64 fileSize = READ(u64);
	    clog("loaded %.*s with size %lld\n", lenStr, str, fileSize);
	    insert(str, lenStr, fileSize, off);
	    off += fileSize;
	};
#endif
    }; 
};
