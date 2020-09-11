#include <maxmod.h>
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
	memcpy(&se_mem[20][0], title_bgMap, title_bgMapLen);

	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_PRIO(3) | BG_CBB(0) | BG_SBB(20) | BG_4BPP | BG_AFF_16x16;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;


}

void render_parallax1()
{
	int ix, iy;
	// Load palette
	memcpy(pal_bg_mem+16, title_parallax1Pal, title_parallax1PalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[1][0], title_parallax1Tiles, title_parallax1TilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[22][0], title_parallax1Map, title_parallax1MapLen);
      //  pal_bg_mem[17]= pal_bg_mem[17] ^ CLR_MAG;
	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG1CNT= BG_PRIO(2) | BG_CBB(1) | BG_SBB(22) | BG_4BPP | BG_AFF_16x16;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1;


}
void render_parallax2()
{
	int ix, iy;
	// Load palette
	memcpy(pal_bg_mem+32, title_parallax2Pal, title_parallax2PalLen);
	// Load tiles into CBB 0
	memcpy(&tile_mem[2][0], title_parallax2Tiles, title_parallax2TilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[24][0], title_parallax2Map, title_parallax2MapLen);

    //pal_bg_mem[32]= pal_bg_mem[32] ^ CLR_MAG;
	// set up BG0 for a 4bpp 64x32t map, using
	//   using charblock 0 and screenblock 31
	REG_BG2CNT= BG_PRIO(1) | BG_CBB(2) | BG_SBB(24) | BG_4BPP | BG_AFF_16x16;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG2;


}

int main()
{
	render_bg();
	render_parallax1();
	render_parallax2();
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2;

	
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
		REG_BG2HOFS= x*.25;
		REG_BG2VOFS= y;
	}

	return 0;
}
