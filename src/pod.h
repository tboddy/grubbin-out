#define POD_DUMP_X FIX16(GAME_WIDTH + 32)
#define POD_DUMP_Y FIX16(GAME_HEIGHT + 32)

#define POD_TIME_LIMIT 150

#define POD_OFF 8

Vect2D_f16 currentPodPos[POD_COUNT];


// spawn

void spawnPod(struct podSpawner spawner){
	s16 i = -1;
	for(s16 h = 0; h < POD_COUNT; h++) if(!pods[h].active && i == -1) i = h;
	if(i > -1){
		pods[i].active = TRUE;
		pods[i].pos.x = spawner.x;
		pods[i].pos.y = spawner.y;
		pods[i].clock = 0;
		pods[i].anim = 0;
		pods[i].visible = FALSE;
		pods[i].image = SPR_addSprite(&imgBomb, fix16ToInt(pods[i].pos.x) - POD_OFF, fix16ToInt(pods[i].pos.y) - POD_OFF, TILE_ATTR(PAL1, 0, FALSE, FALSE));
	}
}


// shoot

static void podPatternOne(s16 i){
	struct bulletSpawner spawner = {
		.x = pods[i].pos.x,
		.y = pods[i].pos.y,
		.image = &imgBigBlueBullet,
		.big = TRUE,
		.angle = random() % 1024,
		.speed = FIX16(podBulletSpeed)
	};
	for(s8 b = 0; b < 3; b++){
		spawnEnemyBullet(spawner, EMPTY);
		spawner.angle += 341;
	}
	// XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
}

static void podPatternTwo(s8 i, s8 mod, s16 aMod){

	struct bulletSpawner spawner = {
		.x = pods[i].pos.x,
		.y = pods[i].pos.y,
		.image = &imgBigBlueBullet,
		.big = TRUE,
		.angle = random() % 1024,
		.speed = FIX16(podBulletSpeed)
	};
	for(s8 b = 0; b < 3; b++){
		spawnEnemyBullet(spawner, EMPTY);
		spawner.angle += aMod;
	}
	spawner.speed = FIX16(podBulletSpeed + 1);
	spawner.image = &imgSmallBlueBullet;
	spawner.big = FALSE;
	spawner.angle += aMod / 2;
	for(s8 b = 0; b < 3 + mod; b++){
		spawnEnemyBullet(spawner, EMPTY);
		spawner.angle += aMod;
	}
}

static void shootPod(s16 i){
	if(currentZone == 1) podPatternOne(i);
	else if(currentZone < 5) podPatternTwo(i, 0, 342);
	else if(currentZone < 10) podPatternTwo(i, 1, 256);
	else podPatternTwo(i, 2, 204);
}


// die

static void killPod(s16 i){
	pods[i].active = FALSE;
	pods[i].pos.x = POD_DUMP_X;
	pods[i].pos.y = POD_DUMP_Y;
	SPR_releaseSprite(pods[i].image);
}


// loop

void loadPod(){
	if(currentZone % 5 == 1 && currentZone > 1) podBulletSpeed++;
}

void resetPod(){
	for(s16 i = 0; i < POD_COUNT; i++){
		pods[i].active = FALSE;
		pods[i].clock = 0;
		SPR_releaseSprite(pods[i].image);
	}
}

void updatePod(){
	for(s16 i = 0; i < POD_COUNT; i++) if(pods[i].active) {
		if(pods[i].clock >= POD_TIME_LIMIT){
			if(!gameOver) shootPod(i);
			killPod(i);
			spawnExplosion(fix16ToInt(pods[i].pos.x), fix16ToInt(pods[i].pos.y), FALSE);
			// XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
		} else {
			if(pods[i].clock % 60 == 0){
				 SPR_setAnim(pods[i].image, pods[i].anim);
				 pods[i].anim++;
			}
			if(!pods[i].visible){
				pods[i].visible = TRUE;
				SPR_setPosition(pods[i].image, fix16ToInt(pods[i].pos.x) - POD_OFF, fix16ToInt(pods[i].pos.y) - POD_OFF);
			}
		}
		if(zoneOver) killPod(i);
		pods[i].clock++;
	}
}