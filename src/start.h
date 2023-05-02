// start

#define START_BG_HEIGHT 28
#define START_BG_WIDTH 32
#define START_I 16
#define START_LIMIT 220

u16 startClock, demoStartClock;

bool started, loadedStart;

// menu

#define START_MENU_X 8
#define START_MENU_Y 18

u8 currentStartMenu, lastStartMenu;
bool selectingStartMenu, aboutShowing;
char startHighScoreStr[10];

static void loadMenu(){
	VDP_drawText("a touhou fangame", START_MENU_X, START_MENU_Y - 1);
	VDP_drawText("PRESS ANY BUTTON", START_MENU_X, START_MENU_Y + 1);
	VDP_drawText("a SHOOT", START_MENU_X, START_MENU_Y + 3);
	VDP_drawText("b FOCUS", START_MENU_X, START_MENU_Y + 4);
	VDP_drawText("c BOMB", START_MENU_X, START_MENU_Y + 5);
	VDP_drawText("2023  TBODDY.ITCH.IO", START_MENU_X - 2, 26);
	// intToStr(highScore, startHighScoreStr, 8);
	// VDP_drawText("HI", 1, 26);
	// VDP_drawText(startHighScoreStr, 4, 26);
}

void selectStartMenu(){
	selectingStartMenu = TRUE;
	resetStart();
	XGM_startPlayPCM(SFX_START_GAME, 1, SOUND_PCM_CH2);
	VDP_setTextPriority(1);
	loadGame();
}

static void updateMenu(){
	switch(startClock){
		case START_LIMIT:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 1), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT + 5:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 2), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT + 10:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 3), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT + 15:
			VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
	}
	if((controls.a || controls.start || controls.b || controls.c) && !aboutShowing && !selectingStartMenu) selectStartMenu();

}


// logo

#define START_LOGO_X 8
#define START_LOGO_Y 1

void loadStartLogo(){
	VDP_drawImageEx(BG_A, &startLogo, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 8), START_LOGO_X, START_LOGO_Y, 0, DMA_QUEUE);
}

// sega

#define SEGA_X 9
#define SEGA_Y 12

static void loadSega(){
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	VDP_drawImageEx(BG_A, &startBoddy, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 8), SEGA_X, SEGA_Y, 0, DMA);
}

static void updateSega(){
	switch(startClock){
		case 5:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 1), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case 10:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 2), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case 15:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 3), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case 20:
			VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT - 30:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 3), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT - 25:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 2), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT - 20:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I + 1), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT - 15:
			VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL2, 1, 0, 0, START_I), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
			break;
		case START_LIMIT - 10:
			loadStartLogo();
			loadMenu();
			XGM_startPlay(&bgmStart);
			break;
	}
}

static void startDemo(){
	selectingStartMenu = TRUE;
	resetStart();
	demo = TRUE;
	VDP_setTextPriority(1);
	loadGame();
}


// main loop

void loadStart(){
	demo = FALSE;
	VDP_setTextPriority(0);
	VDP_setScreenWidth256();
	playerLives = 2;
	playerBombs = 3;
	VDP_loadTileSet(fade1.tileset, START_I, DMA);
	VDP_loadTileSet(fade2.tileset, START_I + 1, DMA);
	VDP_loadTileSet(fade3.tileset, START_I + 2, DMA);
	VDP_loadTileSet(fade4.tileset, START_I + 3, DMA);
	loadSega();
}

void resetStart(){
	VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
}

void updateStart(){
	if(startClock >= START_LIMIT){
		updateMenu();
		demoStartClock++;
	} else updateSega();
	if(startClock <= CLOCK_LIMIT) startClock++;
	if(demoStartClock >= 900) startDemo();
}