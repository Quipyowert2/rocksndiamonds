/* 2000-08-13T14:36:17Z
 *
 * graphics manipulation crap
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "global.h"
#include "display.h"
#include "level.h"

#include <stdio.h>


#if defined(TARGET_X11)

unsigned int frame; /* current frame */
unsigned int screen_x; /* current scroll position */
unsigned int screen_y;

static unsigned short screentiles[14][22]; /* tiles currently on screen */

static unsigned int colours[8];
static unsigned int colour_anim;

static void xdebug(char *msg)
{
#if 0
  XSync(display, False);
  printf("EM DEBUG: %s\n", msg);
#endif
}

static void colour_shuffle(void)
{
  unsigned int i, j, k;

  for (i = 0; i < 8; i++)
    colours[i] = i;

  for (i = 0; i < 8; i++)
  {
    Random = Random * 129 + 1;
    j = (Random >> 10) & 7;
    k = colours[i];
    colours[i] = colours[j];
    colours[j] = k;
  }
}

/* copy the entire screen to the window at the scroll position
 *
 * perhaps use mit-shm to speed this up
 */
void blitscreen(void)
{
  unsigned int x = screen_x % (22 * TILEX);
  unsigned int y = screen_y % (14 * TILEY);

  xdebug("blitscreen");

  if (x < 2 * TILEX && y < 2 * TILEY)
  {
    BlitBitmap(screenBitmap, window, x, y,
	       20 * TILEX, 12 * TILEY, SX, SY);
  }
  else if (x < 2 * TILEX && y >= 2 * TILEY)
  {
    BlitBitmap(screenBitmap, window, x, y,
	       20 * TILEX, 14 * TILEY - y, SX, SY);
    BlitBitmap(screenBitmap, window, x, 0,
	       20 * TILEX, y - 2 * TILEY, SX, SY + 14 * TILEY - y);
  }
  else if (x >= 2 * TILEX && y < 2 * TILEY)
  {
    BlitBitmap(screenBitmap, window, x, y,
	       22 * TILEX - x, 12 * TILEY, SX, SY);
    BlitBitmap(screenBitmap, window, 0, y,
	       x - 2 * TILEX, 12 * TILEY, SX + 22 * TILEX - x, SY);
  }
  else
  {
    BlitBitmap(screenBitmap, window, x, y,
	       22 * TILEX - x, 14 * TILEY - y, SX, SY);
    BlitBitmap(screenBitmap, window, 0, y,
	       x - 2 * TILEX, 14 * TILEY - y, SX + 22 * TILEX - x, SY);
    BlitBitmap(screenBitmap, window, x, 0,
	       22 * TILEX - x, y - 2 * TILEY, SX, SY + 14 * TILEY - y);
    BlitBitmap(screenBitmap, window, 0, 0,
	       x - 2 * TILEX, y - 2 * TILEY,
	       SX + 22 * TILEX - x, SY + 14 * TILEY - y);
  }

  BlitBitmap(scoreBitmap, window, 0, 0,
	     20 * TILEX, SCOREY, SX, SY + 12 * TILEY);
  XFlush(display);

  xdebug("blitscreen - done");
}

/* draw differences between game tiles and screen tiles
 *
 * implicitly handles scrolling and restoring background under the sprites
 *
 * perhaps use mit-shm to speed this up
 */
static void animscreen(void)
{
  unsigned int x, y, dx, dy;
  unsigned short obj;
  unsigned int left = screen_x / TILEX;
  unsigned int top = screen_y / TILEY;

  xdebug("animscreen");

  for (y = top; y < top + 14; y++)
  {
    dy = y % 14;
    for (x = left; x < left + 22; x++)
    {
      dx = x % 22;
      obj = map_obj[frame][Draw[y][x]];

      if (screentiles[dy][dx] != obj)
      {
	screentiles[dy][dx] = obj;
	BlitBitmap(objBitmap, screenBitmap,
		   (obj / 512) * TILEX, (obj % 512) * TILEY / 16,
		   TILEX, TILEY, dx * TILEX, dy * TILEY);
      }
    }
  }
}

/* blit players to the screen
 *
 * handles transparency and movement
 */
