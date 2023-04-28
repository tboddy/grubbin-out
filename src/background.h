#define BG_SCROLL_WIDTH 12
#define BG_SIZE -64
#define BG_I 32

#define BG_W 3
#define BG_H 4

#define BG_SPEED 2

s16 bgScroll, bgI;
Image *bgImg;

s16 bgScrolls[BG_SCROLL_WIDTH];

void drawBackground(){
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_2TILE);
	bgImg = &rock1;
	bgI = BG_I;
	for(u8 x = 0; x < BG_W; x++)
		for(u8 y = 0; y < BG_H; y++)
			VDP_drawImageEx(BG_B, bgImg, TILE_ATTR_FULL(PAL2, 0, 0, 0, bgI), x * 8, y * 8, 0, DMA_QUEUE);
}


// stage

void updateBackgroundStage(){
	if(!boss.active){
		for(u8 i = 0; i < BG_SCROLL_WIDTH; i++){
			bgScrolls[i] -= BG_SPEED;
			if(bgScrolls[i] <= BG_SIZE) bgScrolls[i] = 0;
		}
		VDP_setVerticalScrollTile(BG_B, 0, bgScrolls, BG_SCROLL_WIDTH, DMA);
	}
}


// loop

void updateBackground(){
	if(gameStarting) drawBackground();
	else updateBackgroundStage();
}