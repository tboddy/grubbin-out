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
			bossImage = &imgChimata;
			boss.health = 100;
			break;
		case 1:
			bossImage = &imgChimata;
			boss.health = 150;
			break;
		case 2:
			bossImage = &imgChimata;
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
		spawnEnemyBullet(spawner, EMPTY);
		// bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 96, TRUE);
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
		spawnEnemyBullet(spawner, EMPTY);
	}
	if(boss.clock % 300 == 118 || boss.clock % 300 == (236)){
		struct bulletSpawner spawner = {
			.x = boss.pos.x,
			.y = boss.pos.y,
			.image = &imgSmallBlueBullet
		};
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
		spawnEnemyBullet(spawner, EMPTY);
		boss.fixes[0] = fix16Add(boss.fixes[0], FIX16(12));
	}
}
static void bossPatternEleven(){
	
}
static void bossPatternTwelve(){}
static void bossPatternThirteen(){}


static void shootBoss(){
	bossPatternTen();
	// switch(boss.type){
	// 	case 0:
	// 		if(boss.health < 50){
	// 			hitBossPattern(0);
	// 			bossPatternTwo();
	// 		} else bossPatternOne();
	// 		break;
	// }
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
}


// loop

void updateBoss(){
	if(boss.active) {
		if(boss.clock % 300 >= 240) moveBoss();
		else shootBoss();
		collideBoss();
		boss.clock++;
		if(boss.clock >= 36000) boss.clock = 0;
	}
}










