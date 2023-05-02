// #define ZONE_OVER_CHROME_LIMIT 1
#define ZONE_OVER_CHROME_LIMIT 240

s16 zoneOverClock, zoneOverStage, bossTileIndex;
s8 chromePlayerLives, chromePlayerBombs;
s32 chromeCurrentScore;

fix16 bossLimit, lastBossLimit;

bool loadedZoneOver, loadedChromeGameOver;

char currentZoneStr[2],
	zoneHudStr[2],
	currentZoneApp[1],
	zoneOverTime[8],
	chromeScoreStr[10],
	bossName[12];


// frame

#define FRAME_I 8
#define FRAME_W (WIN_WIDTH - GAME_WIDTH) / 8
#define FRAME_X GAME_WIDTH / 8
#define FRAME_H GAME_HEIGHT / 8

#define HUD_X GAME_WIDTH / 8 + 1

static void drawFrame(){
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I), FRAME_X + 1, 0, FRAME_W - 1, FRAME_H);
	VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, FRAME_I + 1), FRAME_X, 0, 1, FRAME_H);
}


// score

#define SCORE_Y 4

void updateChromeScore(){
	chromeCurrentScore = currentScore;
	intToStr(chromeCurrentScore, chromeScoreStr, 6);
	VDP_drawText("SCORE", HUD_X, SCORE_Y);
	VDP_drawText(chromeScoreStr, HUD_X, SCORE_Y + 1);
}


// lives

#define LIVES_Y 7

static void loadChromeLives(){
	VDP_drawText("PLAYER", HUD_X, LIVES_Y);
}

static void updateChromePlayerLives(){
	if(chromePlayerLives != playerLives){
		for(s8 x = 0; x < 4; x++){
			VDP_clearText(x + HUD_X, LIVES_Y + 1, 1);
			if(x < playerLives) VDP_drawText("#", x + HUD_X, LIVES_Y + 1);
		}
		chromePlayerLives = playerLives;
	}
}


// bombs

#define BOMBS_Y 10

static void loadChromeBombs(){
	VDP_drawText("BOMBS", HUD_X, BOMBS_Y);
}

static void updateChromePlayerBombs(){
	if(chromePlayerBombs != playerBombs){
		for(s8 x = 0; x < 6; x++){
			VDP_clearText(x + HUD_X, BOMBS_Y + 1, 1);
			if(x < playerBombs) VDP_drawText("*", x + HUD_X, BOMBS_Y + 1);
		}
		chromePlayerBombs = playerBombs;
	}
}


// zone

#define ZONE_Y 2

void loadChromeZone(){
	intToStr(currentZone, zoneHudStr, 2);
	VDP_drawText("LVL", HUD_X, ZONE_Y);
	VDP_drawText(zoneHudStr, HUD_X + 4, ZONE_Y);
}


// states

#define OVER_X 3
#define OVER_Y 8

void loadChromeZoneOver(){
	chromePlayerLives = 0;
	chromePlayerBombs = 0;
	intToStr(currentZone, currentZoneStr, 2);
	VDP_drawText("LEVEL", OVER_X, OVER_Y);
	VDP_drawText(currentZoneStr, OVER_X + 6, OVER_Y);
	VDP_drawText("COMPLETE!", OVER_X + 9, OVER_Y);

	VDP_drawText("NO-MISS", OVER_X, OVER_Y + 4);
	VDP_drawText(noMiss ? (currentZone >= 10 ? "3500" : "2500") : "0000", OVER_X + 14, OVER_Y + 4);

	VDP_drawText("LEVEL", OVER_X, OVER_Y + 6);
	VDP_drawText(currentZone >= 10 ? "1500" : "1000", OVER_X + 14, OVER_Y + 6);

	if(currentZone % 5 == 0){
		VDP_drawText("BOSS", OVER_X, OVER_Y + 8);
		VDP_drawText(currentZone >= 10 ? "3500" : "2500", OVER_X + 14, OVER_Y + 8);
		currentScore += currentZone >= 10 ? 3000 : 2000;
	}

	if(currentZone == 10) XGM_stopPlay();
	currentZone++;
	VDP_drawText("next level", OVER_X, 21);
	loadedZoneOver = TRUE;
	if(noMiss) currentScore += currentZone >= 10 ? 3500 : 2500;
	currentScore += currentZone >= 10 ? 1500 : 1000;
	updateChromeScore();
	XGM_startPlayPCM(SFX_ZONE_OVER, 1, SOUND_PCM_CH2);
}

