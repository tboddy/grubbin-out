// start

#define START_BG_HEIGHT 28
#define START_BG_WIDTH 32
#define START_I 16
#define START_LIMIT 220

u16 startClock;


// menu

#define START_MENU_X 11
#define START_MENU_Y 19

u8 currentStartMenu, lastStartMenu;
bool selectingStartMenu, aboutShowing;
char startHighScoreStr[10];

static void loadMenu(){
	VDP_drawText(">", START_MENU_X - 1, START_MENU_Y);
	VDP_drawText("START GAME", START_MENU_X, START_MENU_Y);
	VDP_drawText("ABOUT", START_MENU_X, START_MENU_Y + 2);
	VDP_drawText("2023 T.BODDY", 19, 26);
	intToStr(highScore, startHighScoreStr, 8);
	VDP_drawText("HI", 1, 26);
	VDP_drawText(startHighScoreStr, 4, 26);
}

void selectStartMenu(){
	selectingStartMenu = TRUE;
	switch(currentStartMenu){
		case 0:
			resetStart();
			XGM_startPlayPCM(SFX_START_GAME, 1, SOUND_PCM_CH2);
			VDP_setTextPriority(1);
			loadGame();
			break;
		case 1:
			// loadStartAbout();
			// XGM_startPlayPCM(SFX_MENU_CHOOSE, 1, SOUND_PCM_CH2);
			break;
	}
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

	if(currentStartMenu != lastStartMenu && !aboutShowing){
		VDP_clearTileMapRect(BG_A, START_MENU_X - 1, START_MENU_Y + lastStartMenu * 2, 1, 1);
		VDP_drawText(">", START_MENU_X - 1, START_MENU_Y + currentStartMenu * 2);
		lastStartMenu = currentStartMenu;
	}

	if((controls.up || controls.down) && !selectingStartMenu && !aboutShowing){
		currentStartMenu += controls.up ? -1 : 1;
		if(currentStartMenu > 1) currentStartMenu = 0;
		else if(currentStartMenu < 0) currentStartMenu = 1;
		selectingStartMenu = TRUE;
		XGM_startPlayPCM(SFX_MENU_SELECT, 1, SOUND_PCM_CH2);
	} else if(!controls.up && !controls.down && !controls.a && !controls.b && !controls.c && !controls.start && selectingStartMenu) selectingStartMenu = FALSE;

	if((controls.a || controls.start) && !aboutShowing && !selectingStartMenu) selectStartMenu();

}


// logo

#define START_LOGO_X 8
#define START_LOGO_Y 3

void loadStartLogo(){
	VDP_drawImageEx(BG_A, &startLogo, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 8), START_LOGO_X, START_LOGO_Y, 0, DMA_QUEUE);
	// s8 logoCount = 0;
	// for(s8 i = 0; i < START_LOGO_LINES; i++) startLogoScrolls[i] = random() % 4 - 2;
	// VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_2TILE);
	// VDP_drawImageEx(BG_A, &startLogoV, TILE_ATTR_FULL(PAL1, 0, 0, 0, 100), 3, START_LOGO_Y + 4, 0, DMA_QUEUE);
	// animateStartLogo();
}

void animateStartLogo(){
	// if(startClock % 8 == 0){
	// 	for(s8 i = 0; i < START_LOGO_LINES; i++){
	// 		if(aboutShowing){
	// 			startLogoScrolls[i] = 0;
	// 		} else {
	// 			if(i < START_LOGO_LINES / 3 || (i >= START_LOGO_LINES / 3 && i < START_LOGO_LINES / 3 * 2 && startClock % 16 == 0) ||
	// 				(i >= START_LOGO_LINES / 3 * 2 && startClock % 32 == 0)){
	// 				startLogoScrollsFlip[i] ? startLogoScrolls[i]-- : startLogoScrolls[i]++;
	// 				if(startLogoScrolls[i] >= (2)) startLogoScrollsFlip[i] = TRUE;
	// 				else if(startLogoScrolls[i] <= (	-2)) startLogoScrollsFlip[i] = FALSE;
	// 			}
	// 		}
	// 	}
	// 	VDP_setHorizontalScrollLine(BG_A, 72, startLogoScrolls, START_LOGO_LINES, DMA_QUEUE);
	// }
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
			break;
	}
}









#define START_LOGO_LINES 32

#define START_GRADIENT_Y 23

bool started, loadedStart;

s16 aboutY;

