#ifndef GAME_H
#define GAME_H

#include <gb/gb.h>

#define NBDFRAMES 0x18                  /* Nb frames for the door */
#define NBSFRAMES 0x07                  /* Nb frames for the sprite */
#define WINSZX    0x80                  /* Size of the picture in the window */
#define WINSZY    0x50
#define MINWINX   (MAXWNDPOSX-WINSZX+1) /* Bounds of the window origin */
#define MINWINY   (MAXWNDPOSY-WINSZY+1)
#define MAXWINX   MAXWNDPOSX
#define MAXWINY   MAXWNDPOSY
#define FADESTEP  0x10                  /* Nb steps for the fading effect */
#define STARTFADE (0x06*FADESTEP)       /* Initial value for the fading effect */

#define CLOSED  0x00
#define OPENING 0x01
#define OPENED  0x02
#define CLOSING 0x03

void fade();
void scroll();
void door();
void animate_sprite();
void tile_sprite();
void place_sprite();

#endif
