namespace allocator{
    const u64 CHUNK_SIZE = sizeof(u64)*2;
    u64 CHUNK_COUNT;

    u8 getStat(u8 *stat, u32 off){
        u32 x = (u32)floor((f32)off/(f32)sizeof(u8));
        u8 byte = stat[x];
        off = off - (x*sizeof(u8));
        return IS_BIT(byte, off);
    };
    void setStat(u8 *stat, u32 off, u32 end){
        u32 x = (u32)floor((f32)off/(f32)sizeof(u8));
        u8 &byte = stat[x];
        u32 o = off - (x*sizeof(u8));
        while(off != end){
            SET_BIT(byte, o);
            off += 1;
            o = (o + 1) % 8;
        };
    };
    void *alloc(u64 size, char *memory, u8 *stat){
#if(DBG)
	if(size == 0){
	    clog("\n[MEM]: trying to allocate memory of size 0\n");
	    return nullptr;
	};
#endif
	/*
	  Each allocation remembers the amount of blocks it asked for
	  This decreases cache miss
	*/
	size += sizeof(u32);
	u32 chunkReq = ceil(size/((f64)(CHUNK_SIZE)));
	u32 i = 0;
    MEM_FIND_CHUNKS:
	u32 startOff = i;
	u32 chunkFound = 0;
	while(getStat(stat, i) == 0){
	    chunkFound += 1;
	    if(chunkFound == chunkReq){
		char *ptr = memory+(startOff*CHUNK_SIZE);
		u32 *intPtr = (u32*)ptr;
		*intPtr = chunkReq;
		ptr += sizeof(u32);
                setStat(stat, startOff, chunkFound);
		return(void*)ptr;
	    };
	    i += 1;
	};
	if(i+chunkReq >= CHUNK_COUNT){
#if(DBG)
	    clog("\n[MEM]: out of chunks. Please increase memory size or reduce chunk_size\n");
#endif
	    return nullptr;
	};
	while(getStat(stat, i)){i += 1;};
	goto MEM_FIND_CHUNKS;
	return nullptr;
    };
    void free(void *ptr, char *memory, u8 *stat){
	char *cptr = (char*)ptr;
	cptr -= sizeof(u32);
	u32 off = (cptr - memory)/CHUNK_SIZE;
	u32 chunks = *((u32*)cptr);
        setStat(stat, off, chunks);
#if(DBG)
	memset(cptr, 'A', CHUNK_SIZE*chunks);
#endif
    };
};
namespace mem{
    char *memory;
    u8   *stat;
#if(DBG)
    u32 allocCount;
#endif
    
    void reload(void *mem, u64 memSize){
	/*
	    8x = y
	    x + CHUNK_SIZE*y = memSize
	*/
	const u32 x = floor((f64)memSize/(f64)(sizeof(u8) + 8*allocator::CHUNK_SIZE));
	const u32 y = 8 * x;
        allocator::CHUNK_COUNT = x;
        const u32 statSize = sizeof(u8)*y;
	stat = (u8*)mem;
        memory = (char*)mem + statSize;
    };
    void init(void *mem, u64 memSize){
	reload(mem, memSize);
	allocCount = 0;
        memset(stat, 0, memory - (char*)stat);
#if(DBG)
	memset(memory, 'A', allocator::CHUNK_SIZE * allocator::CHUNK_COUNT);
#endif
    };
    void *alloc(u64 size){
#if(DBG)
	allocCount += 1;
#endif
	return allocator::alloc(size, memory, stat);
    };
    void *calloc(u64 size){
	void *ptr = alloc(size);
	memset(ptr, 0, size);
	return ptr;
    };
    void free(void *ptr){
#if(DBG)
	if(allocCount == 0){
	    clog("[MEM]: allocCount is 0. Trying to free another pointer\n");
	};
	allocCount -= 1;
#endif
	return allocator::free(ptr, memory, stat);
    };
};