s16 startLogoScrolls[START_LOGO_LINES];
bool startLogoScrollsFlip[START_LOGO_LINES], updatingStartCount;

// background

void loadStartBg(){
	// VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_TOP_TILE), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// for(s8 y = 0; y < START_BG_HEIGHT; y++)
	// 	for(s8 x = -1; x < START_BG_WIDTH; x++) if(y > START_BG_HEIGHT - 2) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, 14), x, y);
	// loadStartGradient();
}

void loadStartGradient(){
	// for(s8 x = 0; x < START_BG_WIDTH; x++){
	// 	VDP_drawImageEx(BG_B, &startGradient, TILE_ATTR_FULL(PAL2, 0, 0, 0, 200), x, START_GRADIENT_Y, 0, DMA_QUEUE);
	// 	for(s8 y = 0; y < 12; y++) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, 11), x, y + START_GRADIENT_Y);
	// }
}


// logo


// menu



// about

void loadStartAbout(){
	aboutShowing = TRUE;
	for(s8 x = 0; x < START_BG_WIDTH; x++)
		for(s8 y = 0; y < START_BG_HEIGHT; y++)
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, 13), x, y);
	VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	aboutY = 2;
	VDP_drawText("how to play", 1, aboutY); aboutY += 2;
	VDP_drawText("d=pad  MOVE", 1, aboutY); aboutY++;
	VDP_drawText("a      SHOOT", 1, aboutY); aboutY++;
	VDP_drawText("b      FOCUS/SLOW", 1, aboutY); aboutY++;
	VDP_drawText("c      BOMB", 1, aboutY); aboutY++;
	VDP_drawText("start  PAUSE", 1, aboutY);
	aboutY += 14;
	VDP_drawText("EVERYTHING BY T.BODDY", 1, aboutY); aboutY++;
	VDP_drawText("FOR touhou pride game jam iii", 1, aboutY); aboutY++;
	VDP_drawText("TOUHOU PROJECT COPYRIGHT ZUN?", 1, aboutY); aboutY += 2;
	VDP_drawText("press any button to go back", 1, aboutY); aboutY++;
	// startClock = SEGA_LIMIT + 120;
}

void startGoBack(){
	// selectingStartMenu = TRUE;
	// aboutShowing = FALSE;
	// currentStartMenu = 0;
	// VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// for(s8 y = 0; y < START_BG_HEIGHT; y++)
	// 	for(s8 x = -1; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, 13), x, y);
	// loadStartBg();
	// loadStartLogo();
	// loadStartMenu();
	// loadStartScore();
	// loadStartCredit();
}

void updateStartAbout(){
	if(startClock % 120 == 0) VDP_drawImageEx(BG_A, &startAbout, TILE_ATTR_FULL(PAL1, 0, 0, 0, 64), 22, 8, 0, DMA_QUEUE);
	else if(startClock % 120 == 60) VDP_drawImageEx(BG_A, &startAbout2, TILE_ATTR_FULL(PAL1, 0, 0, 0, 128), 22, 8, 0, DMA_QUEUE);
}


// main loop

void loadStart(){
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
	} else updateSega();
	if(startClock <= 1200) startClock++;


	// if(startClock >= SEGA_LIMIT + 15){
	// 	// updateStartMenu();
	// 	// animateStartLogo();
	// 	if((controls.a || controls.b || controls.c || controls.start) && aboutShowing && !selectingStartMenu) startGoBack();
	// 	else if((controls.a || controls.start) && !aboutShowing && !selectingStartMenu) selectStartMenu();
	// 	if(aboutShowing) updateStartAbout();
	// } else if(startClock == SEGA_LIMIT - 25){
	// 	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, 10), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// } else if(startClock == SEGA_LIMIT - 20){
	// 	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, 11), 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// } else if(startClock == SEGA_LIMIT - 15){
	// } else if(startClock == SEGA_LIMIT - 10){

	// } else if(startClock == SEGA_LIMIT - 5){
	// 	loadStartBg();
	// } else if(startClock == SEGA_LIMIT){
	// 	VDP_clearTileMapRect(BG_A, -1, 0, START_BG_WIDTH, START_BG_HEIGHT);
	// } else if(startClock == SEGA_LIMIT + 5){
	// 	// loadStartLogo();
	// 	loadStartMenu();
	// 	// loadStartScore();
	// 	// loadStartCredit();
	// 	// XGM_startPlay(&bgmStart);
	// }
}