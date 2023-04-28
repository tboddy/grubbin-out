#define POWERUP_LIMIT 300
#define POWERUP_SPAWN 900
#define POWERUP_X 48
#define POWERUP_Y 64
#define POWERUP_W 160
#define POWERUP_H 56

static void spawnPowerup(u8 type){
	powerup.type = type;
	powerup.x = POWERUP_X + random() % POWERUP_W;
	powerup.y = POWERUP_Y + random() % POWERUP_H;
	powerup.clock = 0;
	powerup.active = TRUE;
	powerup.image = SPR_addSprite(&imgPowerup, powerup.x, powerup.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
}

static void killPowerup(){
	powerup.active = FALSE;
	SPR_releaseSprite(powerup.image);
}

void updatePowerup(){
	// if(!boss.active){
	// 	if(powerup.active){
	// 		powerup.clock++;
	// 		if(powerup.clock >= POWERUP_LIMIT) killPowerup();
	// 	} else if(gameClock > 0 && gameClock % POWERUP_SPAWN == 0){
	// 		spawnPowerup(0);
	// 	}
	// }
}