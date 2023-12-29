/**
 * @brief FujiLink, a terminal emulator for FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Dialing Directory routines
 */

#include <video/tms99x8.h>
#include <conio.h>
#include <eos.h>
#include <smartkeys.h>
#include "directory.h"
#include "rleunpack.h"
#include "globals.h"
#include "input.h"
#include "cursor.h"

#define TAB_X 3
#define TAB_Y 1

#define NUM_DIRECTORY_ENTRIES 31

Directory entries[31] =
  {
    {"LEVEL 29","N:TELNET://BBS.FOZZTEXX.COM/",DUMB},
  };

static unsigned char current_pos=0;

// RLE encoded pixel data to go to VDP address 0x0000
static const unsigned char directory_pixels[] = {
  0x8e, 0x00, 0x02, 0x03, 0x07, 0x85, 0x00, 0x03, 0x01, 0x03, 0x07, 0xe4,
  0x00, 0x04, 0xf0, 0x07, 0x03, 0x01, 0xff, 0x00, 0x02, 0x00, 0x07, 0x97,
  0xf0, 0xff, 0x00, 0xe9, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x01,
  0x80, 0x87, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88,
  0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88,
  0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98,
  0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff,
  0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9,
  0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88,
  0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88,
  0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98,
  0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff,
  0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9,
  0x00, 0x88, 0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88,
  0xc0, 0x88, 0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88,
  0x00, 0x98, 0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x98,
  0xf0, 0xff, 0x00, 0xd9, 0x00, 0x88, 0xc0, 0x88, 0x00, 0x96, 0xf0, 0x02,
  0x07, 0x03, 0xff, 0x00, 0xd9, 0x00, 0x87, 0xc0, 0x01, 0x80, 0x88, 0x00,
  0x04, 0xf0, 0xf0, 0x07, 0x03, 0x84, 0x00, 0x02, 0x07, 0x03, 0xff, 0x00,
  0xc6, 0x00, 0x01, 0x01, 0x85, 0x00, 0x01, 0x01, 0x85, 0x00, 0x01, 0x01,
  0x85, 0x00, 0x01, 0x01, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
  0xff, 0x00, 0xff, 0x00, 0xa4, 0x00
};

// RLE encoded color data to go to VDP address 0x2000
static const unsigned char directory_colors[] = {
  0x8e, 0x17, 0x02, 0xe7, 0xe7, 0x85, 0x17, 0x03, 0xf7, 0xfe, 0xfe, 0x84,
  0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84,
  0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84,
  0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84,
  0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84, 0x17, 0x84, 0x1f, 0x84,
  0x17, 0x01, 0xf7, 0x83, 0x7f, 0xff, 0x17, 0x03, 0x17, 0x17, 0x71, 0x86,
  0xe1, 0x88, 0x1e, 0x88, 0xef, 0xe8, 0x1f, 0xff, 0x17, 0x01, 0x17, 0x88,
  0xe1, 0x88, 0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88,
  0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88,
  0x1f, 0xe0, 0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88,
  0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88,
  0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0xe0,
  0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88,
  0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88,
  0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0xe0, 0x17, 0x88,
  0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0xff,
  0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88,
  0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0x88,
  0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9,
  0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88,
  0x1f, 0xe0, 0x17, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88,
  0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88,
  0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0,
  0x17, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88,
  0xe1, 0x88, 0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88,
  0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88,
  0x1f, 0xe0, 0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88,
  0x1e, 0x88, 0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88,
  0xe1, 0x88, 0x1e, 0x88, 0xef, 0x88, 0x1f, 0xe0, 0x17, 0x88, 0x1f, 0xe0,
  0x17, 0x88, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88, 0x1e, 0x88,
  0xef, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7, 0x88, 0x17, 0x88, 0xe1, 0x88,
  0x1e, 0x86, 0xef, 0x02, 0xfe, 0xfe, 0xff, 0x1f, 0xd9, 0x1f, 0x88, 0xf7,
  0x88, 0x17, 0x02, 0xe1, 0xe1, 0x84, 0x1e, 0x7f, 0x11, 0x11, 0xe1, 0xe1,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e,
  0x11, 0x11, 0x1e, 0x57, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x1e, 0x11, 0x11, 0x1e, 0x1e, 0x11, 0x71, 0x1e, 0x1e, 0x11, 0x11, 0x1e,
  0x7e, 0x17, 0x17, 0x1e, 0x1e, 0x11, 0x71, 0x84, 0x17, 0x02, 0x1e, 0x7e,
  0xff, 0x17, 0xff, 0x17, 0xff, 0x17, 0xff, 0x17, 0xff, 0x17, 0xff, 0x17,
  0xa4, 0x17
};

