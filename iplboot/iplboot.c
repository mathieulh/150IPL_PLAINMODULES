#include <pspsdk.h>

void (* Dcache)(void) = (void *)0x800102d8;
void (* Icache)(void) = (void *)0x800102a0;

int (* sceNandRead)(u32 ppn, void *buf, void *extra) = (void *)0x80010334;


void entry(void)
{
	u16 *fat_ptr = (u16*)0x8001081c;
	u32 page;
	u8  *output;
	int i;

	page = (fat_ptr[0]*0x20)+8; // First page of the rest of our code
	output = (u8 *)0x040e0000;
	
	for (i = 0; i < 24; i++)
	{
		sceNandRead(page, output, (void *)0x80010810);
		output += 0x200;
		page++;
	}
	
	// Nullify decryption
	*(u32 *)(0x800100BC) = 0x00001021; // mov v0, zero
	// Patch checksum check
	*(u32*)(0x800100D4) =  0; 
	// Bypass FAT Read
	*(u32*)(0x80010154) = 0x00001021; // mov v0, zero

	// Move the blocks one position left for new pre-ipl execution	
	do
	{
		fat_ptr[0] = fat_ptr[1];
		fat_ptr++;
	} while(((u32)fat_ptr) < 0x8001091a);

	Dcache();
	Icache();	

	// return to PRE-IPL
}
