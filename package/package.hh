#pragma once

#define PKG_END_MAGIC_NUMBER 0x6969
#define READ(T) *(T*)off; off += sizeof(T);