// shooting

	// void bossPatternOne(){
	// 	// if(boss.clock % 10 == 0 && boss.clock % 60 < 50){
	// 	// 	if(boss.clock % 60 == 0) bossInt1 = random() % 1024;
	// 	// 	struct bulletSpawner bSpawn = {
	// 	// 		.x = FIX16(boss.clock % 120 < 60 ? 208 : 48),
	// 	// 		.y = BOSS_B_Y,
	// 	// 		.type = 1,
	// 	// 		.angle = bossInt1,
	// 	// 		.speed = FIX16(5)
	// 	// 	};
	// 	// 	for(s8 b = 0; b < 6; b++){
	// 	// 		if(bSpawn.angle % 1024 >= 0 && bSpawn.angle % 1024 <= 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 	// 		bSpawn.angle += 170;
	// 	// 	}
	// 	// 	// XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	// 	bossInt1 += boss.clock % 120 < 60 ? 32 : -32;
	// 	// 	if(boss.clock % 30 == 0) spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 	// 	if(boss.clock % 20 == 10){
	// 	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 96, TRUE);
	// 	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 16, FALSE);
	// 	// 		bSpawn.type = 4;
	// 	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 	// 		// XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	// 	}
	// 	// }
	// }

	// void bossPatternTwo(){
	// 	if(boss.clock % 60 == 0){
	// 		bossInt1 = 240 + random() % 32;
	// 		bossInt2 = boss.clock % 120 == 0 ? 48 : -48;
	// 		bossInt3 = bossInt2 / 2;
	// 	}
	// 	if(boss.clock % 10 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 120 < 60 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.type = 4,
	// 			.angle = bossInt1 - bossInt2,
	// 			.speed = FIX16(5)
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 3; b++){
	// 			spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.speed = fix16Add(bSpawn.speed, FIX16(1.5));
	// 			bSpawn.angle += bossInt3;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1 += bossInt2;
	// 	}
	// }

	// void bossPatternThree(){
	// 	if(boss.clock % 60 < 18 && boss.clock % 6 == 0){
	// 		if(boss.clock % 60 == 0){
	// 			bossInt1 = 3; 
	// 			bossInt2 = boss.clock % 120 == 0 ? 68 : -68;
	// 			bossInt3 = boss.clock % 120 == 0 ? 224 : 288;
	// 			bossFix1 = FIX16(boss.clock % 120 < 60 ? 208 : 48);
	// 		}
	// 		struct bulletSpawner bSpawn = {
	// 			.x = bossFix1,
	// 			.y = BOSS_B_Y,
	// 			.type = 3,
	// 			.angle = bossInt3,
	// 			.speed = FIX16(bossInt1)
	// 		};
	// 		if(boss.clock % 60 == 0) spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 4; b++){
	// 			spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += bossInt2;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1++;
	// 	}
	// 	else if(boss.clock % 60 >= 30 && boss.clock % 5 == 0){
	// 		if(boss.clock % 60 == 30){
	// 			bossFix1 = FIX16(32 + random() % 192);
	// 			spawnExplosion(fix16ToInt(bossFix1), fix16ToInt(BOSS_B_Y), FALSE);
	// 		}
	// 		struct bulletSpawner bSpawn = {
	// 			.x = bossFix1,
	// 			.y = BOSS_B_Y,
	// 			.type = 2
	// 		};
	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 96, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 16, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternFour(){
	// 	if(boss.clock % 60 <= 10 && boss.clock % 5 == 0){
	// 		if(boss.clock % 60 == 0) bossInt1 = 4;
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.speed = FIX16(bossInt1)
	// 		};
	// 		bSpawn.angle = boss.clock % 60 == 5 ? 32 : 0;
	// 		bSpawn.type = boss.clock % 60 == 0 ? 3 : 4;
	// 		for(s8 b = 0; b < (boss.clock % 60 == 5 ? 5 : 6); b++){
	// 			bSpawn.angle += 64;
	// 			if(b > 0) spawnEnemyBullet(bSpawn, eUpdate);
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1++;
	// 	} else if(boss.clock % 60 >= 30 && boss.clock % 5 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 10 == 0 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.type = 1
	// 		};
	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 96, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 4, 16, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 	}
	// }

	// void bossPatternFive(){
	// 	if(boss.clock % 60 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.angle = random() % 1024,
	// 			.speed = FIX16(4),
	// 			.type = 4
	// 		};
	// 		for(s8 b = 0; b < 16; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512){
	// 				spawnEnemyBullet(bSpawn, eUpdate);
	// 			}
	// 			bSpawn.angle += 64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	} else if(boss.clock % 60 >= 30 && boss.clock % 60 < 45 && boss.clock % 5 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.type = boss.clock % 60 >= 38 ? 3 : 4
	// 		};
	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 128, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 16, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	}
	// 	if(boss.clock % 60 >= 30 && boss.clock % 5 == 3){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 30 < 15 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.type = boss.clock % 15 > 6 ? 1 : 2
	// 		};
	// 		if(boss.clock % 15 == 0){
	// 			bossFix1 = bSpawn.x;
	// 			bossFix2 = bSpawn.y;
	// 		}
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		bSpawn.velocityX = honeEnemyBullet(bossFix1, bossFix2, 4, 32, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bossFix1, bossFix2, 4, 16, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternSix(){
	// 	if(boss.clock % 5 == 0 && boss.clock % 120 < 110){
	// 		if(boss.clock % 120 == 0){
	// 			bossInt1 = 0 + random() % 32;
	// 			bossInt2 = 512 - random() % 32;
	// 			bossInt3 = 1;
	// 		}
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(48),
	// 			.y = BOSS_B_Y,
	// 			.angle = bossInt1,
	// 			.speed = FIX16(7),
	// 			.type = boss.clock % 240 < 120 ? 4 : 2 
	// 		};
	// 		if(boss.clock % 10 == 0) spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 2; b++){
	// 			if(bSpawn.angle % 1024 < 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 512;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bSpawn.x = FIX16(208);
	// 		bSpawn.angle = bossInt2;
	// 		if(boss.clock % 10 == 5) spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 2; b++){
	// 			if((bSpawn.angle < 0 && abs(bSpawn.angle) % 1024 > 512) || (bSpawn.angle >= 0 && bSpawn.angle < 512)) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 512;
	// 		}
	// 		bossInt1 += 5 * bossInt3;
	// 		bossInt2 -= 10 * bossInt3;
	// 		bossInt3++;
	// 	}
	// }

	// void bossPatternSeven(){
	// 	if(boss.clock % 120 < 100){
	// 		if(boss.clock % 120 == 0) bossInt1 = 0;
	// 		if(boss.clock % 15 == 0){
	// 			struct bulletSpawner bSpawn = {
	// 				.x = BOSS_B_X,
	// 				.y = BOSS_B_Y,
	// 				.angle = 0 + bossInt1,
	// 				.speed = FIX16(8),
	// 				.type = 2
	// 			};
	// 			for(s8 b = 0; b < 16; b++){
	// 				if(bSpawn.angle % 1024 > 32 && bSpawn.angle % 1024 < 480) spawnEnemyBullet(bSpawn, eUpdate);
	// 				bSpawn.angle += 64;
	// 			}
	// 			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 			bossInt1 += 24;
	// 		}
	// 		if(boss.clock % 10 == 5){
	// 			struct bulletSpawner bSpawn = {
	// 				.x = BOSS_B_X,
	// 				.y = BOSS_B_Y,
	// 				.type = 3
	// 			};
	// 			bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 128, TRUE);
	// 			bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 16, FALSE);
	// 			spawnEnemyBullet(bSpawn, eUpdate);
	// 			XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 		}
	// 	}
	// }

	// void bossPatternEight(){
	// 	if(boss.clock % 30 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 60 == 0 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.angle = random() % 1024,
	// 			.speed = FIX16(8),
	// 			.type = 1
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 8; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512){
	// 				spawnEnemyBullet(bSpawn, eUpdate);
	// 				bSpawn.angle += 48;
	// 				spawnEnemyBullet(bSpawn, eUpdate);
	// 			} else bSpawn.angle += 48;
	// 			bSpawn.angle += 80;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	} else if(boss.clock % 30 == 15){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 60 == 15 ? 192 : 64),
	// 			.y = BOSS_B_Y,
	// 			.angle = random() % 1024,
	// 			.speed = FIX16(6),
	// 			.type = 4
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 16; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternNine(){
	// 	if(boss.clock % 60 < 40 && boss.clock % 6 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(64 + random() % 128),
	// 			.y = BOSS_B_Y,
	// 			.type = 3
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 4; b++){
	// 			bSpawn.type = b % 2 == 0 ? 3 : 4;
	// 			bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 192, TRUE);
	// 			bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 5, 8, FALSE);
	// 			spawnEnemyBullet(bSpawn, eUpdate);
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternTen(){
	// 	if(boss.clock % 15 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(64 + random() % 128),
	// 			.y = BOSS_B_Y,
	// 			.type = boss.clock % 30 == 0 ? 4 : 3,
	// 			.angle = random() % 1024,
	// 			.speed = FIX16(boss.clock % 30 == 0 ? 7 : 5)
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 16; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 	}
	// 	else if(boss.clock % 60 > 15 && boss.clock % 60 < 30 && boss.clock % 2 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.type = boss.clock % 4 == 0 ? 2 : 1
	// 		};
	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, 6, 96, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, 6, 8, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternEleven(){
	// 	if(boss.clock % 60 < 30 && boss.clock % 4 == 0){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 120 < 60 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.type = 3
	// 		};
	// 		if(boss.clock % 60 == 0) bossInt1 = 4;
	// 		if(boss.clock % 60 == 0 || boss.clock % 60 == 12) spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		bSpawn.velocityX = honeEnemyBullet(bSpawn.x, bSpawn.y, bossInt1, 64, TRUE);
	// 		bSpawn.velocityY = honeEnemyBullet(bSpawn.x, bSpawn.y, bossInt1, 8, FALSE);
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1++;
	// 	}
	// 	if(boss.clock % 15 == 5){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.type = 2,
	// 			.angle = boss.clock % 30 == 5 ? 32 : 0,
	// 			.speed = FIX16(6)
	// 		};
	// 		for(s8 b = 0; b < 16; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternTwelve(){
	// 	if(boss.clock % 60 < 35 && boss.clock % 6 == 0){
	// 		if(boss.clock % 60 == 0){
	// 			bossInt1 = 320;
	// 			bossInt2 = 192;
	// 		}
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 12 == 0 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.speed = FIX16(7),
	// 			.angle = boss.clock % 12 == 0 ? bossInt2 : bossInt1,
	// 			.type = 4
	// 		};
	// 		for(s8 b = 0; b < 4; b++){
	// 			spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += boss.clock % 12 == 0 ? 64 : -64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1 -= 8;
	// 		bossInt2 += 8;
	// 	}
	// 	if(boss.clock % 3 == 1 && boss.clock % 30 < 15){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.type = 1
	// 		};
	// 		if(boss.clock % 30 == 1){
	// 			bossFix1 = honeEnemyBullet(bSpawn.x, bSpawn.y, 7, 0, TRUE);
	// 			bossFix2 = honeEnemyBullet(bSpawn.x, bSpawn.y, 7, 0, FALSE);
	// 		}
	// 		bSpawn.velocityX = bossFix1;
	// 		bSpawn.velocityY = bossFix2;
	// 		spawnEnemyBullet(bSpawn, eUpdate);
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void bossPatternThirteen(){
	// 	if(boss.clock % 5 == 0){
	// 		if(boss.clock == 0){
	// 			bossInt1 = 0;
	// 			bossInt2 = 1024;
	// 		}
	// 		struct bulletSpawner bSpawn = {
	// 			.x = FIX16(boss.clock % 10 == 0 ? 208 : 48),
	// 			.y = BOSS_B_Y,
	// 			.type = 3,
	// 			.angle = boss.clock % 10 == 0 ? bossInt2 : bossInt1,
	// 			.speed = FIX16(6)
	// 		};
	// 		spawnExplosion(fix16ToInt(bSpawn.x), fix16ToInt(bSpawn.y), FALSE);
	// 		for(s8 b = 0; b < 5; b++){
	// 			if(bSpawn.angle % 1024 > 0 && bSpawn.angle % 1024 < 512) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 205;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	// 		bossInt1 += 19;
	// 		bossInt2 -= 15;
	// 		if(bossInt1 >= 1024) bossInt1 = 0;
	// 		if(bossInt2 <= 0) bossInt2 = 1024;
	// 	}
	// 	if(boss.clock % 60 == 35 || boss.clock % 60 == 36){
	// 		struct bulletSpawner bSpawn = {
	// 			.x = BOSS_B_X,
	// 			.y = BOSS_B_Y,
	// 			.type = boss.clock % 60 == 35 ? 2 : 1,
	// 			.angle = boss.clock % 60 == 35 ? 0 : 32,
	// 			.speed = FIX16(boss.clock % 60 == 35 ? 5 : 8)
	// 		};
	// 		for(s8 b = 0; b < 8; b++){
	// 			if(b > 0 && (boss.clock % 60 == 35 || (boss.clock % 60 == 36 && b < 7))) spawnEnemyBullet(bSpawn, eUpdate);
	// 			bSpawn.angle += 64;
	// 		}
	// 		XGM_startPlayPCM(SFX_BULLET_1, 1, SOUND_PCM_CH3);
	// 	}
	// }

	// void hitBossPattern(u8 i){
	// 	if(!hitBossPatterns[i]){
	// 		hitBossPatterns[i] = TRUE;
	// 		boss.clock = BOSS_SWITCH_TIME;
	// 		killBullets = TRUE;
	// 	}
	// }

	// void shootBoss(){
	// 	switch(bossType){
	// 		case 1:
	// 			if(boss.health < 50){
	// 				hitBossPattern(0);
	// 				if(boss.clock >= 0) bossPatternTwo();
	// 			} else if(boss.clock >= 0) bossPatternOne();
	// 			break;
	// 		case 2:
	// 			if(boss.health < 50){
	// 				hitBossPattern(2);
	// 				if(boss.clock >= 0) bossPatternFour();
	// 			} else if(boss.health < 100){
	// 				hitBossPattern(1);
	// 				if(boss.clock >= 0) bossPatternFive();
	// 			} else if(boss.clock >= 0) bossPatternThree();
	// 			break;
	// 		case 3:
	// 			if(boss.health < 50){
	// 				hitBossPattern(4);
	// 				if(boss.clock >= 0) bossPatternEight();
	// 			} else if(boss.health < 100){
	// 				hitBossPattern(3);
	// 				if(boss.clock >= 0) bossPatternSeven();
	// 			} else if(boss.clock >= 0) bossPatternSix();
	// 			break;
	// 		case 4:
	// 			if(boss.health < 50){
	// 				hitBossPattern(8);
	// 				if(boss.clock >= 0) bossPatternThirteen();
	// 			} else if(boss.health < 100){
	// 				hitBossPattern(7);
	// 				if(boss.clock >= 0) bossPatternTwelve();
	// 			} else if(boss.health < 150){
	// 				hitBossPattern(6);
	// 				if(boss.clock >= 0) bossPatternEleven();
	// 			} else if(boss.health < 200){
	// 				hitBossPattern(5);
	// 				if(boss.clock >= 0) bossPatternTen();
	// 			} else if(boss.clock >= 0) bossPatternNine();
	// 			break;
	// 	}
	// }