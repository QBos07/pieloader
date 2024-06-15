//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#define FRAMEBUFFER ((volatile unsigned char *)0x8C000000)
#define LCD_Refresh ((void (*)(void))0x8003733E)

void (*const Debug_WaitKey)(void) = (void (*)(void))0x80094380;

unsigned int _exit_address = 0xEDEDBEEF;
volatile int x = 0;

void memset(void *where, int was, unsigned int length) {
  for(int i = 0; i<length/sizeof(int); i++)
    ((int *)where)[i] = was;
}

void setX(int val) {
  x = val;
}

void printX() {
  memset(FRAMEBUFFER, x, 0x1337 + 0x420);
}

void main() {
  setX(42);
  printX();
  LCD_Refresh();
  Debug_WaitKey();
}
