#define YIN_SPEED 1
#define YIN_OFF 8

// movement

void moveYinHorizontal(s8 i){
	yins[i].pos.x += yins[i].flag1 ? 0 - YIN_SPEED : YIN_SPEED;
	if(yins[i].pos.x >= GAME_WIDTH - 10){
		yins[i].flag1 = TRUE;
		SPR_setHFlip(yins[i].image, 1);
	} else if(yins[i].pos.x <= 10){
		yins[i].flag1 = FALSE;
		SPR_setHFlip(yins[i].image, 0);
	}
}

void moveYinVertical(s8 i){
	yins[i].pos.y += yins[i].flag1 ? 0 - YIN_SPEED : YIN_SPEED;
	if(yins[i].pos.y >= 112){
		yins[i].flag1 = TRUE;
	} else if(yins[i].pos.y <= 56){
		yins[i].flag1 = FALSE;
	}
}


// shooting

Vect2D_f16 yinHone;

void yinPatternOne(s8 i){
	if(yins[i].clock % 80 == 40){
		struct bulletSpawner spawner = {
			.x = FIX16(yins[i].pos.x),
			.y = FIX16(yins[i].pos.y),
			.image = &imgSmallRedBullet
		};
		yinHone.x = spawner.x;
		yinHone.y = spawner.y;
		spawner.velocity = hone(yinHone, playerPos, FIX16(yinBulletSpeed), 0);
		spawnEnemyBullet(spawner, EMPTY);
		// XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnExplosion(yins[i].pos.x, yins[i].pos.y, FALSE);
	}
}

void yinPatternTwo(s8 i){
	if(yins[i].clock % 40 == 20){
		struct bulletSpawner spawner = {
			.x = FIX16(yins[i].pos.x),
			.y = FIX16(yins[i].pos.y),
			.image = yins[i].clock % 80 == 20 ? &imgBigRedBullet : &imgSmallRedBullet
		};
		if(yins[i].clock % 80 == 20) spawner.big = TRUE;
		yinHone.x = spawner.x;
		yinHone.y = spawner.y;
		spawner.velocity = hone(yinHone, playerPos, FIX16(yinBulletSpeed), 0);
		spawnEnemyBullet(spawner, EMPTY);
		// XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnExplosion(yins[i].pos.x, yins[i].pos.y, FALSE);
	}
}

void yinPatternThree(s8 i, s8 count){
	if(yins[i].clock % 80 >= 20 && yins[i].clock % 80 < 30 && yins[i].clock % 5 == 0){
		struct bulletSpawner spawner = {
			.x = FIX16(yins[i].pos.x),
			.y = FIX16(yins[i].pos.y),
		};
		yinHone.x = spawner.x;
		yinHone.y = spawner.y;
		spawner.ints[0] = 64;
		for(u8 b = 0; b < count; b++){
			spawner.velocity = hone(yinHone, playerPos, FIX16(yinBulletSpeed), spawner.ints[0]);
			if(yins[i].horizontal) spawner.velocity.y = FIX16(yinBulletSpeed);
			else if(yins[i].horizontal) spawner.velocity.x = FIX16(yinBulletSpeed);
			spawner.image = yins[i].clock % 8 == 0 ? (b % 2 == 0 ? &imgBigRedBullet : &imgSmallRedBullet) : (b % 2 == 0 ? &imgSmallRedBullet : &imgBigRedBullet);
			spawnEnemyBullet(spawner, EMPTY);
			spawner.ints[0] += 16;
		}
		// // XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnExplosion(yins[i].pos.x, yins[i].pos.y, FALSE);
	}
}

void yinShoot(s8 i){
	if((!yins[i].horizontal && !yins[i].last && yins[i].clock % 240 < 80) ||
		(yins[i].horizontal && yins[i].clock % 240 >= 80 && yins[i].clock % 240 < 160) ||
		(yins[i].last && yins[i].clock % 240 >= 160)){
		if(currentZone == 1) yinPatternOne(i);
		else if(currentZone < 5) yinPatternTwo(i);
		else if(currentZone < 10) yinPatternThree(i, 1);
		else if(currentZone < 15) yinPatternThree(i, 2);
		else yinPatternThree(i, 3);
	}
}


// loop

void loadYins(){
	if(currentZone % 5 == 1 && currentZone > 1) yinBulletSpeed++;
	for(s16 i = 0; i < YIN_COUNT; i++){
		yins[i].pos.x = i == 0 ? YIN_OFF : (i == 2 ? (GAME_WIDTH - YIN_OFF) : GAME_WIDTH / 2);
		yins[i].pos.y = i % 2 == 0 ? (i == 0 ? 64 : 96) : YIN_OFF;
		yins[i].speed = 20;
		yins[i].horizontal = i % 2 == 1;
		yins[i].last = i % 3 == 2;
		yins[i].clock = 0;
		yins[i].image = SPR_addSprite(&imgYin, yins[i].pos.x, yins[i].pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
		if(yins[i].last) yins[i].flag1 = TRUE;
		SPR_setDepth(yins[i].image, 1);
	}
}

void resetYins(){
	for(s16 i = 0; i < YIN_COUNT; i++){
		yins[i].pos.x = i == 0 ? 10 : (i == 2 ? (256 - 10) : 128);
		yins[i].pos.y = i % 2 == 0 ? (i == 0 ? 64 : 96) : 24;
		yins[i].clock = 0;
		SPR_releaseSprite(yins[i].image);
	}
}

void updateYins(){
	for(s16 i = 0; i < YIN_COUNT; i++){
		if(yins[i].clock % yins[i].speed == 0) yins[i].horizontal ? moveYinHorizontal(i) : moveYinVertical(i);
		else if(yins[i].clock % yins[i].speed == 1) SPR_setPosition(yins[i].image, yins[i].pos.x - YIN_OFF, yins[i].pos.y - YIN_OFF);
		if(yins[i].clock % 5 == 0 && yins[i].speed > 2) yins[i].speed--;
		if(zoneOver) SPR_setVisibility(yins[i].image, HIDDEN);
		else if(!gameOver) yinShoot(i);
		if(!zoneOver) yins[i].clock++;
		if(yins[i].clock >= 240) yins[i].clock = 0;
	}
}