//#include <gba.h>
#include <string.h>
#include <maxmod.h>
#include "toolbox.h"
//include <types.h>
#include "input.h"
//sprites

//backgrounds
#include "Sky.h"
#include "DunesFar.h"
#include "DunesNear.h"
#include "TitleLogo.h"

//music and soundbanks
#include "soundbank.h"
#include "soundbank_bin.h"
#define IRQ_VBLANK 0x001
#define SkyLayer 3
void renderSky()
{
	// Load sky palette
	memcpy16(pal_bg_mem, SkyPal, 16);
	// Load tiles into CBB 0
	memcpy16(&tile_mem[SkyLayer][0], SkyTiles, SkyTilesLen);
	// Load map into SBB 30
	memcpy16(&se_mem[22][0], SkyMap, SkyMapLen);

	// set up BG1 for a 4bpp 32x32t map, using charblock 1 and screenblock 20
	REG_BG3CNT= BG_PRIO(3) | BG_CBB(SkyLayer) | BG_SBB(22) | BG_4BPP | BG_REG_32x32;
}

#define DunesFarLayer 2
void renderDunesFar()
{
	// Load duneFar palette
	memcpy16(pal_bg_mem+16, DunesFarPal, 16);
	// Load tiles into CBB 0
	memcpy16(&tile_mem[DunesFarLayer][0], DunesFarTiles, DunesFarTilesLen);
	// Load map into SBB 30
	memcpy16(&se_mem[19][0], DunesFarMap, DunesFarMapLen);
	
	//set all the tiles of that layer to theri palette
	for (int i=0; i<DunesFarTilesLen; i++)
		se_mem[19][i] |= SE_PALBANK(1);

	// set up BG0 for a 4bpp 32x32t map, using charblock 0 and screenblock 22
	REG_BG2CNT= BG_PRIO(2) | BG_CBB(DunesFarLayer) | BG_SBB(19) | BG_4BPP | BG_REG_32x32;
}

#define DunesNearLayer 1
void renderDunesNear()
{
	// Load duneFar palette
	memcpy16(pal_bg_mem+32, DunesNearPal, 16);
	// Load tiles into CBB 0
	memcpy16(&tile_mem[DunesNearLayer][0], DunesNearTiles, DunesNearTilesLen);
	// Load map into SBB 30
	memcpy16(&se_mem[20][0], DunesNearMap, DunesNearMapLen);
	
	//set all the tiles of that layer to theri palette
	for (int i=0; i<DunesNearTilesLen; i++)
		se_mem[20][i] |= SE_PALBANK(2);

	// set up BG0 for a 4bpp 32x32t map, using charblock 0 and screenblock 22
	REG_BG1CNT= BG_PRIO(1) | BG_CBB(DunesNearLayer) | BG_SBB(20) | BG_4BPP | BG_REG_32x32;
}

#define TitleLogoLayer 0
void renderLogo()
{
	// Load duneFar palette
	memcpy16(pal_bg_mem+48, TitleLogoPal, 16);
	// Load tiles into CBB 0
	memcpy16(&tile_mem[TitleLogoLayer][0], TitleLogoTiles, TitleLogoTilesLen);
	// Load map into SBB 30
	memcpy16(&se_mem[21][0], TitleLogoMap, TitleLogoMapLen);
	
	//set all the tiles of that layer to theri palette
	for (int i=0; i<TitleLogoTilesLen; i++)
		se_mem[21][i] |= SE_PALBANK(3);

	// set up BG0 for a 4bpp 32x32t map, using charblock 0 and screenblock 22
	REG_BG0CNT= BG_PRIO(0) | BG_CBB(TitleLogoLayer) | BG_SBB(21) | BG_4BPP | BG_REG_32x32;

	//position the logo somewhere close the center i guess
	REG_BG0HOFS= -64;
	REG_BG0VOFS= -32;
}

//shitty wave for the bobbing logo
const int wave[8]={0,1,3,4,5,4,2,1};

int main()
{
	irqInit();
	irqSet( IRQ_VBLANK, mmVBlank );
	irqEnable(IRQ_VBLANK);
	// initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 24 );

	// Start playing module
	mmStart( MOD_BGM, MM_PLAY_LOOP );
	renderDunesFar();
	renderDunesNear();
	renderLogo();
	renderSky();
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3;

	// Scroll around shit and later reuse as much of temporary variables as possible
	int x = 0, x1 = 0, w = 0, wt = 0, temp = 0;
	while(1)
	{
				
		VBlankIntrWait();
		mmFrame();
		vid_vsync();
		key_poll();

		//tick the ticker
		temp++;temp%=8;

		//stepp the layers kinda rightward 
		if(temp == 3){x++;x1++;}//layer 2 moves every 8 frames
		if(temp == 7){x1++;}//layer 1 moves every 4 frames

		//loop them layers
		x %= 256;x1 %= 256;

		//scrolling them layers, whoosh
		REG_BG1HOFS= x1;

		REG_BG2HOFS= x;

		//put the far away dunes a little higher
		REG_BG2VOFS= 12;
		
		//bobbing logo
		wt++;
		if(wt>15)
		{wt = 0;w++;}
		w%=8;
		REG_BG0VOFS= -32+wave[w];

		//do some sparkles and a "press start" prompt here tomorrow?
	}

	return 0;
}
