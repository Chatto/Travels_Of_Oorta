#include <string.h>
#include "toolbox.h"
#include "input.h"
#include "title_bg.h"
#include "title_parallax1.h"
#include "title_parallax2.h"

void render_bg()
{
	// Load palette
	memcpy(pal_bg_mem, title_bgPal, title_bgPalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[0][0], title_bgTiles, title_bgTilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[28][0], title_bgMap, title_bgMapLen);

	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_AFF_16x16;

}

void render_parallax()
{
	int ix, iy;
	// Load palette
	memcpy(pal_bg_mem+16, title_parallax1Pal, title_parallax1PalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[16][0], title_parallax1Tiles, title_parallax1TilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[28][0], title_parallax1Map, title_parallax1MapLen);

    for(ix=1; ix<32; ix++)
        pal_bg_mem[ix]= pal_bg_mem[ix] ^ CLR_MAG;
	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG1CNT= BG_CBB(16) | BG_SBB(28) | BG_4BPP | BG_AFF_16x16;

}

int main()
{
	render_bg();
	render_parallax();
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1;
/*
	// Load palette
	memcpy(pal_bg_mem, title_parallax1Pal, title_parallax1PalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[0][0], title_parallax1Tiles, title_parallax1TilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[28][0], title_parallax1Map, title_parallax1MapLen);

	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(1) | BG_SBB(28) | BG_8BPP | BG_REG_64x64;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1;
*/
	// Scroll around some
	int x= 0, y= 0;
	while(1)
	{
		vid_vsync();
		key_poll();

		x += 1;

		REG_BG0HOFS= x;
		REG_BG0VOFS= y;
		REG_BG1HOFS= x*.5;
		REG_BG1VOFS= y;
	}

	return 0;
}
