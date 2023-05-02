#define PLAYER_LIMIT_LEFT FIX16(4)
#define PLAYER_LIMIT_RIGHT fix16Sub(FIX16(GAME_WIDTH), PLAYER_LIMIT_LEFT)
#define PLAYER_LIMIT_TOP PLAYER_LIMIT_LEFT
#define PLAYER_LIMIT_BOTTOM fix16Sub(FIX16(GAME_HEIGHT), PLAYER_LIMIT_LEFT)

#define MOVEMENT_NORMALIZE FIX16(0.707)
#define MOVEMENT_NORMALIZE_NEG FIX16(-0.707)
#define MOVEMENT_POS FIX16(1)
#define MOVEMENT_NEG FIX16(-1)

#define PLAYER_SPEED FIX16(3.5)
#define PLAYER_SPEED_FOCUS FIX16(2.25)

#define PLAYER_INIT_X FIX16(GAME_WIDTH / 2)
#define PLAYER_INIT_Y FIX16(GAME_HEIGHT - 32)

#define PLAYER_SHOT_INTERVAL 10
#define PLAYER_BULLET_SPEED FIX16(8 * 4)

#define PLAYER_BULLET_UP_LIMIT FIX16(8)
#define PLAYER_BULLET_DOWN_LIMIT FIX16(GAME_HEIGHT + 16)

#define PLAYER_BULLET_DUMP_X GAME_WIDTH + 16
#define PLAYER_BULLET_DUMP_Y GAME_HEIGHT + 16

#define RECOVER_INTERVAL 30
#define RECOVER_INTERVAL_HALF 15
#define RECOVER_MAX RECOVER_INTERVAL * 6

#define BOMB_LIMIT 200
#define BOMB_INTERVAL 10

Sprite* playerSprite;

Vect2D_f16 playerVelocity;

s16 playerShotClock, recoverClock, bombClock, playerMoveClock;


void updatePlayerBounds(){
	if(playerPos.x < PLAYER_LIMIT_LEFT) playerPos.x = PLAYER_LIMIT_LEFT;
	else if(playerPos.x > PLAYER_LIMIT_RIGHT) playerPos.x = PLAYER_LIMIT_RIGHT;
	if(playerPos.y < PLAYER_LIMIT_TOP) playerPos.y = PLAYER_LIMIT_TOP;
	else if(playerPos.y > PLAYER_LIMIT_BOTTOM) playerPos.y = PLAYER_LIMIT_BOTTOM;
}

void updatePlayerMove(){
	playerVelocity.x = FIX16(0);
	playerVelocity.y = FIX16(0);
	if(controls.left){
		if(controls.down){
			playerVelocity.x = MOVEMENT_NORMALIZE_NEG;
			playerVelocity.y = MOVEMENT_NORMALIZE;
		}
		else if(controls.up){
			playerVelocity.x = MOVEMENT_NORMALIZE_NEG;
			playerVelocity.y = MOVEMENT_NORMALIZE_NEG;
		}
		else playerVelocity.x = MOVEMENT_NEG;
	}
	else if(controls.right){
		if(controls.down){
			playerVelocity.x = MOVEMENT_NORMALIZE;
			playerVelocity.y = MOVEMENT_NORMALIZE;
		}
		else if(controls.up){
			playerVelocity.x = MOVEMENT_NORMALIZE;
			playerVelocity.y = MOVEMENT_NORMALIZE_NEG;
		}
		else playerVelocity.x = MOVEMENT_POS;
	}
	else if(controls.up || controls.down) playerVelocity.y = controls.up ? MOVEMENT_NEG : MOVEMENT_POS;
	playerPos.x = fix16Add(playerPos.x, fix16Mul(playerVelocity.x, controls.b ? PLAYER_SPEED_FOCUS : PLAYER_SPEED));
	playerPos.y = fix16Add(playerPos.y, fix16Mul(playerVelocity.y, controls.b ? PLAYER_SPEED_FOCUS : PLAYER_SPEED));
	updatePlayerBounds();
	SPR_setPosition(playerSprite, fix16ToInt(playerPos.x) - 11, fix16ToInt(playerPos.y) - 16);
}


// shooting

void spawnPlayerBullet(){
	static s16 i = 0;
	for(s16 j = 0; j < PLAYER_BULLET_LIMIT; j++) if(!playerBullets[j].active) i = j;
	playerBullets[i].active = TRUE;
	playerBullets[i].pos.x = playerPos.x;
	playerBullets[i].pos.y = fix16Sub(playerPos.y, FIX16(8));
	// XGM_startPlayPCM(SFX_PLAYER_SHOT, 0, SOUND_PCM_CH4);
}