static void display_entry(char *s, unsigned char pos)
{
  int r = pos >> 1;
  int c = pos % 2;
  int y = r + 3;
  int x = c ? 17 : 4;
  int b = r % 2 ? VDP_INK_WHITE : VDP_INK_CYAN;
  
  vdp_color(VDP_INK_BLACK,b,VDP_INK_CYAN);
  gotoxy(x,y);
  
  cputs(s);
}

static void update_cursor(unsigned char pos)
{
  int r = pos >> 1;
  int c = pos % 2;
  int y = r + 3;
  int x = c ? 16 : 3;

  selector(1);
  selector_pos(x,y);
}

static void home(void)
{
  current_pos=0;
  update_cursor(current_pos);
}

static void up(void)
{
  if (current_pos)
    current_pos -= 2;

  if (current_pos<0)
    current_pos = 0;

  update_cursor(current_pos);
}

static void right(void)
{
  if (!(current_pos % 2))
    current_pos++;
  
  update_cursor(current_pos);
}

static void down(void)
{
  int p=current_pos+2;
  
  if (p<0)
    return;
  else if (p>29)
    return;

  current_pos = p;

  update_cursor(current_pos);
}

static void left(void)
{
  if ((current_pos % 2))
    current_pos--;

  update_cursor(current_pos);
}

void directory(void)
{
  // Graphics II mode
  vdp_set_mode(2);
  
  /* // Blank screen */
  vdp_noblank();

  // unpack the directory screen
  RLEUnpack(MODE2_ATTR,directory_colors,MODE2_MAX);
  RLEUnpack(0x0000,directory_pixels,MODE2_MAX);

  vdp_color(VDP_INK_BLACK,VDP_INK_WHITE,VDP_INK_CYAN);
  gotoxy(TAB_X,TAB_Y);
  cprintf(" DIRECTORY");

  // Populate directory entry display
  for (unsigned char i=0;i<NUM_DIRECTORY_ENTRIES;i++)
    {
      display_entry(entries[i].name,i);
    }

  // Smart keys display
  smartkeys_display(NULL,NULL,NULL,NULL,"  \x1F\x1F""EDIT\n ENTRY","  \x1F\x1F""DIAL\n OTHER");  
  smartkeys_status("   WELCOME TO FUJILINK I\n   SELECT ENTRY WITH ARROWS\n   [RETURN] TO DIAL, OR...");
  
  /* // show screen */
  vdp_blank();

  update_cursor(0);

  eos_start_read_keyboard();
  
  while (state==DIRECTORY)
    {
      switch(input())
	{
	case KEY_RETURN:
	  break;
	case KEY_HOME:
	  home();
	  break;
	case KEY_UP_ARROW:
	  up();
	  break;
	case KEY_DOWN_ARROW:
	  down();
	  break;
	case KEY_LEFT_ARROW:
	  left();
	  break;
	case KEY_RIGHT_ARROW:
	  right();
	  break;
	case KEY_SMART_V:
	  break;
	case KEY_SMART_VI:
	  break;
	}
    }

  // State changed, turn off keyboard scanning.
  eos_end_read_keyboard();
}