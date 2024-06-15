#include <stdint.h>

#define LCD_Refresh ((void (*)(void))0x8003733E)

uint16_t *LCD_GetVRAMAddress();
void Debug_WaitKey();
void LCD_GetSize(int *width, int *height);
void LCD_VRAMBackup();
void LCD_VRAMRestore();

int width, height;
uint16_t* vram;

int main() {
	vram = LCD_GetVRAMAddress();
	LCD_GetSize(&width, &height);
	LCD_VRAMBackup();
	for(uint16_t i=0; i < 1337; i++)
		vram[i]=i;
	LCD_Refresh();
	Debug_WaitKey();
	LCD_VRAMRestore();
	LCD_Refresh();
	Debug_WaitKey();
	return 0;
}