void removePlayerBullet(s16 i){
	playerBullets[i].pos.x = FIX16(PLAYER_BULLET_DUMP_X);
	playerBullets[i].pos.y = FIX16(PLAYER_BULLET_DUMP_Y);
	SPR_setPosition(playerBullets[i].image, PLAYER_BULLET_DUMP_X, PLAYER_BULLET_DUMP_Y);
	playerBullets[i].active = FALSE;
}

void updatePlayerBullets(){
	for(s16 i = 0; i < PLAYER_BULLET_LIMIT; i++) if(playerBullets[i].active){
		playerBullets[i].pos.y = fix16Sub(playerBullets[i].pos.y, PLAYER_BULLET_SPEED);
		SPR_setPosition(playerBullets[i].image, fix16ToInt(playerBullets[i].pos.x) - 12, fix16ToInt(playerBullets[i].pos.y) - 8);
		if(playerBullets[i].pos.y <= PLAYER_BULLET_UP_LIMIT || playerBullets[i].pos.y >= PLAYER_BULLET_DOWN_LIMIT || zoneOver) removePlayerBullet(i);
	}
}

void updatePlayerShot(){
	if(playerShotClock >= PLAYER_SHOT_INTERVAL && controls.a) playerShotClock = 0;
	if(playerShotClock == 0 && !zoneOver) spawnPlayerBullet();
	playerShotClock++;
	if(playerShotClock >= 600) playerShotClock = PLAYER_SHOT_INTERVAL;
}


// bomb

void spawnBomb(){
	killBullets = TRUE;
	SND_startPlayPCM_XGM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
	spawnExplosion(random() % GAME_WIDTH, random() % GAME_HEIGHT, TRUE);
}

void updatePlayerBomb(){
	if(bombing){
		if(bombClock % BOMB_INTERVAL == 0) spawnBomb();
		bombClock++;
		if(bombClock >= BOMB_LIMIT){
			bombClock = 0;
			bombing = FALSE;
		}
	} else if(controls.c && playerBombs > 0){
		bombing = TRUE;
		playerBombs -= 1;
	}
}


// get hit

void updatePlayerHit(){
	if(hitPlayer){
		hitPlayer = FALSE;
		playerRecovering = TRUE;
		playerPos.x = PLAYER_INIT_X;
		playerPos.y = PLAYER_INIT_Y;
		playerLives -= 1;
		noMiss = FALSE;
		XGM_startPlayPCM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 1, SOUND_PCM_CH4);
		spawnExplosion(fix16ToInt(playerPos.x), fix16ToInt(playerPos.y), FALSE);
		// if(playerLives < 0) playerLives = 0;
		if(playerLives < 0) gameOver = TRUE;
		else playerBombs = 3;
	}
	if(!gameOver && playerRecovering){
		if(recoverClock % RECOVER_INTERVAL == 0) SPR_setVisibility(playerSprite, HIDDEN);
		else if(recoverClock % RECOVER_INTERVAL == RECOVER_INTERVAL_HALF) SPR_setVisibility(playerSprite, VISIBLE);
		recoverClock++;
		if(recoverClock >= RECOVER_MAX){
			SPR_setVisibility(playerSprite, VISIBLE);
			recoverClock = 0;
			playerRecovering = FALSE;
		}
	}
}


