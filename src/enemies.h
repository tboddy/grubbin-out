
#define ENEMY_BULLET_COLLISION_CHECK FIX16(8)


s32 enemyBulletCollisionDistance;
s16 killBulletsClock, enemyBulletCount;


// bullets

Image *bulletImage;

void spawnEnemyBullet(struct bulletSpawner spawner, void(*updater)){
	s16 i = -1;
	for(s16 h = 0; h < ENEMY_BULLET_LIMIT; h++) if(!bullets[h].active && i == -1) i = h;
	if(i > -1 && !gameOver){
		bullets[i].active = TRUE;
		bullets[i].grazed = FALSE;
		bullets[i].pos.x = spawner.x;
		bullets[i].pos.y = spawner.y;
		bullets[i].speed = spawner.speed;
		bullets[i].angle = spawner.angle;
		bullets[i].big = spawner.big;
		if(spawner.velocity.x || spawner.velocity.y){
			bullets[i].velocity.x = spawner.velocity.x;
			bullets[i].velocity.y = spawner.velocity.y;
		} else {
			bullets[i].velocity.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			bullets[i].velocity.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		bullets[i].updater = updater;
		bullets[i].clock = 0;
		bullets[i].image = SPR_addSprite(spawner.image, bullets[i].pos.x, bullets[i].pos.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
		bullets[i].xOffset = bullets[i].big ? 8 : 4;
		bullets[i].yOffset = bullets[i].xOffset;
		for(u8 i = 0; i < 8; i++) bullets[i].ints[i] = spawner.ints[i];
		SPR_setDepth(bullets[i].image, spawner.top ? 1 : 2);
		SPR_setPosition(bullets[i].image, fix16ToInt(bullets[i].pos.x) - bullets[i].xOffset, fix16ToInt(bullets[i].pos.y) - bullets[i].yOffset);
	}
}

void checkEnemyBulletCollision(s16 i){
	enemyBulletCollisionDistance = getApproximatedDistance(
		fix16ToFix32(playerPos.x) - fix16ToFix32(bullets[i].pos.x),
		fix16ToFix32(playerPos.y) - fix16ToFix32(bullets[i].pos.y));
	if(enemyBulletCollisionDistance < intToFix32(bullets[i].xOffset) && !playerRecovering){
		spawnExplosion(fix16ToInt(bullets[i].pos.x), fix16ToInt(bullets[i].pos.y), FALSE);
		destroyEnemyBullet(i);
		if(!demo){
			hitPlayer = TRUE;
			killBullets = TRUE;
			// SND_startPlayPCM_XGM(SFX_EXPLOSION2, 15, SOUND_PCM_CH2);
		}
	} else if(!bullets[i].grazed){
		bullets[i].grazed = TRUE;
		currentScore += 50;
		// SND_startPlayPCM_XGM(SFX_GRAZE, 15, SOUND_PCM_CH2);
	}
}

void updateEnemyBullet(s16 i){
	bullets[i].pos.x = fix16Add(bullets[i].pos.x, bullets[i].velocity.x);
	bullets[i].pos.y = fix16Add(bullets[i].pos.y, bullets[i].velocity.y);
	bullets[i].updater(i);
	enemyBulletCount++;
	bullets[i].clock++;
	if(bullets[i].pos.x < FIX16(-bullets[i].xOffset) ||
		bullets[i].pos.x > FIX16(GAME_WIDTH + bullets[i].xOffset) ||
		bullets[i].pos.y < FIX16(-bullets[i].yOffset) ||
		bullets[i].pos.y > FIX16(GAME_HEIGHT + bullets[i].yOffset) ||
		killBullets || bullets[i].clock >= 600 || zoneOver){
		destroyEnemyBullet(i);
	}
}

void destroyEnemyBullet(s16 i){
	SPR_releaseSprite(bullets[i].image);
	bullets[i].active = FALSE;
}

void drawEnemyBullet(s16 i){
	SPR_setPosition(bullets[i].image,
		fix16ToInt(bullets[i].pos.x) - bullets[i].xOffset,
		fix16ToInt(bullets[i].pos.y) - bullets[i].yOffset);
	if(bullets[i].pos.y > fix16Sub(playerPos.y, ENEMY_BULLET_COLLISION_CHECK) &&
		bullets[i].pos.y < fix16Add(playerPos.y, ENEMY_BULLET_COLLISION_CHECK) &&
		bullets[i].pos.x > fix16Sub(playerPos.x, ENEMY_BULLET_COLLISION_CHECK) &&
		bullets[i].pos.x < fix16Add(playerPos.x, ENEMY_BULLET_COLLISION_CHECK)){
		checkEnemyBulletCollision(i);
	}
}




// kill bullets dramatically

void updateKillBullets(){
	if(enemyBulletCount >= ENEMY_BULLET_LIMIT) killBullets = TRUE;
	if(killBullets){
		if(killBulletsClock % 4 == 0 && killBulletsClock < 20 && !bombing)
			spawnExplosion(32 + random() % 192, 32 + random() % 128, FALSE);
		killBulletsClock++;
		if(killBulletsClock >= 30){
			killBullets = FALSE;
			killBulletsClock = 0;
		}
	}
}


// loop

void loadEnemies(){
	loadYins();
	loadCentipede();
	loadPod();
}

void resetEnemies(){
	resetYins();
	resetCentipede();
	resetPod();
	for(s16 i = 0; i < ENEMY_BULLET_LIMIT; i++) destroyEnemyBullet(i);
}

void updateEnemies(){
	if(zoneStarting && !boss.active) loadEnemies();
	else {
		if(boss.active) updateBoss();
		else {
			updateYins();
			updateCentipede();
			updatePod();
		}
		if(gameClock % 2 == 0){
			enemyBulletCount = 0;
			for(s16 i = 0; i < ENEMY_BULLET_LIMIT; i++) if(bullets[i].active) updateEnemyBullet(i);
			updateKillBullets();
		} else for(s16 i = 0; i < ENEMY_BULLET_LIMIT; i++) if(bullets[i].active) drawEnemyBullet(i);
	}
}