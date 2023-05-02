#define BOSS_OFF_X 16
#define BOSS_OFF_Y 20

#define BOSS_COLLIDE_OFFSET FIX32(40)

#define BOSS_SWITCH_TIME -90

s32 bossCollisionDistance;

bool hitBossPatterns[8];
 

// spawn

#define BOSS_Y FIX16(56)

Sprite *bossImage;
static void spawnBoss(){
	switch(boss.type){
		case 0:
			bossImage = &imgMike;
			boss.health = 100;
			break;
		case 1:
			bossImage = &imgTsukasa;
			boss.health = 150;
			break;
		case 2:
			bossImage = &imgMegumu;
			boss.health = 150;
			break;
		case 3:
			bossImage = &imgChimata;
			boss.health = 250;
			break;
	}
	for(u8 i = 0; i < 8; i++) boss.ints[i] = 0;
	boss.max = boss.health;
	boss.clock = 0;
	boss.active = TRUE;
	boss.pos.x = FIX16(GAME_WIDTH / 2);
	boss.pos.y = BOSS_Y;
	boss.velocity.x = FIX16(0.75);
	boss.velocity.y = FIX16(0.125);
	boss.image = SPR_addSprite(bossImage, fix16ToInt(boss.pos.x) - BOSS_OFF_X, fix16ToInt(boss.pos.y) - BOSS_OFF_Y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
	SPR_setDepth(boss.image, 3);
}


// shoot

#define BOSS_SIDE_L FIX16(56)
#define BOSS_SIDE_R FIX16(GAME_WIDTH - 56)
Vect2D_f16 bossHone;
Vect2D_f16 bossHonePos, bossHonePos2, bossHonePos3;

void hitBossPattern(u8 i){
	if(!hitBossPatterns[i]){
		hitBossPatterns[i] = TRUE;
		boss.clock = BOSS_SWITCH_TIME;
		killBullets = TRUE;
	}
}

static void bossPatternOne(){
	// fan
	if(boss.clock % 15 == 0){
		struct bulletSpawner spawner = {
			.x = boss.clock % 60 < 30 ? BOSS_SIDE_L : BOSS_SIDE_R,
			.y = BOSS_Y,
			.angle = 112,
			.speed = FIX16(4),
			.image = &imgSmallRedBullet
		};
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 4; i++){
			spawnEnemyBullet(spawner, EMPTY);
			spawner.angle += 96;
		}
	}
	// homing
	else if(boss.clock % 60 == 40 || boss.clock % 60 == 50){
		if(boss.clock % 60 == 40) bossHone = hone(boss.pos, playerPos, FIX16(5), 0);
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.top = TRUE,
			.image = &imgBigBlueBullet,
			.big = TRUE
		};
		spawner.velocity = bossHone;
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
		// bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 96, TRUE);
	}
}
static void bossPatternTwo(){
	// fan
	if(boss.clock % 15 == 0){
		struct bulletSpawner spawner = {
			.x = boss.clock % 60 < 30 ? BOSS_SIDE_L : BOSS_SIDE_R,
			.y = BOSS_Y,
			.angle = 112,
			.speed = FIX16(4),
			.image = &imgSmallBlueBullet
		};
		if(boss.clock % 30 > 0) spawner.angle += 48;
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < (boss.clock % 30 == 0 ? 4 : 3); i++){
			spawnEnemyBullet(spawner, EMPTY);
			spawner.angle += 96;
		}
	}
	// homing
	if(boss.clock % 40 == 20 || boss.clock % 40 == 25){
		// if(boss.clock % 60 == 40)
		bossHone = hone(boss.pos, playerPos, FIX16(5), 0);
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.top = TRUE,
			.image = &imgBigRedBullet,
			.big = TRUE
		};
		spawner.velocity = bossHone;
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
}
static void bossPatternThree(){
	// rain
	if(boss.clock % 10 == 0){
		if(boss.clock % 60 == 0) boss.ints[0] = 32 + random() % 64;
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgSmallBlueBullet,
			.angle = boss.ints[0],
			.speed = FIX16(6)
		};
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 4; i++){
			spawnEnemyBullet(spawner, EMPTY);
			spawner.angle += 64;
		}
		boss.ints[0] += 32;
	}
	// balls
	else if(boss.clock % 40 == 35){
		struct bulletSpawner spawner = {
			.x = boss.clock % 80 < 40 ? BOSS_SIDE_L : BOSS_SIDE_R,
			.y = BOSS_Y,
			.top = TRUE,
			.image = &imgBigRedBullet,
			.big = TRUE
		};
		bossHonePos.x = spawner.x;
		bossHonePos.y = spawner.y;
		spawner.velocity = hone(bossHonePos, playerPos, FIX16(5), 0);
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
}
static void bossPatternFour(){
	// ring
	if(boss.clock % 30 == 0){
		boss.ints[0] = 0;
		boss.ints[1] = boss.clock % 60 == 0 ? 32 : 24;
		boss.bools[0] = boss.clock % 600 < 300;
	}
	if(boss.clock % 10 == 0){
		struct bulletSpawner spawner = {
			.x = boss.bools[0] ? BOSS_SIDE_R : BOSS_SIDE_L,
			.y = BOSS_Y,
			.image = &imgSmallBlueBullet,
			.speed = FIX16(5)
		};
		if(boss.bools[0]){
			spawner.angle = 128;
			boss.ints[0] += boss.ints[1];
		} else {
			spawner.angle = 192;
			boss.ints[0] -= boss.ints[1];
		}
		spawner.angle += boss.ints[0];
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 3; i++){
			spawnEnemyBullet(spawner, EMPTY);
			spawner.angle += 96;
		}
	}
	// homing
	if(boss.clock % 30 == 25){
		struct bulletSpawner spawner = {
			.x = boss.bools[0] ? BOSS_SIDE_L : BOSS_SIDE_R,
			.y = BOSS_Y,
			.top = TRUE,
			.image = &imgSmallRedBullet
		};
		bossHonePos.x = spawner.x;
		bossHonePos.y = spawner.y;
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 4; i++){
			if(i == 3){
				spawner.image = &imgBigRedBullet;
				spawner.big = TRUE;
			}
			spawner.velocity = hone(bossHonePos, playerPos, FIX16(5), 88);
			spawnEnemyBullet(spawner, EMPTY);
		}
	}
}
static void bossPatternFive(){
	// ringz
	if(boss.clock % 3 == 0){
		struct bulletSpawner spawner = {
			.x = boss.clock % 6 == 0 ? BOSS_SIDE_R : BOSS_SIDE_L,
			.y = BOSS_Y,
			.image = &imgBigBlueBullet,
			.speed = FIX16(boss.clock % 12 < 6 ? 5 : 6),
			.angle = random() % 256,
			.big = TRUE
		};
		if(boss.clock % 6 == 0) spawner.angle += 256;
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
}
static void bossPatternSix(){
	// homing
	if(boss.clock % 15 == 3){
		struct bulletSpawner spawner = {
			.x = boss.clock % 30 == 3 ? BOSS_SIDE_R : BOSS_SIDE_L,
			.y = BOSS_Y,
			.image = &imgBigBlueBullet,
			.big = TRUE
		};
		bossHonePos.x = spawner.x;
		bossHonePos.y = spawner.y;
		spawner.velocity = hone(bossHonePos, playerPos, FIX16(6), 8);
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
	// puke
	if(boss.clock % 4 == 0){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgSmallRedBullet
		};
		spawner.velocity = hone(boss.pos, playerPos, FIX16(5), 112);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
}
static void bossPatternSeven(){
	// swash
	if(boss.clock % 5 == 0){
		if(boss.clock % 30 == 0){
			boss.ints[0] = 400 - random() % 64;
		}
		struct bulletSpawner spawner = {
			.x = BOSS_SIDE_L,
			.y = BOSS_Y,
			.image = &imgBigRedBullet,
			.big = TRUE,
			.angle = boss.ints[0],
			.speed = FIX16(5),
			.top = TRUE
		};
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
		boss.ints[0] -= 56;
	}
	// ring
	if(boss.clock % 10 == 3){
		struct bulletSpawner spawner = {
			.x = BOSS_SIDE_R,
			.y = BOSS_Y,
			.image = &imgSmallBlueBullet,
			.angle = 256 - random() % 128,
			.speed = FIX16(6)
		};
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 4; i++){
			if(spawner.angle < 480) spawnEnemyBullet(spawner, EMPTY);
			spawner.angle += 96;
		}
	}
}
static void bossPatternEight(){
	// rays
	if(boss.clock % 4 == 0){
		struct bulletSpawner spawner = {
			.x = FIX16(random() % 64),
			.y = BOSS_Y,
			.image = &imgSmallBlueBullet,
			.speed = FIX16(5),
			.angle = 128 + random() % 256
		};
		spawner.x = fix16Add(spawner.x, FIX16(boss.clock % 10 == 0 ? 32 : 96));
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
	// homing
	if(boss.clock % 60 < 45 && boss.clock % 10 == 5){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgBigRedBullet,
			.top = TRUE,
			.big = TRUE
		};
		spawner.velocity = hone(boss.pos, playerPos, FIX16(6), 16);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
}
static void bossPatternNine(){
	// sides
	if(boss.clock % 150 < 60){
		if(boss.clock % 150 == 0){
			boss.ints[0] = 0;
		}
		if(boss.clock % 5 == 0){
			struct bulletSpawner spawner = {
				.x = boss.clock % 10 == 0 ? BOSS_SIDE_L : BOSS_SIDE_R,
				.y = BOSS_Y,
				.image = &imgBigRedBullet,
				.big = TRUE,
				.speed = FIX16(6),
				.angle = 256 + (boss.clock % 10 == 0 ? -boss.ints[0] : boss.ints[0])
			};
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			spawnEnemyBullet(spawner, EMPTY);
			boss.ints[0] += 16;
		}
	}
	// homing
	if(boss.clock % 60 < 50 && boss.clock % 10 == 3){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgBigBlueBullet,
			.top = TRUE,
			.big = TRUE
		};
		spawner.velocity = hone(boss.pos, playerPos, FIX16(5), 0);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
	if(boss.clock % 300 == 118 || boss.clock % 300 == (236)){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgSmallBlueBullet
		};
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < 8; i++){
			spawner.velocity = hone(boss.pos, playerPos, FIX16(4 + random() % 2), 64);
			spawnEnemyBullet(spawner, EMPTY);
		}
	}
}
static void bossPatternTen(){
	// ring
	if((boss.clock % 300 < 60 || (boss.clock % 300 >= 120 && boss.clock % 300 < 180)) && boss.clock % 10 == 0){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgBigBlueBullet,
			.angle = boss.clock % 20 == 0 ? 0 : 32,
			.speed = FIX16(5),
			.big = TRUE
		};
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		for(u8 i = 0; i < (boss.clock % 20 == 0 ? 8 : 7); i++){
			spawner.angle += 57;
			spawnEnemyBullet(spawner, EMPTY);
		}
	}

	// puke
	else if(((boss.clock % 300 >= 60 && boss.clock % 300 < 120) || (boss.clock % 300 >= 180)) && boss.clock % 5 == 0){
		if(boss.clock % 300 == 60 || boss.clock % 300 == 180){
			bossHonePos2.x = playerPos.x;
			bossHonePos2.y = playerPos.y;
			boss.fixes[0] = FIX16(16);
		}
		struct bulletSpawner spawner = {
			.x = boss.clock % 10 == 0 ? boss.fixes[0] : fix16Sub(FIX16(GAME_WIDTH), boss.fixes[0]),
			.y = BOSS_Y,
			.image = &imgBigRedBullet,
			.angle = boss.clock % 20 == 0 ? 0 : 32,
			.speed = FIX16(6),
			.big = TRUE
		};
		bossHonePos.x = spawner.x;
		bossHonePos.y = spawner.y;
		spawner.velocity = hone(bossHonePos, bossHonePos2, FIX16(5), 0);
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
		boss.fixes[0] = fix16Add(boss.fixes[0], FIX16(12));
	}
}
static void bossPatternEleven(){
	// sides
	if(boss.clock % 5 == 0){
		struct bulletSpawner spawner = {
			.x = boss.clock % 10 == 0 ? FIX16(40) : FIX16(GAME_WIDTH - 40),
			.y = BOSS_Y,
			.image = &imgBigRedBullet,
			.angle = 240 + random() % 32,
			.speed = FIX16(6),
			.big = TRUE
		};
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
	}
	// puke
	if(boss.clock % 15 == 4){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y
		};
		for(u8 i = 0; i < 4; i++){
			if(i % 2 == 0) spawner.image = &imgSmallBlueBullet;
			else {
				spawner.image = &imgBigBlueBullet;
				spawner.big = TRUE;
			}
			spawner.velocity = hone(boss.pos, playerPos, FIX16(i % 2 == 0 ? 5 : 7), 80);
			spawnEnemyBullet(spawner, EMPTY);
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}
static void bossPatternTwelve(){
	// swash
	if(boss.clock % 5 == 0){
		if(boss.clock % 60 == 0){
			boss.ints[0] = 0;
		}
		struct bulletSpawner spawner = {
			.x = boss.clock % 10 == 0 ? BOSS_SIDE_L : BOSS_SIDE_R,
			.y = BOSS_Y,
			.image = &imgBigRedBullet,
			.angle = boss.clock % 10 == 0 ? (448 - boss.ints[0]) : (64 + boss.ints[0]),
			.speed = FIX16(6),
			.big = TRUE
		};
		spawnExplosion(fix16ToInt(spawner.x), fix16ToInt(spawner.y), FALSE);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		spawnEnemyBullet(spawner, EMPTY);
		boss.ints[0] += 32;
	}
	if(boss.clock % 15 == 4){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y
		};
		for(u8 i = 0; i < 4; i++){
			if(i % 2 == 0) spawner.image = &imgSmallBlueBullet;
			else {
				spawner.image = &imgBigBlueBullet;
				spawner.big = TRUE;
			}
			spawner.velocity = hone(boss.pos, playerPos, FIX16(i % 2 == 0 ? 5 : 7), 80);
			spawnEnemyBullet(spawner, EMPTY);
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}
static void bossPatternThirteen(){
	if(boss.clock % 300 < 120){
		if(boss.clock % 6 == 0){

			struct bulletSpawner spawner = {
				.x = boss.pos.x,
				.y = boss.pos.y,
				.image = &imgBigBlueBullet,
				.angle = 32 + random() % 64,
				.speed = FIX16(8),
				.big = TRUE
			};
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			for(u8 i = 0; i < 5; i++){
				spawner.angle += 64;
				spawnEnemyBullet(spawner, EMPTY);
			}
		}
	} else {
		if(boss.clock % 10 == 0){
			struct bulletSpawner spawner = {
				.x = boss.pos.x,
				.y = boss.pos.y,
				.image = &imgBigRedBullet,
				.big = TRUE
			};
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			for(u8 i = 0; i < 5; i++){
				spawner.velocity = hone(boss.pos, playerPos, FIX16(i % 2 == 0 ? 6 : 8), 96);
				spawnEnemyBullet(spawner, EMPTY);
			}
		}
	}
}


static void shootBoss(){
	switch(boss.type){
		case 0:
			if(boss.health < 50){
				hitBossPattern(0);
				if(boss.clock >= 0) bossPatternTwo();
			} else if(boss.clock >= 0) bossPatternOne();
			break;
		case 1:
			if(boss.health < 50){
				hitBossPattern(2);
				if(boss.clock >= 0) bossPatternFour();
			} else if(boss.health < 100){
				hitBossPattern(1);
				if(boss.clock >= 0) bossPatternFive();
			} else if(boss.clock >= 0) bossPatternThree();
			break;
		case 2:
			if(boss.health < 50){
				hitBossPattern(4);
				if(boss.clock >= 0) bossPatternEight();
			} else if(boss.health < 100){
				hitBossPattern(3);
				if(boss.clock >= 0) bossPatternSeven();
			} else if(boss.clock >= 0) bossPatternSix();
			break;
		case 3:
			if(boss.health < 50){
				hitBossPattern(8);
				if(boss.clock >= 0) bossPatternThirteen();
			} else if(boss.health < 100){
				hitBossPattern(7);
				if(boss.clock >= 0) bossPatternTwelve();
			} else if(boss.health < 150){
				hitBossPattern(6);
				if(boss.clock >= 0) bossPatternEleven();
			} else if(boss.health < 200){
				hitBossPattern(5);
				if(boss.clock >= 0) bossPatternTen();
			} else if(boss.clock >= 0) bossPatternNine();
			break;
	}
}


// move

#define BOSS_LIMIT_X FIX16(48)
#define BOSS_LIMIT_W FIX16(GAME_WIDTH - 48)
#define BOSS_LIMIT_Y FIX16(48)
#define BOSS_LIMIT_H FIX16(64)
s16 bossMoveClock;

static void moveBoss(){
	bossMoveClock = boss.clock % 300 - 240;
	switch(bossMoveClock){
		case 0: boss.velocity.x = FIX16(boss.flip ? -0.5 : 0.5); break;
		case 15: boss.velocity.x = FIX16(boss.flip ? -1 : 1); break;
		case 45: boss.velocity.x = FIX16(boss.flip ? -0.5 : 0.5); break;
	}
	boss.pos.x = fix16Add(boss.pos.x, boss.velocity.x);
	boss.pos.y = fix16Add(boss.pos.y, boss.velocity.y);
	if(boss.pos.x >= BOSS_LIMIT_W || boss.pos.x <= BOSS_LIMIT_X){
		boss.velocity.x *= -1;
		boss.flip = boss.pos.x >= BOSS_LIMIT_W;
	}
	if(boss.pos.y >= BOSS_LIMIT_H || boss.pos.y <= BOSS_LIMIT_Y)
		boss.velocity.y *= -1;
	SPR_setPosition(boss.image, fix16ToInt(boss.pos.x) - BOSS_OFF_X, fix16ToInt(boss.pos.y) - BOSS_OFF_Y);
}


// collision

void hitBoss(){
	boss.health--;
	// boss.health -= 10;
	if(boss.health <= 0) finishBoss();
}

#define BOSS_COLLIDE_CHECK_B FIX16(8)
#define BOSS_COLLIDE_CHECK_X FIX16(14)
#define BOSS_COLLIDE_CHECK_Y FIX16(20)

void collideBoss(){
	if(bombing && boss.clock % 10 == 0) boss.health--;
	else if(!bombing){
		for(s16 j = 0; j < PLAYER_BULLET_LIMIT; j++) if(playerBullets[j].active) {
			if(fix16Sub(playerBullets[j].pos.y, BOSS_COLLIDE_CHECK_B) <= fix16Add(boss.pos.y, BOSS_COLLIDE_CHECK_Y) &&
				fix16Add(playerBullets[j].pos.y, BOSS_COLLIDE_CHECK_B) >= fix16Sub(boss.pos.y, BOSS_COLLIDE_CHECK_Y) &&
				fix16Sub(playerBullets[j].pos.x, BOSS_COLLIDE_CHECK_B) <= fix16Add(boss.pos.x, BOSS_COLLIDE_CHECK_X) &&
				fix16Add(playerBullets[j].pos.x, BOSS_COLLIDE_CHECK_B) >= fix16Sub(boss.pos.x, BOSS_COLLIDE_CHECK_X)){
				spawnExplosion(fix16ToInt(playerBullets[j].pos.x), fix16ToInt(playerBullets[j].pos.y), TRUE);
				hitBoss();
				removePlayerBullet(j);
			}
		}
	}
}

void finishBoss(){
	boss.clock = BOSS_SWITCH_TIME;
	boss.active = FALSE;
	zoneOver = TRUE;
	boss.type++;
}


// loop

void updateBoss(){
	if(boss.active){
		if(boss.clock % 300 >= 240) moveBoss();
		else shootBoss();
		collideBoss();
		boss.clock++;
		if(boss.clock >= CLOCK_LIMIT) boss.clock = 0;
	}
}