// demo
s32 demoDist;
s16 demoAngle;
bool foundCentipede, foundBullet;
static void updateDemoMove(){
	playerVelocity.x = FIX16(0);
	playerVelocity.y = FIX16(0);
	// boss
	if(boss.active){
		if(boss.pos.x < fix16Sub(playerPos.x, FIX16(8)) || boss.pos.x >= fix16Add(playerPos.x, FIX16(8))){
			if(boss.pos.x < playerPos.x) demoAngle = 512;
			else demoAngle = 0;
			playerVelocity.x = cosFix16(demoAngle);
			playerVelocity.y = sinFix16(demoAngle);
		}
	}

	// centipede
	else {
		foundCentipede = FALSE;
		for(u8 i = 0; i < centipedeCount; i++) if(centipedes[i].active && !foundCentipede) {
			foundCentipede = TRUE;
			demoDist = getApproximatedDistance(
				fix16ToFix32(fix16Sub(playerPos.x, centipedes[i].pos.x)),
				fix16ToFix32(fix16Sub(playerPos.y, centipedes[i].pos.y)));
			if(demoDist <= FIX32(48)){
				if(centipedes[i].pos.y < playerPos.y && playerPos.y > FIX16(GAME_HEIGHT - (random() % 16 + 32)))
					demoAngle = centipedes[i].pos.x < playerPos.x ? 128 : 384;
				else
					demoAngle = centipedes[i].pos.x < playerPos.x ? 896 : 640;
				playerVelocity.x = cosFix16(demoAngle);
				playerVelocity.y = sinFix16(demoAngle);
			}

			else if(playerMoveClock % 30 < 20) {
				if(centipedes[i].pos.x < playerPos.x) demoAngle = 512;
				else demoAngle = 0;
				playerVelocity.x = cosFix16(demoAngle);
				playerVelocity.y = sinFix16(demoAngle);
			}

			else if(playerMoveClock % 30 < 20){
				if(playerPos.x < FIX16(48)) demoAngle = 0;
				else if(playerPos.x > FIX16(GAME_WIDTH - 48)) demoAngle = 512;
				playerVelocity.x = cosFix16(demoAngle);
				playerVelocity.y = sinFix16(demoAngle);
			}
		}
	}

	// bullet
	if(!foundCentipede){
		foundBullet = FALSE;
		for(s16 j = 0; j < ENEMY_BULLET_LIMIT; j++) if(bullets[j].active && !foundBullet){
		demoDist = getApproximatedDistance(
			fix16ToFix32(fix16Sub(playerPos.x, bullets[j].pos.x)),
			fix16ToFix32(fix16Sub(playerPos.y, bullets[j].pos.y)));
		if(demoDist <= FIX32(48)){
			foundBullet = TRUE;
			if(bullets[j].pos.y < playerPos.y && playerPos.y > FIX16(GAME_HEIGHT - (random() % 16 + 32)))
				demoAngle = playerPos.x < FIX16(GAME_WIDTH / 2) ? 128 : 384;
			else{
				if(playerPos.y >= FIX16(GAME_WIDTH / 4 * 3))
					demoAngle = playerPos.x < FIX16(GAME_WIDTH / 2) ? 896 : 640;
				else 
					demoAngle = playerPos.x < FIX16(GAME_WIDTH / 2) ? 128 : 384;
			}
			playerVelocity.x = cosFix16(demoAngle);
			playerVelocity.y = sinFix16(demoAngle);
		}
	}
	}

	playerPos.x = fix16Add(playerPos.x, fix16Mul(playerVelocity.x, PLAYER_SPEED));
	playerPos.y = fix16Add(playerPos.y, fix16Mul(playerVelocity.y, PLAYER_SPEED));
	updatePlayerBounds();
	SPR_setPosition(playerSprite, fix16ToInt(playerPos.x) - 11, fix16ToInt(playerPos.y) - 16);
	playerMoveClock++;
	if(playerMoveClock >= CLOCK_LIMIT) playerMoveClock = 0;
}
static void updateDemoShot(){
	if(playerShotClock >= PLAYER_SHOT_INTERVAL) playerShotClock = 0;
	if(playerShotClock == 0) spawnPlayerBullet();
	playerShotClock++;
	if(playerShotClock >= CLOCK_LIMIT) playerShotClock = PLAYER_SHOT_INTERVAL;

}


// loop


void loadPlayer(){
	playerShotClock = PLAYER_SHOT_INTERVAL;
	playerPos.x = PLAYER_INIT_X;
	playerPos.y = PLAYER_INIT_Y;
	playerSprite = SPR_addSprite(&imgPlayer, playerPos.x, playerPos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
	SPR_setDepth(playerSprite, 4);
	for(s16 i = 0; i < PLAYER_BULLET_LIMIT; i++){
		playerBullets[i].image = SPR_addSprite(&imgBullet, PLAYER_BULLET_DUMP_X, PLAYER_BULLET_DUMP_Y, TILE_ATTR(PAL1, 1, FALSE, FALSE));
		SPR_setDepth(playerBullets[i].image, 4);
	}
}

void resetPlayer(){
	playerPos.x = PLAYER_INIT_X;
	playerPos.y = PLAYER_INIT_Y;
	SPR_releaseSprite(playerSprite);
	for(s16 i = 0; i < PLAYER_BULLET_LIMIT; i++) if(playerBullets[i].active){
		playerBullets[i].active = FALSE;
		SPR_releaseSprite(playerBullets[i].image);
	}
}

void updatePlayer(){
	if(zoneStarting) loadPlayer();
	else {
		updatePlayerBullets();
		if(!gameOver && !paused){
			if(zoneOver) SPR_setVisibility(playerSprite, HIDDEN);
			else {
				if(demo) updateDemoMove();
				else updatePlayerMove();
				updatePlayerHit();
			}
			if(demo) updateDemoShot();
			else updatePlayerShot();
			updatePlayerBomb();
			if(demo && (controls.a || controls.b || controls.c || controls.start)){
				resetGame();
				demo = FALSE;
			}
		} else if(gameOver) resetPlayer();
	}
}