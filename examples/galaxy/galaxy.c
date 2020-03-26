/*
 * C version of the 'space' assembly demo.
 *
 * Little demo illustrating how to use the graphical possibilities
 *  of the GB (background, window and animated sprite)
 * I have used fixed-point values for both the position and
 *  speed of objects to get smooth movements
 *
 * OBJ data             : 0x8000 -> 0x8FFF (unsigned)
 * Window data          : 0x8800 -> 0x97FF (unsigned)
 * Background data      : 0x8800 -> 0x97FF (signed)
 *
 * Tiled 0xFC -> 0xFF are standard tiles (all black -> all white)
 *
 * Keys:
 * Arrow keys           : Change the speed (and direction) of the sprite
 * Arrow keys + A       : Change the speed (and direction) of the window
 * Arrow keys + B       : Change the speed (and direction) of the background
 * START                : Open/close the door
 * SELECT               : Basic fading effect
 *
 * Note that the window is kept in the lower right part of the screen
 * since it can't be made transparent
 */

#include "galaxy.h"
#include "assets.h"

static UBYTE time;  /* Global "time" value (counter) */
UBYTE doorstate;    /* State of the door (OPENED, CLOSED...) */
UBYTE doorpos;      /* Current position in the door animation */
static UBYTE color; /* Current color for fading effect */
UBYTE sframe;       /* Current frame of the sprite */
fixed bposx, bposy; /* Background position (fixed point) */
fixed bspx, bspy;   /* Background speed (fixed point) */
fixed wposx, wposy; /* Window position (fixed point) */
fixed wspx, wspy;   /* Window speed (fixed point) */
fixed sposx, sposy; /* Sprite position (fixed point) */
fixed sspx, sspy;   /* Sprite speed (fixed point) */

/* Should really be const, but sdcc doesnt yet support it. */
const unsigned char *const film[] = {
    &door1_tiles[0x0C * 0],
    &door2_tiles[0x0C * 0],
    &door3_tiles[0x0C * 0],
    &door4_tiles[0x0C * 0],
    &door1_tiles[0x0C * 1],
    &door2_tiles[0x0C * 1],
    &door3_tiles[0x0C * 1],
    &door4_tiles[0x0C * 1],
    &door1_tiles[0x0C * 2],
    &door2_tiles[0x0C * 2],
    &door3_tiles[0x0C * 2],
    &door4_tiles[0x0C * 2],
    &door1_tiles[0x0C * 3],
    &door2_tiles[0x0C * 3],
    &door3_tiles[0x0C * 3],
    &door4_tiles[0x0C * 3],
    &door1_tiles[0x0C * 4],
    &door2_tiles[0x0C * 4],
    &door3_tiles[0x0C * 4],
    &door4_tiles[0x0C * 4],
    &door1_tiles[0x0C * 5],
    &door2_tiles[0x0C * 5],
    &door3_tiles[0x0C * 5],
    &door4_tiles[0x0C * 5],
    &door1_tiles[0x0C * 6]
};

/* Fade the screen (off and on) */
void fade()
{
    if (color == 0) {
        return;
    }
    switch (color) {
        case STARTFADE:
        case STARTFADE-4*FADESTEP:
            BGP_REG = 0xF9U;
            break;
        case STARTFADE-FADESTEP:
        case STARTFADE-3*FADESTEP:
            BGP_REG = 0xFEU;
            break;
        case STARTFADE-2*FADESTEP:
            BGP_REG = 0xFFU;
            break;
        case STARTFADE-5*FADESTEP:
            BGP_REG = 0xE4U;
            break;
    }
    color--;
}

/* Scroll the background, the window and the sprite */
void scroll()
{
    /* Update background */
    bposx.w += bspx.w;
    bposy.w += bspy.w;
    SCX_REG = bposx.b.h;
    SCY_REG = bposy.b.h;

    /* Update window */
    wposx.w += wspx.w ;
    wposy.w += wspy.w ;
    /* X position */
    if (wposx.b.h >= MAXWINX) {
        wposx.b.h = MAXWINX;
        /* Invert speed */
        wspx.w = -(WORD)wspx.w;
    } else if (wposx.b.h <= MINWINX) {
        wposx.b.h = MINWINX;
        /* Invert speed */
        wspx.w = -(WORD)wspx.w;
    }
    WX_REG = wposx.b.h;
    /* Y position */
    if (wposy.b.h >= MAXWINY) {
        wposy.b.h = MAXWINY;
        /* Invert speed */
        wspy.w = -(WORD)wspy.w;
    } else if (wposy.b.h <= MINWINY) {
        wposy.b.h = MINWINY;
        /* Invert speed */
        wspy.w = -(WORD)wspy.w;
    }
    WY_REG = wposy.b.h;

    /* Update sprite */
    sposx.w += sspx.w;
    sposy.w += sspy.w;
    place_sprite();
}

