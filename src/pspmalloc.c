#include <pspsdk.h>
#include <pspuser.h>
#include "allocation/pspmalloc.h"

void* psp_malloc(size_t size)
{
	SceUID uid = sceKernelAllocPartitionMemory(PSP_MEMORY_PARTITION_USER, "", PSP_SMEM_Low, size + 8, NULL);
	if (uid >= 0)
	{
		unsigned int* p = (unsigned int*)sceKernelGetBlockHeadAddr(uid);
		*p = uid;
		*(p + 1) = size;
		return (void*)(p + 2);
	}

	return NULL;
}

void psp_free(void* ptr)
{
	if (ptr)
	{
		sceKernelFreePartitionMemory(*(((SceUID*)ptr) - 2));
	}
}
