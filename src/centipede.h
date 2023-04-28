#define CENTIPEDE_OFF 8
#define CENTIPEDE_LIMIT_LEFT FIX16(8)
#define CENTIPEDE_LIMIT_RIGHT FIX16(GAME_WIDTH - 8)
#define CENTIPEDE_LIMIT_TOP FIX16(8)
#define CENTIPEDE_LIMIT_BOTTOM FIX16(GAME_HEIGHT - 8)

#define CENTIPEDE_COLLIDE_OFFSET FIX16(8)
#define CENTIPEDE_BULLET_OFFSET FIX16(8)
#define CENTIPEDE_PLAYER_OFFSET FIX16(2)
#define CENTIPEDE_POD_OFFSET FIX32(4)

#define CENTIPEDE_HIT 8


s32 centipedePodCheck;
u16 centipedeClock;

bool centipedeCollided, zoneOverCheck;

#define CENTIPEDE_INIT_Y FIX16(-8)
#define CENTIPEDE_INIT_X FIX16(GAME_WIDTH / 3)

static void spawnCentipede(s16 i){
	centipedes[i].active = TRUE;
	// centipedes[i].health = 5;
	centipedes[i].health = 80;
	centipedes[i].speed = FIX16(2);



	if(currentZone >= 15) centipedes[i].angle = 128;
	else if(currentZone >= 10) centipedes[i].angle = 96;
	else if(currentZone >= 5) centipedes[i].angle = 64;
	else centipedes[i].angle = 32;

	centipedes[i].seen = FALSE;
	centipedes[i].clock = 0;
	centipedes[i].velocity.x = fix16Mul(cosFix16(centipedes[i].angle), centipedes[i].speed);
	centipedes[i].velocity.y = fix16Mul(sinFix16(centipedes[i].angle), centipedes[i].speed);
	centipedes[i].pos.x = fix16Sub(CENTIPEDE_INIT_X, fix16Mul(cosFix16(centipedes[i].angle), FIX16(i * 16)));
	centipedes[i].pos.y = fix16Sub(CENTIPEDE_INIT_Y, fix16Mul(sinFix16(centipedes[i].angle), FIX16(i * 16)));
	centipedes[i].image = SPR_addSprite(&imgGumdrop, fix16ToInt(centipedes[i].pos.x), fix16ToInt(centipedes[i].pos.y), TILE_ATTR(PAL1, 0, FALSE, FALSE));
	SPR_setDepth(centipedes[i].image, 3);
	SPR_setFrame(centipedes[i].image, i % 2);
	SPR_setAnim(centipedes[i].image, 0);
}


// movement

static void moveCentipede(s16 i){
	centipedes[i].pos.x = fix16Add(centipedes[i].pos.x, centipedes[i].velocity.x);
	centipedes[i].pos.y = fix16Add(centipedes[i].pos.y, centipedes[i].velocity.y);
	if(centipedes[i].seen){
		if(centipedes[i].pos.x >= CENTIPEDE_LIMIT_RIGHT || centipedes[i].pos.x <= CENTIPEDE_LIMIT_LEFT) centipedes[i].velocity.x *= -1;
		if(centipedes[i].pos.y >= CENTIPEDE_LIMIT_BOTTOM || centipedes[i].pos.y <= CENTIPEDE_LIMIT_TOP) centipedes[i].velocity.y *= -1;
	} else if(centipedes[i].pos.y > CENTIPEDE_LIMIT_TOP) centipedes[i].seen = TRUE;
}


// collision against centipede

static void hitCentipede(s16 i){
	spawnExplosion(fix16ToInt(centipedes[i].pos.x), fix16ToInt(centipedes[i].pos.y), TRUE);
	centipedes[i].health -= CENTIPEDE_HIT;
	// XGM_startPlayPCM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 1, SOUND_PCM_CH4);
	currentScore += 15;
	if(centipedes[i].health < 0){
		struct podSpawner pSpawn = {
			.x = centipedes[i].pos.x,
			.y = centipedes[i].pos.y
		};
		spawnPod(pSpawn);
		destroyCentipede(i);
		currentScore += currentZone >= 10 ? 1000 : 500;
		// XGM_startPlayPCM(SFX_EXPLOSION_3, 2, SOUND_PCM_CH4);
	}
	else if(centipedes[i].health <= 20)
		SPR_setAnim(centipedes[i].image, 2);
	else if(centipedes[i].health <= 40)
		SPR_setAnim(centipedes[i].image, 1);
}

