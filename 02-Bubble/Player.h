#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool canDash();
	float getX();
	float getY();
	bool isDead();
	void changeDeathStatus(bool b);
	void changetoDeadAnim();
	void setDash();
	void clearDash();
	void setDeath();
	void setReappearing();
	void setPlatform(bool a);
	bool getPlatform();
	bool isDying();

	void dash();

	void die();

	void reappear();
	bool infinite_dash;

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool can_dash = true;
	int dash_steps = 10;
	int die_steps = 60;
	int reappearing_steps = 15;
	string dash_direction;
	bool falling;
	bool dashing = false;
	bool stickied = false;
	bool reappearing = false;
	bool dead = false;
	bool dying = false;
	
	bool platformJumping = false;
	bool bPlatformJumping = false;
};


#endif // _PLAYER_INCLUDE


