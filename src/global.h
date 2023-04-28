#define WIN_WIDTH 256
#define GAME_WIDTH 192
#define GAME_HEIGHT 224

#define SFX_MENU_SELECT 64
#define SFX_MENU_CHOOSE 65
#define SFX_START_GAME 66
#define SFX_PLAYER_SHOT 67
#define SFX_ZONE_OVER 68

#define SFX_BULLET_1 69

#define SFX_EXPLOSION_1 73
#define SFX_EXPLOSION_2 74
#define SFX_EXPLOSION_3 75

#define SFX_GAME_OVER 76
#define SFX_BEAT_GAME 77

#define HALF_TILE 10
#define LEAST_TILE 11
#define FULL_TILE 12

#define CLOCK_LIMIT 64000

bool gameOver, noMiss, paused, pausing, zoneOver, zoneStarting, gameStarting, noMiss, doZoneStart, demo;
bool killBullets;

u16 gameClock;
u8 centipedeCount;

s16 currentZone, gameOverClock, bossMax, lastBossHealth;
s32 currentScore, highScore;

Vect2D_f16 playerPos;
bool hitPlayer, playerRecovering, bombing;
s8 playerLives, playerBombs;

s16 yinBulletSpeed, podBulletSpeed;


// utils

void eUpdate(s16 i){if(i){}}

s16 emptyI; void EMPTY(s16 i){emptyI = i;}

void updateEnemyBulletVelocity(s16 i){
	bullets[i].velocity.x = fix16Mul(cosFix16(bullets[i].angle), bullets[i].speed);
	bullets[i].velocity.y = fix16Mul(sinFix16(bullets[i].angle), bullets[i].speed);
}

// fix16 honeX, honeY, honeSpeed;
// fix16 honeEnemyBullet(fix16 x, fix16 y, s16 speed, s16 lerp, bool isX){
// 	honeX = playerPos.x;
// 	honeY = playerPos.y;
// 	if(lerp > 0){
// 		honeX = fix16Add(fix16Sub(honeX, FIX16(lerp)), FIX16(random() % (lerp * 2)));
// 		honeY = fix16Add(fix16Sub(honeY, FIX16(lerp)), FIX16(random() % (lerp * 2)));
// 	}
// 	honeSpeed = fix32ToFix16(getApproximatedDistance(fix16ToFix32(x) - fix16ToFix32(honeX), fix16ToFix32(y) - fix16ToFix32(honeY)));
// 	return isX ? fix16Mul(fix16Div(fix16Sub(honeX, x), honeSpeed), FIX16(speed)) : fix16Mul(fix16Div(fix16Sub(honeY, y), honeSpeed), FIX16(speed));
// }

fix16 honeSpeed;
Vect2D_f16 hPos, velPos, velPos2, velPos3;
Vect2D_f16 hone(Vect2D_f16 pos, Vect2D_f16 target, fix16 speed, s16 lerp){
	hPos.x = target.x;
	hPos.y = target.y;
	if(lerp > 0){
		hPos.x = fix16Add(fix16Sub(hPos.x, FIX16(lerp)), FIX16(random() % (lerp * 2)));
		hPos.y = fix16Add(fix16Sub(hPos.y, FIX16(lerp)), FIX16(random() % (lerp * 2)));
	}
	honeSpeed = fix32ToFix16(getApproximatedDistance(
		fix32Sub(fix16ToFix32(pos.x), fix16ToFix32(hPos.x)),
		fix32Sub(fix16ToFix32(pos.y), fix16ToFix32(hPos.y))));
	hPos.x = fix16Mul(fix16Div(fix16Sub(hPos.x, pos.x), honeSpeed), speed);
	hPos.y = fix16Mul(fix16Div(fix16Sub(hPos.y, pos.y), honeSpeed), speed);
	return hPos;
}