/* Open and close the door */
void door()
{
    if (doorstate == OPENING) {
        doorpos++;
        /* Draw the door in the window */
        set_win_tiles(2, 2, 12, 6, film[doorpos]);
        if (doorpos == NBDFRAMES) {
            doorstate = OPENED;
        }
    } else if (doorstate == CLOSING) {
        doorpos--;
        /* Draw the door in the window */
        set_win_tiles(2, 2, 12, 6, film[doorpos]);
        if (doorpos == 0) {
            doorstate = CLOSED;
        }
    }
}

/* Animate sprite */
void animate_sprite()
{
    if ((time & 0x07) == 0) {
        sframe++;
        if (sframe == NBSFRAMES) {
            sframe = 0;
        }
        tile_sprite();
    }
}

/* Set sprite tiles */
void tile_sprite()
{
    UBYTE s;

    s = sframe << 1;
    set_sprite_tile(0, earth_tiles[s]);
    set_sprite_tile(1, earth_tiles[s + 1]);
}

/* Place sprite */
void place_sprite()
{
    move_sprite(0, sposx.b.h, sposy.b.h);
    move_sprite(1, sposx.b.h + 8, sposy.b.h);
}

void main()
{
    UBYTE i, j;

    disable_interrupts();
    DISPLAY_OFF;
    LCDC_REG = 0x67;
    /*
     * LCD        = Off
     * WindowBank = 0x9C00
     * Window     = On
     * BG Chr     = 0x8800
     * BG Bank    = 0x9800
     * OBJ        = 8x16
     * OBJ        = On
     * BG         = On
     */

    doorstate = CLOSED;

    /* Set palettes */
    BGP_REG = OBP0_REG = OBP1_REG = 0xE4U;

    /* Initialize the background */
    set_bkg_data(0xFC, 0x04, std_data);
    set_bkg_data(0x00, 0x2D, bkg_data);
    /*
     * Draw the background
     *
     * Width  = 0x100 = 0x20 * 8
     * Height = 0x100 = 0x20 * 8
     */
    for (i = 0; i < 32; i += 8)
        for (j = 0; j < 32; j += 8) {
            set_bkg_tiles(i, j, 8, 8, bkg_tiles);
        }
    bposx.w = 0;
    SCX_REG = 0;
    bposy.w = 0;
    SCY_REG = 0;
    bspx.w = 0xFF00;
    bspy.w = 0x0080;

    /* Initialize the window */
    set_win_data(0x80, 0x21, frame_data);
    /*
     * Draw the frame in the window
     *
     * Width  = 0x80 = 0x10 * 8
     * Height = 0x50 = 0x0A * 8
     */
    set_win_tiles(0, 0, 16, 10, frame_tiles);
    /*
     * Draw the door in the window
     *
     * Width  = 0x60 = 0x20 * 12
     * Height = 0x30 = 0x20 * 6
     */
    set_win_tiles(2, 2, 12, 6, door1_tiles);
    wposx.b.h = MAXWNDPOSX;
    wposx.b.l = 0;
    WX_REG = MAXWNDPOSX;
    wposy.b.h = MAXWNDPOSY;
    wposy.b.l = 0;
    WY_REG = MAXWNDPOSY;
    wspx.w = 0xFF80;
    wspy.w = 0xFFC0;

    /* Initialize the sprite */
    set_sprite_data(0x00, 0x1C, earth_data);
    set_sprite_prop(0, 0x00);
    set_sprite_prop(1, 0x00);
    sframe = 0;
    sposx.w  = 0x1000;
    sposy.w  = 0x1000;
    sspx.w  = 0x0040;
    sspy.w  = 0x0040;
    tile_sprite();
    place_sprite();

    DISPLAY_ON;
    enable_interrupts();

    while (1) {
        /* Skip four VBLs (slow down animation) */
        for (i = 0; i < 4; i++) {
            wait_vbl_done();
        }
        time++;
        fade();
        door();
        scroll();
        animate_sprite();
        i = joypad();
        if (i & J_B) {
            if (i & J_UP) {
                bspy.w -= 0x0010;
            }
            if (i & J_DOWN) {
                bspy.w += 0x0010;
            }
            if (i & J_LEFT) {
                bspx.w -= 0x0010;
            }
            if (i & J_RIGHT) {
                bspx.w += 0x0010;
            }
        } else if (i & J_A) {
            if (i & J_UP) {
                wspy.w -= 0x0010;
            }
            if (i & J_DOWN) {
                wspy.w += 0x0010;
            }
            if (i & J_LEFT) {
                wspx.w -= 0x0010;
            }
            if (i & J_RIGHT) {
                wspx.w += 0x0010;
            }
        } else {
            if (i & J_SELECT) {
                color = STARTFADE;
            }
            if (i & J_START)
                if (doorstate == CLOSED) {
                    doorstate = OPENING;
                    doorpos = 0;
                } else if (doorstate == OPENED) {
                    doorstate = CLOSING;
                    doorpos = NBDFRAMES;
                }
            if (i & J_UP) {
                sspy.w -= 0x0010;
            }
            if (i & J_DOWN) {
                sspy.w += 0x0010;
            }
            if (i & J_LEFT) {
                sspx.w -= 0x0010;
            }
            if (i & J_RIGHT) {
                sspx.w += 0x0010;
            }
        }
    }
}