void updateChromeZoneOver(){ // what the fuck am i on to do this
	strcpy(zoneOverTime, zoneOverClock >= 180 ? "3" : (zoneOverClock >= 120 ? "2" : (zoneOverClock >= 60 ? "1" : "0")));
	strcat(zoneOverTime, ":");
	if(zoneOverClock % 60 < 10) strcat(zoneOverTime, "0");
	else if(zoneOverClock % 60 < 20) strcat(zoneOverTime, "1");
	else if(zoneOverClock % 60 < 30) strcat(zoneOverTime, "2");
	else if(zoneOverClock % 60 < 40) strcat(zoneOverTime, "3");
	else if(zoneOverClock % 60 < 50) strcat(zoneOverTime, "4");
	else if(zoneOverClock % 60 < 60) strcat(zoneOverTime, "5");
	switch(zoneOverClock % 6){
		case 0: strcat(zoneOverTime, "0"); break;
		case 1: strcat(zoneOverTime, "1"); break;
		case 2: strcat(zoneOverTime, "2"); break;
		case 3: strcat(zoneOverTime, "3"); break;
		case 4: strcat(zoneOverTime, "4"); break;
		case 5: strcat(zoneOverTime, "5"); break;
		case 6: strcat(zoneOverTime, "6"); break;
		case 7: strcat(zoneOverTime, "7"); break;
		case 8: strcat(zoneOverTime, "8"); break;
		case 9: strcat(zoneOverTime, "9"); break;
	}
	VDP_drawText(zoneOverTime, OVER_Y + 9, 21);
	zoneOverClock--;
	if(zoneOverClock <= 0) {
		VDP_clearTextArea(OVER_X, OVER_Y, 21, 21);
		nextZone();
	}
}

void loadChromeGameOver(bool beatIt){
	XGM_stopPlay();
	loadedChromeGameOver = TRUE;
	VDP_drawText(beatIt ? "beat game!" : "game over!", 11 - 4, 9);
	VDP_drawText(currentScore > highScore ? "FINAL  SCORE" : "FINAL  SCORE", 10 - 5, 12);
	VDP_drawText(chromeScoreStr, 12 - 4, 14);
	// if(beatIt){
		VDP_drawText("special thanks", 9 - 4, 18);
		VDP_drawText("TOUHOU GAMEDEV DISCORD", 5 - 4, 20);
	// } else VDP_drawText("press any button", 8 - 4, 18);
	if(currentScore > highScore) highScore = currentScore;
	XGM_startPlayPCM(beatIt ? SFX_BEAT_GAME : SFX_GAME_OVER, 2, SOUND_PCM_CH2);
}

void loadChromeBeatGame(){
	// gameOver = TRUE;
	// loadChromeGameOver(TRUE);
}


// boss

#define BOSS_TILE_X 1
#define BOSS_TILE_Y 1
#define BOSS_TILE_COUNT 22
#define BOSS_TILE_PX BOSS_TILE_COUNT * 8
#define BOSS_I 10

void updateChromeBoss(){
	if(lastBossHealth != boss.health){
		if(lastBossHealth < boss.health){
			switch(boss.type){
				case 0: strcpy(bossName, "MIKE"); break;
				case 1: strcpy(bossName, "TSUKASA"); break;
				case 2: strcpy(bossName, "MEGUMU"); break;
				case 3: strcpy(bossName, "CHIMATA"); break;
			}
			VDP_drawText(bossName, BOSS_TILE_X, BOSS_TILE_Y + 1);
		}
		bossLimit = fix16Div(fix16Mul(fix16Div(FIX16(boss.health), FIX16(boss.max)), BOSS_TILE_PX), 8);
		for(s16 x = 0; x < BOSS_TILE_COUNT; x++){
			if(boss.health <= 1 || FIX16(x) >= bossLimit){
				VDP_clearText(x + BOSS_TILE_X, BOSS_TILE_Y, 1);
			}
		}
		for(s16 x = 0; x < BOSS_TILE_COUNT; x++){
			if(boss.health > 1 && FIX16(x) < bossLimit) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, BOSS_I), x + BOSS_TILE_X, BOSS_TILE_Y);
		}
		lastBossHealth = boss.health;
		if(lastBossHealth <= 0 && boss.health <= 0) VDP_clearText(BOSS_TILE_X, BOSS_TILE_Y + 1, 12);
	}
}


// loop

void loadChrome(){
	VDP_setTextPriority(1);
	VDP_loadTileSet(frame1.tileset, FRAME_I, DMA);
	VDP_loadTileSet(frame2.tileset, FRAME_I + 1, DMA);
	VDP_loadTileSet(bossBar.tileset, BOSS_I, DMA);
	drawFrame();
	updateChromeScore();
	loadChromeZone();
	loadChromeLives();
	loadChromeBombs();
	zoneOverClock = ZONE_OVER_CHROME_LIMIT;
}

void updateChrome(){
	if(zoneStarting) loadChrome();
	else {
		if(zoneOver){
			if(!loadedZoneOver) currentZone == 20 ? loadChromeBeatGame() : loadChromeZoneOver();
			updateChromeZoneOver();
		} else if(gameOver){
			if(!loadedChromeGameOver) loadChromeGameOver(FALSE);
		} else {
			updateChromePlayerLives();
			updateChromePlayerBombs();
			updateChromeBoss();
			if(chromeCurrentScore < currentScore) updateChromeScore();
		}
	}
}