static void blitplayer(struct PLAYER *ply)
{
  unsigned int x, y, dx, dy;
  unsigned short obj, spr;

  xdebug("blitplayer");

  if (ply->alive)
  {
    x = (frame * ply->oldx + (8 - frame) * ply->x) * TILEX / 8;
    y = (frame * ply->oldy + (8 - frame) * ply->y) * TILEY / 8;
    dx = x + TILEX - 1;
    dy = y + TILEY - 1;

    if ((unsigned int)(dx - screen_x) < (21 * TILEX - 1) &&
	(unsigned int)(dy - screen_y) < (13 * TILEY - 1))
    {
      spr = map_spr[ply->num][frame][ply->anim];
      x %= 22 * TILEX;
      y %= 14 * TILEY;
      dx %= 22 * TILEX;
      dy %= 14 * TILEY;

      if (objmaskBitmap)
      {
	obj = screentiles[y / TILEY][x / TILEX];
	XCopyArea(display, objmaskBitmap, spriteBitmap, spriteGC,
		  (obj / 512) * TILEX, (obj % 512) * TILEY / 16, TILEX, TILEY,
		  -(x % TILEX), -(y % TILEY));

	obj = screentiles[dy / TILEY][dx / TILEX];
	XCopyArea(display, objmaskBitmap, spriteBitmap, spriteGC,
		  (obj / 512) * TILEX, (obj % 512) * TILEY / 16, TILEX, TILEY,
		  (22 * TILEX - x) % TILEX, (14 * TILEY - y) % TILEY);
      }
      else if (sprmaskBitmap)
      {
	XCopyArea(display, sprmaskBitmap, spriteBitmap, spriteGC,
		  (spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY, 0, 0);
      }
      else
      {
	XFillRectangle(display, spriteBitmap, spriteGC, 0, 0, TILEX, TILEY);
      }

      screentiles[y / TILEY][x / TILEX] = -1; /* mark screen as dirty */
      screentiles[dy / TILEY][dx / TILEX] = -1;

#if 1


#if 1

      SetClipMask(sprBitmap, sprBitmap->stored_clip_gc, spriteBitmap);

      SetClipOrigin(sprBitmap, sprBitmap->stored_clip_gc, x, y);
      BlitBitmapMasked(sprBitmap, screenBitmap,
		       (spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		       x, y);

      SetClipOrigin(sprBitmap, sprBitmap->stored_clip_gc, x - 22 * TILEX, y);
      BlitBitmapMasked(sprBitmap, screenBitmap,
		       (spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		       x - 22 * TILEX, y);

      SetClipOrigin(sprBitmap, sprBitmap->stored_clip_gc, x, y - 14 * TILEY);
      BlitBitmapMasked(sprBitmap, screenBitmap,
		       (spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		       x, y - 14 * TILEY);

      SetClipMask(sprBitmap, sprBitmap->stored_clip_gc, None);

#else

      XSetClipMask(display, sprBitmap->stored_clip_gc, spriteBitmap);

      XSetClipOrigin(display, sprBitmap->stored_clip_gc, x, y);
      XCopyArea(display, sprBitmap->drawable, screenBitmap->drawable,
		sprBitmap->stored_clip_gc,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x, y);

      XSetClipOrigin(display, sprBitmap->stored_clip_gc, x - 22 * TILEX, y);
      XCopyArea(display, sprBitmap->drawable, screenBitmap->drawable,
		sprBitmap->stored_clip_gc,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x - 22 * TILEX, y);

      XSetClipOrigin(display, sprBitmap->stored_clip_gc, x, y - 14 * TILEY);
      XCopyArea(display, sprBitmap->drawable, screenBitmap->drawable,
		sprBitmap->stored_clip_gc,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x, y - 14 * TILEY);

      XSetClipMask(display, sprBitmap->stored_clip_gc, None);

#endif

#else

      XSetClipMask(display, screenGC, spriteBitmap);
      XSetClipOrigin(display, screenGC, x, y);
      XCopyArea(display, sprPixmap, screenPixmap, screenGC,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x, y);
      XSetClipOrigin(display, screenGC, x - 22 * TILEX, y);
      XCopyArea(display, sprPixmap, screenPixmap, screenGC,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x - 22 * TILEX, y);
      XSetClipOrigin(display, screenGC, x, y - 14 * TILEY);
      XCopyArea(display, sprPixmap, screenPixmap, screenGC,
		(spr / 8) * TILEX, (spr % 8) * TILEY, TILEX, TILEY,
		x, y - 14 * TILEY);
      XSetClipMask(display, screenGC, None);

#endif
    }
  }
}

void game_initscreen(void)
{
  unsigned int x,y;

  xdebug("game_initscreen");

  frame = 6;
  screen_x = 0;
  screen_y = 0;

  for (y = 0; y < 14; y++)
    for (x = 0; x < 22; x++)
      screentiles[y][x] = -1;

  colour_shuffle();
  colours[0] += 16;
  colours[1] += 16;
  colours[2] += 16;
  colour_anim = 0;

  ClearRectangle(scoreBitmap, 0, 0, 20 * TILEX, SCOREY);
  BlitBitmap(botBitmap, scoreBitmap,
	     11 * SCOREX, colours[0] * SCOREY, 3 * SCOREX, SCOREY,
	     1 * SCOREX, 0);				/* 0-63 time */
  BlitBitmap(botBitmap, scoreBitmap,
	     18 * SCOREX, colours[0] * SCOREY, 6 * SCOREX, SCOREY,
	     15 * SCOREX, 0);				/* 112-207 diamonds */
  BlitBitmap(botBitmap, scoreBitmap,
	     14 * SCOREX, colours[0] * SCOREY, 4 * SCOREX, SCOREY,
	     32 * SCOREX, 0);				/* 256-319 score */
}

void game_blitscore(void)
{
  unsigned int i;

  xdebug("game_blitscore");

  i = (lev.time + 4) / 5;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     7 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     6 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     5 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     4 * SCOREX, 0);

  i = lev.score;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     39 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     38 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     37 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     36 * SCOREX, 0);

  if (lev.home == 0)
  {
    BlitBitmap(botBitmap, scoreBitmap,
	       12 * SCOREX, 24 * SCOREY, 12 * SCOREX, SCOREY,
	       14 * SCOREX, 0); /* relax */

    goto done;
  }

  if (ply1.alive + ply2.alive >= lev.home && lev.required == 0)
  {
    BlitBitmap(botBitmap, scoreBitmap,
	       24 * SCOREX, colours[2] * SCOREY, 12 * SCOREX, SCOREY,
	       14 * SCOREX, 0); /* find the exit */

    goto done;
  }

  if (ply1.alive + ply2.alive < lev.home)
  {
    if (++colour_anim > 11)
      colour_anim = 0;

    if (colour_anim < 6)
    {
      BlitBitmap(botBitmap, scoreBitmap,
		 0, 24 * SCOREY, 12 * SCOREX, SCOREY,
		 14 * SCOREX, 0); /* forget it */

      goto done;
    }

    BlitBitmap(botBitmap, scoreBitmap,
	       18 * SCOREX, colours[0] * SCOREY, 6 * SCOREX, SCOREY,
	       15 * SCOREX, 0); /* diamonds */
  }

  i = lev.required;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     24 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     23 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     22 * SCOREX, 0); i /= 10;
  BlitBitmap(botBitmap, scoreBitmap,
	     (i % 10) * SCOREX, colours[1] * SCOREY, SCOREX, SCOREY,
	     21 * SCOREX, 0);

 done:
}

void game_animscreen(void)
{
  unsigned int x,y;

  xdebug("game_animscreen");

  x = (frame * ply1.oldx + (8 - frame) * ply1.x) * TILEX / 8
    + (19 * TILEX) / 2;
  y = (frame * ply1.oldy + (8 - frame) * ply1.y) * TILEY / 8
    + (11 * TILEY) / 2;

  if (x > lev.width * TILEX)
    x = lev.width * TILEX;
  if (y > lev.height * TILEY)
    y = lev.height * TILEY;
  if (x < 20 * TILEX)
    x = 20 * TILEY;
  if (y < 12 * TILEY)
    y = 12 * TILEY;

  screen_x = x - 19 * TILEX;
  screen_y = y - 11 * TILEY;

  animscreen();
  blitplayer(&ply1);
  blitplayer(&ply2);
  blitscreen();

  XFlush(display);

  Random = Random * 129 + 1;
}

void title_initscreen(void)
{
  xdebug("title_initscreen");

  screen_x = 0;
  screen_y = 0;

  colour_shuffle();
  colours[1] += 8;
  colour_anim = 0;

#if 1

  BlitBitmap(ttlBitmap, screenBitmap,
	     0, 0, 20 * TILEX, 12 * TILEY, 0, 0);

  if (botmaskBitmap)
  {
    BlitBitmap(botBitmap, scoreBitmap,
	      0, colours[1] * SCOREY, 20 * TILEX, SCOREY, 0, 0);

    SetClipOrigin(botBitmap, botBitmap->stored_clip_gc,
		  0, 0 - colours[0] * SCOREY);
  }

  BlitBitmapMasked(botBitmap, scoreBitmap,
		   0, colours[0] * SCOREY, 20 * TILEX, SCOREY, 0, 0);

#else

  XCopyArea(display, ttlPixmap, screenPixmap, screenGC,
	    0, 0, 20 * TILEX, 12 * TILEY, 0, 0);

  if (botmaskBitmap)
  {
    XCopyArea(display, botPixmap, scorePixmap, scoreGC,
	      0, colours[1] * SCOREY, 20 * TILEX, SCOREY, 0, 0);
    XSetClipMask(display, scoreGC, botmaskBitmap);
    XSetClipOrigin(display, scoreGC, 0, 0 - colours[0] * SCOREY);
  }

  XCopyArea(display, botPixmap, scorePixmap, scoreGC,
	    0, colours[0] * SCOREY, 20 * TILEX, SCOREY, 0, 0);

  if (botmaskBitmap)
    XSetClipMask(display, scoreGC, None);

#endif
}

void title_blitscore(void)
{
  unsigned int x, y, i;

  xdebug("title_blitscore");

  if (++colour_anim > 30)
    colour_anim = 0;

  i = colour_anim >= 16 ? 31 - colour_anim : colour_anim;
  x = (i / 8 + 18) * 2 * SCOREX;
  y = (i % 8 + 16) * SCOREY;

#if 1
  if (botmaskBitmap)
  {
    BlitBitmap(botBitmap, scoreBitmap,
	       32 * SCOREX, colours[1] * SCOREY, 2 * SCOREX, SCOREY,
	       32 * SCOREX, 0);

    SetClipOrigin(botBitmap, botBitmap->stored_clip_gc,
		  32 * SCOREX - x, 0 - y);
  }

  BlitBitmapMasked(botBitmap, scoreBitmap,
		   x, y, 2 * SCOREX, SCOREY, 32 * SCOREX, 0);

#else

  if (botmaskBitmap)
  {
    XCopyArea(display, botPixmap, scorePixmap, scoreGC,
	      32 * SCOREX, colours[1] * SCOREY, 2 * SCOREX, SCOREY,
	      32 * SCOREX, 0);
    XSetClipMask(display, scoreGC, botmaskBitmap);
    XSetClipOrigin(display, scoreGC, 32 * SCOREX - x, 0 - y);
  }

  XCopyArea(display, botPixmap, scorePixmap, scoreGC,
	    x, y, 2 * SCOREX, SCOREY, 32 * SCOREX, 0);

  if (botmaskBitmap)
    XSetClipMask(display, scoreGC, None);
#endif
}

void title_blitants(unsigned int y)
{
  static const char ants_dashes[2] = { 8, 7 };

  xdebug("title_blitants");

  XSetDashes(display, antsGC, colour_anim, ants_dashes, 2);
  XDrawRectangle(display, screenPixmap, antsGC,
		 0, y * TILEY, 20 * TILEX - 1, TILEY - 1);
}

void title_animscreen(void)
{
  blitscreen();
  XFlush(display);

  Random = Random * 129 + 1;
}

static int ttl_map[] =
{
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,0,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,2,3,4,-1,      /* !',-. */
  5,6,7,8,9,10,11,12,13,14,15,-1,-1,-1,-1,16,      /* 0123456789:? */
  -1,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, /* ABCDEFGHIJKLMNO */
  32,33,34,35,36,37,38,39,40,41,42,-1,-1,-1,-1,-1, /* PQRSTUVWXYZ */
  -1,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, /* abcdefghijklmno */
  32,33,34,35,36,37,38,39,40,41,42,-1,-1,-1,-1,-1  /* pqrstuvwxyz */
};

void title_string(unsigned int y, unsigned int left, unsigned int right,
		  char *string)
{
  int i;
  unsigned int x;

  xdebug("title_string");

  y *= TILEY;
  left *= SCOREX;
  right *= SCOREX;

  x = (left + right - strlen(string) * MENUFONTX) / 2;
  if (x < left || x >= right) x = left;

  /* restore background graphic where text will be drawn */
  BlitBitmap(ttlBitmap, screenBitmap,
	     left, y, right - left, MENUFONTY, left, y);

#if 1
#else
  if (ttlmaskBitmap)
    XSetClipMask(display, screenGC, ttlmaskBitmap);
#endif

  for (i = 0; string[i] && x < right; i++)
  {
    int ch_pos, ch_x, ch_y;

    ch_pos = ttl_map[string[i] & 127];

    if (ch_pos == -1 || ch_pos > 22 * 2)
      continue;				/* no graphic for this character */

    ch_x = (ch_pos % 22) * GFXMENUFONTX;
    ch_y = (ch_pos / 22 + 12) * TILEY;

#if 1
    SetClipOrigin(ttlBitmap, ttlBitmap->stored_clip_gc,
		  x - ch_x, y - ch_y);

    BlitBitmapMasked(ttlBitmap, screenBitmap,
		     ch_x, ch_y, MENUFONTX, MENUFONTY, x, y);
#else
    if (ttlmaskBitmap)
      XSetClipOrigin(display, screenGC, x - ch_x, y - ch_y);

    XCopyArea(display, ttlPixmap, screenPixmap, screenGC,
	      ch_x, ch_y, MENUFONTX, MENUFONTY, x, y);
#endif

    x += MENUFONTX;
  }

#if 1
#else
  XSetClipMask(display, screenGC, None);
#endif
}

#endif