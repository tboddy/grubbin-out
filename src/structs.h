struct controls {
	bool left, right, up, down, a, b, c, x, y, z, start, mode;
};
struct controls controls;

#define POD_COUNT 16
struct pod {
	bool active, visible;
	s16 clock;
	u8 anim;
	Vect2D_f16 pos;
	Sprite* image;
};
struct podSpawner {
	fix16 x, y;
};
struct pod pods[POD_COUNT];


#define PLAYER_BULLET_LIMIT 4
struct playerBullet {
	bool active, downward;
	Vect2D_f16 pos;
	Sprite* image;
};
struct playerBullet playerBullets[PLAYER_BULLET_LIMIT];


#define ENEMY_BULLET_LIMIT 64
struct enemyBullet {
	bool active, flag1, flag2, flag3, flag4, grazed, big;
	Vect2D_f16 pos, velocity;
	fix16 speed;
	Sprite* image;
	s16 xOffset, yOffset, clock, angle;
	void (*updater)(s16);
	s16 ints[8];
};
struct bulletSpawner {
	bool flag1, flag2, flag3, flag4, big, top;
	fix16 speed, x, y;
	Vect2D_f16 velocity;
	s16 angle;
	Sprite* image;
	s16 ints[8];
};
struct enemyBullet bullets[ENEMY_BULLET_LIMIT];

#define CENTIPEDE_MAX 8
struct centipede {
	Sprite* image;
	Vect2D_f16 pos, velocity;
	bool active, seen;
	u16 clock;
	s16 health, angle;
	f16 speed, nextY;
};
struct centipede centipedes[CENTIPEDE_MAX];

#define YIN_COUNT 3
struct yin {
	Sprite* image;
	Vect2D_s16 pos;
	bool horizontal, last, flag1, flag2;
	s16 clock, speed, int1, int2, int3;
};
struct yin yins[YIN_COUNT];

struct powerupStruct {
	u8 type;
	Sprite* image;
	s16 x, y;
	u16 clock;
	bool active;
};
struct powerupStruct powerup;

struct bossStruct {
	u8 type;
	Vect2D_f16 pos, velocity;
	u16 clock;
	s16 health, max;
	bool active, flip, big;
	Sprite* image;
	s16 ints[8];
	bool bools[8];
	fix16 fixes[8]; 
};
struct bossStruct boss;