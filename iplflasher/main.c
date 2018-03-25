#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>



PSP_MODULE_INFO("IplFlasher", 0x1000, 1, 0);

PSP_MAIN_THREAD_ATTR(0);

#define printf	pspDebugScreenPrintf


void ErrorExit(int milisecs, char *fmt, ...)
{
	va_list list;
	char msg[256];	

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	printf(msg);

	sceKernelDelayThread(milisecs*1000);
	sceKernelExitGame();
}

u32 FindProc(const char* szMod, const char* szLib, u32 nid)
{
	struct SceLibraryEntryTable *entry;
	SceModule *pMod;
	void *entTab;
	int entLen;

	pMod = sceKernelFindModuleByName(szMod);

	if (!pMod)
	{
		printf("Cannot find module %s\n", szMod);
		return 0;
	}
	
	int i = 0;

	entTab = pMod->ent_top;
	entLen = pMod->ent_size;
	//***printf("entTab %p - entLen %d\n", entTab, entLen);
	while(i < entLen)
    {
		int count;
		int total;
		unsigned int *vars;

		entry = (struct SceLibraryEntryTable *) (entTab + i);

        if(entry->libname && !strcmp(entry->libname, szLib))
		{
			total = entry->stubcount + entry->vstubcount;
			vars = entry->entrytable;

			if(entry->stubcount > 0)
			{
				for(count = 0; count < entry->stubcount; count++)
				{
					if (vars[count] == nid)
						return vars[count+total];					
				}
			}
		}

		i += (entry->len * 4);
	}

	printf("Funtion not found.\n");
	return 0;
}


u8 ipl[1048576] __attribute__((aligned(64)));

int ReadFile(char *file, void *buf, int size)
{
	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);

	if (fd < 0)
	{
		return fd;
	}

	int read = sceIoRead(fd, buf, size);
	sceIoClose(fd);

	return read;
}

int (* sceIplUpdateClearIpl)();
int (* sceIplUpdateSetIpl)(void *buf, int size);

int main()
{
	pspSdkInstallNoDeviceCheckPatch();
	pspSdkInstallNoPlainModuleCheckPatch();
	pspDebugScreenInit();

	int size = ReadFile("ipl.bin", ipl, sizeof(ipl));

	if (size <= 0)
	{
		ErrorExit(6000, "Error reading ipl file.\n");
	}

	if (pspSdkLoadStartModule("ipl_update.prx", PSP_MEMORY_PARTITION_KERNEL) < 0)
	{
		ErrorExit(6000, "Error loading module.\n");
	}

	sceIplUpdateClearIpl = (void *)FindProc("IplUpdater", "sceIplUpdate_driver", 0x26093B04);
	sceIplUpdateSetIpl = (void *)FindProc("IplUpdater", "sceIplUpdate_driver", 0xEE7EB563);

	if (sceIplUpdateClearIpl() < 0)
	{
		ErrorExit(6000, "Error clearing ipl.\n");
	}

	if (sceIplUpdateSetIpl(ipl, size) < 0)
	{
		ErrorExit(6000, "Error seting ipl.\n");
	}

	ErrorExit(7000, "\nFinished.\n");

	return 0;
}
