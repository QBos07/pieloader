#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reent.h>
//#include "src/hhk.h"

#define FRAMEBUFFER ((unsigned char *)0x8C000000)
#define LCD_Refresh ((void (*)(void))0x8003733E)

void (*const Debug_WaitKey)(void) = (void (*)(void))0x80094380;

extern void __sinit (struct _reent *);

void cas_setup();
void cas_cleanup();

void main() {
  /*_REENT_INIT_PTR(_REENT);
  __sinit(_REENT);
  cas_setup();
  printf("hi\n\n");
  fflush(stdout);
  Debug_WaitKey();
  cas_cleanup();*/
  memset(FRAMEBUFFER, 0x42694269, 0x1337);
  LCD_Refresh();
  Debug_WaitKey();
}
