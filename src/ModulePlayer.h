#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
private:
	enum SCREEN_PORTION
	{
		FAR_LEFT = 0,
		LEFT,
		LEFT_MIDDLE,
		MIDDLE,
		RIGHT_MIDDLE,
		RIGHT,
		FAR_RIGHT,
		FAR_LEFT_F,
		LEFT_F,
		LEFT_MIDDLE_F,
		MIDDLE_F,
		RIGHT_MIDDLE_F,
		RIGHT_F,
		FAR_RIGHT_F
	};

	int const portion_calculate();
	int tempx; 

public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* player = nullptr;
	SDL_Texture* crosstexture = nullptr;
	Animation cross;
	Animation* current_animation = nullptr;
	Animation idle[14];
	Animation down[14];
	Animation walk_right;
	Animation walk_left;
	Animation roll_right;
	Animation roll_left;
	iPoint position;
	iPoint cposition; //cross position 
};

#endif