void turnCentipede(s16 i, s16 j){
	centipedes[i].velocity.x *= -1;
	centipedes[i].velocity.y *= -1;
}

void collideCentipede(s16 i){
	// against pods
	for(s16 j = 0; j < POD_COUNT; j++) if(pods[j].active) {
		centipedePodCheck = getApproximatedDistance(
			fix16ToFix32(centipedes[i].pos.x) - fix16ToFix32(pods[j].pos.x),
			fix16ToFix32(centipedes[i].pos.y) - fix16ToFix32(pods[j].pos.y));
		if(centipedePodCheck < CENTIPEDE_POD_OFFSET) turnCentipede(i, j);
	}
	if(bombing && centipedes[i].clock % 20 == 0) hitCentipede(i);
	// against player bullet
	centipedeCollided = FALSE;
	for(s16 j = 0; j < PLAYER_BULLET_LIMIT; j++) if(playerBullets[j].active) {
		if(fix16Sub(playerBullets[j].pos.y, CENTIPEDE_BULLET_OFFSET) <= fix16Add(centipedes[i].pos.y, CENTIPEDE_COLLIDE_OFFSET) &&
			fix16Add(playerBullets[j].pos.y, CENTIPEDE_BULLET_OFFSET) >= fix16Sub(centipedes[i].pos.y, CENTIPEDE_COLLIDE_OFFSET) &&
			fix16Sub(playerBullets[j].pos.x, CENTIPEDE_BULLET_OFFSET) <= fix16Add(centipedes[i].pos.x, CENTIPEDE_COLLIDE_OFFSET) &&
			fix16Add(playerBullets[j].pos.x, CENTIPEDE_BULLET_OFFSET) >= fix16Sub(centipedes[i].pos.x, CENTIPEDE_COLLIDE_OFFSET)){
			centipedeCollided = TRUE;
			hitCentipede(i);
			removePlayerBullet(j);
		}
	}
	// against player
	// if(centipedes[i].clock % 4 == 0 && (!centipedeCollided && centipedes[i].pos.y >= fix16Sub(playerPos.y, FIX16(32)))){
	// 	if(fix16Sub(playerPos.y, CENTIPEDE_PLAYER_OFFSET) <= fix16Add(centipedes[i].pos.y, CENTIPEDE_COLLIDE_OFFSET) &&
	// 		fix16Add(playerPos.y, CENTIPEDE_PLAYER_OFFSET) >= fix16Sub(centipedes[i].pos.y, CENTIPEDE_COLLIDE_OFFSET) &&
	// 		fix16Sub(playerPos.x, CENTIPEDE_PLAYER_OFFSET) <= fix16Add(centipedes[i].pos.x, CENTIPEDE_COLLIDE_OFFSET) &&
	// 		fix16Add(playerPos.x, CENTIPEDE_PLAYER_OFFSET) >= fix16Sub(centipedes[i].pos.x, CENTIPEDE_COLLIDE_OFFSET)){
			// hitCentipede(i);
			// turnCentipede(i);
			// hit player here
	// 	}
	// }
}

void destroyCentipede(s16 i){
	centipedes[i].active = FALSE;
	SPR_releaseSprite(centipedes[i].image);
}


// loop

void loadCentipede(){
	centipedeCount = 4;
	if(currentZone >= 5) centipedeCount++;
	if(currentZone >= 10) centipedeCount++;
	if(currentZone >= 15) centipedeCount++;
	for(u8 i = 0; i < centipedeCount; i++) spawnCentipede(i);
}

void resetCentipede(){
	for(u8 i = 0; i < centipedeCount; i++) destroyCentipede(i);
}

void updateCentipede(){
	zoneOverCheck = TRUE;
	for(u8 i = 0; i < centipedeCount; i++) if(centipedes[i].active) {
		zoneOverCheck = FALSE;
		moveCentipede(i);
		collideCentipede(i);
		SPR_setPosition(centipedes[i].image, fix16ToInt(centipedes[i].pos.x) - CENTIPEDE_OFF, fix16ToInt(centipedes[i].pos.y) - CENTIPEDE_OFF);
		centipedes[i].clock++;
		if(centipedes[i].clock >= CLOCK_LIMIT) centipedes[i].clock = 0;
	}
	centipedeClock++;
	if(centipedeClock >= CLOCK_LIMIT) centipedeClock = 0;
	if(zoneOverCheck) zoneOver = TRUE;
}
