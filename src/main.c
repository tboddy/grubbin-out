#include <genesis.h>
#include <resources.h>

#include "structs.h"
#include "global.h"
#include "start.h"
#include "controls.h"
#include "chrome.h"
#include "background.h"
#include "powerups.h"
#include "centipede.h"
#include "pod.h"
#include "yin.h"
#include "boss.h"
#include "enemies.h"
#include "player.h"
#include "explosion.h"

void loadResources(){
	VDP_loadFont(font.tileset, DMA);
	VDP_loadTileSet(half.tileset, HALF_TILE, DMA);
	VDP_loadTileSet(least.tileset, LEAST_TILE, DMA);
	VDP_loadTileSet(full.tileset, FULL_TILE, DMA);
	VDP_setPalette(PAL1, font.palette -> data);
	VDP_setPalette(PAL2, shadow.palette -> data);
	VDP_setPalette(PAL3, desat.palette -> data);
	VDP_setTextPalette(1);

	XGM_setPCM(SFX_MENU_SELECT, sfxMenuSelect, sizeof(sfxMenuSelect)); // shit
	XGM_setPCM(SFX_MENU_CHOOSE, sfxMenuChoose, sizeof(sfxMenuChoose));
	XGM_setPCM(SFX_START_GAME, sfxStartGame, sizeof(sfxStartGame));
	XGM_setPCM(SFX_PLAYER_SHOT, sfxPlayerShot, sizeof(sfxPlayerShot));
	XGM_setPCM(SFX_ZONE_OVER, sfxZoneOver, sizeof(sfxZoneOver));

	XGM_setPCM(SFX_BULLET_1, sfxBullet1, sizeof(sfxBullet1));

	XGM_setPCM(SFX_EXPLOSION_1, sfxExplosion1, sizeof(sfxExplosion1));
	XGM_setPCM(SFX_EXPLOSION_2, sfxExplosion2, sizeof(sfxExplosion2));
	XGM_setPCM(SFX_EXPLOSION_3, sfxExplosion3, sizeof(sfxExplosion3));

	XGM_setPCM(SFX_GAME_OVER, sfxGameOver, sizeof(sfxGameOver));
	XGM_setPCM(SFX_BEAT_GAME, sfxBeatGame, sizeof(sfxBeatGame));

}

void loadGame(){
	// demo = TRUE;
	currentZone = 4;
	zoneOver = TRUE;
	started = TRUE;
	zoneStarting = TRUE;
	gameStarting = TRUE;
	noMiss = TRUE;
	yinBulletSpeed = 4;
	podBulletSpeed = 3;
	currentScore = 0;
	loadExplosion();
	// nextZone();
	// XGM_startPlay(&bgmStage1);
}

void resetGame(){
	if(boss.active) finishBoss();
	for(s16 i = 0; i < ENEMY_BULLET_LIMIT; i++) if(bullets[i].active) destroyEnemyBullet(i);
	resetYins();
	resetPod();
	resetCentipede();
	VDP_clearPlane(BG_A, TRUE);
	VDP_clearPlane(BG_B, TRUE);
	DMA_waitCompletion();
	startClock = 0;
	demoStartClock = 0;
	demoClock = 0;
	startClock = 0;
	zoneOver = FALSE;
	started = FALSE;
	gameOverClock = 0;
	bombClock = 0;
	recoverClock = 0;
	playerShotClock = PLAYER_SHOT_INTERVAL;
	paused = FALSE;
	gameOver = FALSE;
	zoneOverClock = 0;
	loadedChromeGameOver = FALSE;
	loadedZoneOver = FALSE;
	killBulletsClock = 0;
	zoneStarting = FALSE;
	gameClock = 0;
	playerLives = 2;
	explosionClock = 0;
	explosionClockPlayerShot = 0;
	playerBombs = 3;
	boss.type = 0;
	noMiss = TRUE;
	SPR_reset();
	loadStart();
	// SYS_reset();
}


void updateGame(){
	if(started && !paused){
		updateBackground();
		updatePowerup();
		updateEnemies();
		updatePlayer();
		updateExplosion();
		gameClock++;
		if(gameClock >= CLOCK_LIMIT) gameClock = 0;
	}
	updateChrome();
	if(demo) demoClock++;
	if(zoneStarting) zoneStarting = FALSE;
	if(gameStarting) gameStarting = FALSE;
	if(doZoneStart){
		zoneStarting = TRUE;
		doZoneStart = FALSE;
	}
	if(!gameOver && started && gameClock >= 15 && !zoneOver){
		if(controls.start && !pausing){
			pausing = TRUE;
			paused = paused ? FALSE : TRUE;
		} else if(!controls.start && pausing) pausing = FALSE;
	} else if(gameOver){
		if(gameOverClock < 600) gameOverClock++;
		if(gameOverClock >= 120 && (controls.a || controls.b || controls.c || controls.start)) resetGame();
	}
	if(demo && demoClock >= 1800) resetGame();
};

void nextZone(){
	if(currentZone == 21){
		resetGame();
	} else {
		SPR_reset();
		loadExplosion();
		// drawBackground();
		resetEnemies();
		resetPlayer();
		doZoneStart = TRUE;
		zoneOver = FALSE;
		loadedZoneOver = FALSE;
		gameClock = -5;
		noMiss = TRUE;
		if(currentZone % 5 == 0) spawnBoss();
		// XGM_startPlayPCM(SFX_START_GAME, 1, SOUND_PCM_CH2);
		if(currentZone == 11) XGM_startPlay(&bgmStage2);
	}
}

int main() {
	XGM_setLoopNumber(-1);
	JOY_init();
	JOY_setEventHandler(&updateControls);
	loadResources();
	SPR_init(0, 0, 0);
	VDP_setScreenWidth256();
	// loadStart();
	loadGame();
	playerLives = 2;
	playerBombs = 3;
	while(1){
		started ? updateGame() : updateStart();
		SPR_update();
		SYS_doVBlankProcess();
	}
	return (0);
}