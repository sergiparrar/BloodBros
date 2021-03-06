#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleUserI.h"
#include "ModuleFadeToBlack.h"
#include "ModuleScene.h"
#include "ModuleEnd.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

int const ModulePlayer::portion_calculate()
{
	int distance = (cposition.x + 70/2) - (position.x + 87/2);
	int portion = SCREEN_WIDTH / 7;
	if (distance >= portion/2)
	{
		if (distance >= (2 * portion + portion / 2))
			return FAR_RIGHT;
		else if (distance >= (portion + portion / 2))
			return RIGHT;
		else
			return RIGHT_MIDDLE;
	}
	else if (distance <= -portion/2)
	{
		if (distance <= -(2 * portion + portion / 2))
			return FAR_LEFT;
		else if (distance <= -(portion + portion / 2))
			return LEFT;
		else
			return LEFT_MIDDLE;
	}
	else
		return MIDDLE;
}

ModulePlayer::ModulePlayer()
{
	player = NULL;
	current_animation = NULL;

	cross.PushBack({ 11, 17, 69, 63 });
	cross.PushBack({ 107, 17, 69, 63 });
	cross.loop = true;
	cross.speed = 0.2f;

	fcross.PushBack({ 11, 105, 69, 63 });
	fcross.PushBack({ 107, 105, 69, 63 });
	fcross.PushBack({ 11, 201, 69, 63 });
	fcross.PushBack({ 107,201, 69, 63 });
	fcross.PushBack({ 11, 17, 69, 63 });
	fcross.PushBack({ 107, 105, 69, 63 });
	fcross.loop = true;
	fcross.speed = 0.2f;
	
	idle[FAR_LEFT].PushBack({ 0, 0, 144, 192 });
	idle[LEFT].PushBack({ 144, 0, 144, 192 });
	idle[LEFT_MIDDLE].PushBack({ 288, 0, 144, 192 });
	idle[MIDDLE].PushBack({ 432, 0, 144, 192 });
	idle[RIGHT_MIDDLE].PushBack({ 576, 0, 144, 192 });
	idle[RIGHT].PushBack({ 720, 0, 144, 192 });
	idle[FAR_RIGHT].PushBack({ 864, 0, 144, 192 });

	idle[FAR_LEFT_F].PushBack({ 0, 192, 144, 192 }); 
	idle[FAR_LEFT_F].PushBack({ 0, 384, 144, 192 });
	idle[FAR_LEFT_F].speed = 0.25f;
	idle[LEFT_F].PushBack({ 144, 192, 144, 192 });
	idle[LEFT_F].PushBack({ 144, 384, 144, 192 });
	idle[LEFT_F].speed = 0.25f;
	idle[LEFT_MIDDLE_F].PushBack({ 288, 192, 144, 192 });
	idle[LEFT_MIDDLE_F].PushBack({ 288, 384, 144, 192 });
	idle[LEFT_MIDDLE_F].speed = 0.25f;
	idle[MIDDLE_F].PushBack({ 432, 192, 144, 192 });
	idle[MIDDLE_F].PushBack({ 432, 384, 144, 192 });
	idle[MIDDLE_F].speed = 0.25f;
	idle[RIGHT_MIDDLE_F].PushBack({ 576, 192, 144, 192 });
	idle[RIGHT_MIDDLE_F].PushBack({ 576, 384, 144, 192 });
	idle[RIGHT_MIDDLE_F].speed = 0.25f;
	idle[RIGHT_F].PushBack({ 720, 192, 144, 192 });
	idle[RIGHT_F].PushBack({ 720, 384, 144, 192 });
	idle[RIGHT_F].speed = 0.25f;
	idle[FAR_RIGHT_F].PushBack({ 864, 192, 144, 192 });
	idle[FAR_RIGHT_F].PushBack({ 864, 384, 144, 192 });
	idle[FAR_RIGHT_F].speed = 0.25f;
	

	down[FAR_LEFT].PushBack({ 0, 576, 144, 192 });
	down[LEFT].PushBack({ 144, 576, 144, 192 });
	down[LEFT_MIDDLE].PushBack({ 288, 576, 144, 192 });
	down[MIDDLE].PushBack({ 432, 576, 144, 192 });
	down[RIGHT_MIDDLE].PushBack({ 576, 576, 144, 192 });
	down[RIGHT].PushBack({ 720, 576, 144, 192 });
	down[FAR_RIGHT].PushBack({ 864, 576, 144, 192 });

	down[FAR_LEFT_F].PushBack({ 0, 768, 144, 192 });
	down[FAR_LEFT_F].PushBack({ 0, 960, 144, 192 });
	down[FAR_LEFT_F].speed = 0.25f;
	down[LEFT_F].PushBack({ 144, 768, 144, 192 });
	down[LEFT_F].PushBack({ 144, 960, 144, 192 });
	down[LEFT_F].speed = 0.25f;
	down[LEFT_MIDDLE_F].PushBack({ 288, 768, 144, 192 });
	down[LEFT_MIDDLE_F].PushBack({ 288, 960, 144, 192 });
	down[LEFT_MIDDLE_F].speed = 0.25f;
	down[MIDDLE_F].PushBack({ 432, 768, 144, 192 });
	down[MIDDLE_F].PushBack({ 432, 960, 144, 192 });
	down[MIDDLE_F].speed = 0.25f;
	down[RIGHT_MIDDLE_F].PushBack({ 576, 768, 144, 192 });
	down[RIGHT_MIDDLE_F].PushBack({ 576, 960, 144, 192 });
	down[RIGHT_MIDDLE_F].speed = 0.25f;
	down[RIGHT_F].PushBack({ 720, 768, 144, 192 });
	down[RIGHT_F].PushBack({ 720, 960, 144, 192 });
	down[RIGHT_F].speed = 0.25f;
	down[FAR_RIGHT_F].PushBack({ 864, 768, 144, 192 });
	down[FAR_RIGHT_F].PushBack({ 864, 960, 144, 192 });
	down[FAR_RIGHT_F].speed = 0.25f;

	walk_right.PushBack({ 0, 1152, 144, 192 });
	walk_right.PushBack({ 144, 1152, 144, 192 });
	walk_right.PushBack({ 288, 1152, 144, 192 });
	walk_right.speed = 0.2f;

	walk_left.PushBack({ 864, 1152, 144, 192 });
	walk_left.PushBack({ 720, 1152, 144, 192 });
	walk_left.PushBack({ 576, 1152, 144, 192 });
	walk_left.speed = 0.2f;

	roll_right.PushBack({ 0, 1344, 240, 192 });
	roll_right.PushBack({ 240, 1344, 240, 192 });
	roll_right.PushBack({ 480, 1344, 240, 192 });
	roll_right.PushBack({ 720, 1344, 240, 192 });
	roll_right.PushBack({ 0, 1536, 240, 192 });
	roll_right.PushBack({ 240, 1536, 240, 192 });
	roll_right.PushBack({ 480, 1536, 240, 192 });
	roll_right.PushBack({ 720, 1536, 240, 192 });
	roll_right.speed = 0.15f;
	roll_right.loop = false;

	roll_left.PushBack({ 0, 1728, 240, 192 });
	roll_left.PushBack({ 240, 1728, 240, 192 });
	roll_left.PushBack({ 480, 1728, 240, 192 });
	roll_left.PushBack({ 720, 1728, 240, 192 });
	roll_left.PushBack({ 0, 1920, 240, 192 });
	roll_left.PushBack({ 240, 1920, 240, 192 });
	roll_left.PushBack({ 480, 1920, 240, 192 });
	roll_left.PushBack({ 720, 1920, 240, 192 });
	roll_left.speed = 0.15f;
	roll_left.loop = false;

	dance1.PushBack({ 0, 2304, 96, 192 });
	dance1.PushBack({ 96, 2304, 96, 192 });
	dance1.speed = 0.1f;
														//first part of the dance
	dance2.PushBack({ 192, 2352, 48, 144 });
	dance2.PushBack({ 240, 2352, 48, 144 });
		//second part
	dance2.speed = 0.1f;


	dance3.PushBack({ 288, 2400, 48, 96 });
	dance3.PushBack({ 336, 2400, 48, 96 });
	dance3.speed = 0.1f;

	dance4.PushBack({ 384, 2448, 48, 48 });
	dance4.PushBack({ 432, 2448, 48, 48 });
	dance4.speed = 0.2f;

	dance5.PushBack({ 480, 2448, 48, 48 });
	dance5.PushBack({ 528, 2448, 48, 48 });
	dance5.speed = 0.2f;



	dead.PushBack({ 0, 2112, 192, 192 });
	dead.PushBack({ 192, 2112, 192, 192 });
	dead.PushBack({ 384, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.PushBack({ 576, 2112, 192, 192 });
	dead.loop = false;
	dead.speed = 0.1f;

    godbutt.PushBack({ 576, 2400, 336, 48 });
	godbutt.PushBack({ 576, 2448, 336, 48 });
	godbutt.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	player = App->textures->Load("sprites/p1_sprites-rearranged.png");
	crosstexture = App->textures->Load("sprites/aims.png"); 
	shoot = App->audio->LoadSFX("sound/soundfx/shoot.wav");
	hit_sound = App->audio->LoadSFX("sound/soundfx/player_hit.wav");
	cross_collider = App->collision->AddCollider({ SCREEN_WIDTH/2, SCREEN_HEIGHT, 23, 21 }, COLLIDER_PLAYER_SHOT);
	player_collider = App->collision->AddCollider({ (SCREEN_WIDTH - 87) / 2, SCREEN_HEIGHT / 2 + 117, TILE, (TILE*4)-8 }, COLLIDER_PLAYER); 
	ground_collider = App->collision->AddCollider({ 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50 }, COLLIDER_PLAYER_SHOT);
	state = ST_IDLE;
	shooting_cooldown = 0;
	damage_cooldown = 0;

	position.x = (SCREEN_WIDTH - 87) / 2;    //Initial x position of the player and crossbow, 87 is sprite width
	position.y = SCREEN_HEIGHT / 2 + 117;
	cposition.x = (SCREEN_WIDTH - 60) / 2;
	cposition.y = position.y - 150;
	

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(player);
	App->textures->Unload(crosstexture);

	App->audio->UnloadSFX(shoot);
	App->audio->UnloadSFX(hit_sound);

	App->collision->EraseCollider(cross_collider);
	App->collision->EraseCollider(player_collider);
	App->collision->EraseCollider(ground_collider);


	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 4;
	int xcorrection = 0;
	int ycorrection = 0; // x and y correction for temporary sprite changes
	bool firing = false;
	position.y = SCREEN_HEIGHT / 2 + 117;
	int screen_portion = portion_calculate();
	cross_collider->SetPos(SCREEN_WIDTH / 2 , SCREEN_HEIGHT);
	player_collider->SetPos(position.x+TILE, position.y+8);
	current_time = SDL_GetTicks();
	if (godmode)
	{
		hit = false;
		App->render->Blit(player, position.x - 96, position.y - 48, &(godbutt.GetCurrentFrame()));
	}
	if (current_time >= blink_time)
	{
		blink = !blink;
		blink_time = current_time + 100;
	}

	if (current_time < damage_cooldown)
		hit = false;

	
	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_STATE::KEY_DOWN)
		godmode = !godmode;

	switch (state)
	{
	case ST_IDLE:
		if (current_animation == &roll_right)
			position.x += 240 - 144;
		current_animation = &idle[screen_portion];
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			state = ST_WALK_LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			state = ST_WALK_RIGHT;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT && App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_REPEAT)
			state = ST_FIRE_STANDING;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
			state = ST_CROUCH;
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
		{
			if (cposition.x > position.x)
				state = ST_ROLLING_RIGHT;
			else
				state = ST_ROLLING_LEFT;
		}
		if (hit)
			state = ST_DEATH;

		break;

	case ST_CROUCH:
		current_animation = &down[screen_portion];
		ycorrection += 25;
		if (cposition.y < position.y + 15)
			cposition.y += speed * 2;

		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_REPEAT)
			state = ST_FIRE_CROUCH;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_UP)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			state = ST_WALK_LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			state = ST_WALK_RIGHT;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
		{
			if (cposition.x > position.x)
				state = ST_ROLLING_RIGHT;
			else
				state = ST_ROLLING_LEFT;
		}
		if (hit)
			state = ST_DEATH;
		break;

	case ST_WALK_LEFT:
		if (position.x > 0)
			position.x -= speed;
		if (cposition.x > -35)
			cposition.x -= speed * 2;
		current_animation = &walk_left;

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_UP)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y < position.y + 15)
				cposition.y += speed * 2;
		}
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_REPEAT)
			state = ST_FIRE_STANDING;
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
			state = ST_ROLLING_LEFT;
		if (hit)
			state = ST_DEATH;
		break;

	case ST_WALK_RIGHT:
		if (position.x < SCREEN_WIDTH - 144)
			position.x += speed;
		if (cposition.x < SCREEN_WIDTH - 35)
			cposition.x += speed * 2;
		current_animation = &walk_right;

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_UP)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y < position.y + 15)
				cposition.y += speed * 2;
		}
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_REPEAT)
			state = ST_FIRE_STANDING;
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
			state = ST_ROLLING_RIGHT;
		if (hit)
			state = ST_DEATH;
		break;

	case ST_FIRE_STANDING:
		if (current_time > shooting_cooldown)
		{
			shooting_cooldown = current_time + 200;
			cross_collider->SetPos(cposition.x + 23, cposition.y + 23);
			App->audio->PlaySFX(shoot);
		}
		screen_portion += 7;
		if (screen_portion <= MIDDLE_F)
			xcorrection += idle[screen_portion - 7].frames[0].w - idle[screen_portion].frames[0].w;
		ycorrection += idle[screen_portion - 7].frames[0].h - idle[screen_portion].frames[0].h;

		current_animation = &idle[screen_portion];

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			if (cposition.x > -35)
				cposition.x -= speed * 2;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			if (cposition.x < SCREEN_WIDTH - 35)
				cposition.x += speed * 2;
		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_UP)
			state = ST_IDLE;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_REPEAT)
			state = ST_FIRE_CROUCH;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
		{
			if (cposition.x > position.x)
				state = ST_ROLLING_RIGHT;
			else
				state = ST_ROLLING_LEFT;
		}
		if (hit)
			state = ST_DEATH;
		break;

	case ST_FIRE_CROUCH:
		if (current_time > shooting_cooldown)
		{
			shooting_cooldown = current_time + 200;
			cross_collider->SetPos(cposition.x, cposition.y);
			App->audio->PlaySFX(shoot);
		}
		screen_portion += 7;
		if (screen_portion <= MIDDLE_F)
			xcorrection += idle[screen_portion - 7].frames[0].w - idle[screen_portion].frames[0].w;
		ycorrection += idle[screen_portion - 7].frames[0].h - idle[screen_portion].frames[0].h;

		current_animation = &down[screen_portion];

		if (cposition.y < position.y + 15)
			cposition.y += speed * 2;

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
			if (cposition.x > -35)
				cposition.x -= speed * 2;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
			if (cposition.x < SCREEN_WIDTH - 35)
				cposition.x += speed * 2;
		if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_UP)
			state = ST_CROUCH;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_STATE::KEY_UP)
			state = ST_FIRE_STANDING;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}
		if (App->input->keyboard[SDL_SCANCODE_LALT] == KEY_STATE::KEY_DOWN)
		{
			if (cposition.x > position.x)
				state = ST_ROLLING_RIGHT;
			else
				state = ST_ROLLING_LEFT;
		}
		if (hit)
			state = ST_DEATH;
		break;

	case ST_DEATH:
		current_animation = &dead;
		if (hit)
		{
			App->useri->hitpoints -= 1;
			hit = false;
		}

		if (dead.Finished())
		{
			dead.Reset();
			damage_cooldown = current_time + 3000;
			state = ST_IDLE;
		}
		break;

	case ST_ROLLING_LEFT:
		current_animation = &roll_left;
		if (position.x > 0)
			position.x -= speed;

		if (roll_left.Finished())
		{
			roll_left.Reset();
			state = ST_IDLE;
			hit = false;
		}
		break;

	case ST_ROLLING_RIGHT:
		if (current_animation != &roll_right)
			position.x -= 240 - 144;
		player_collider->SetPos(position.x + TILE + (240 - 144), position.y);
		current_animation = &roll_right;
		if (position.x + 240 < SCREEN_WIDTH)
			position.x += speed;

		if (roll_right.Finished())
		{
			/*position.x += TILE * 2;
			if (position.x > SCREEN_WIDTH - 144)
			position.x = SCREEN_WIDTH - 144;*/
			roll_right.Reset();
			state = ST_IDLE;
			hit = false;
		}
		break;

	case ST_DANCE:
		if (current_time < dance_start + 3000)
		{
			current_animation = &dance1;
			if (position.x < (SCREEN_WIDTH - 87) / 2)
				position.x += speed;
			else
				position.x -= speed;
		}
		else if (current_time < dance_start + 4000)
		{
			position.x = SCREEN_WIDTH / 2 - TILE*4;
			position.y = SCREEN_HEIGHT / 2;
			current_animation = &dance2;
		}
		else if (current_time < dance_start + 5000)
		{
			position.x = SCREEN_WIDTH / 2 + TILE*2;
			position.y = SCREEN_HEIGHT / 2 - 100;
			current_animation = &dance3;
		}
		else if (current_time < dance_start + 6000)
		{
			position.x = SCREEN_WIDTH / 2 + TILE;
			position.y = SCREEN_HEIGHT / 2-80;
			current_animation = &dance4;
		}

		else if (current_time < dance_start + 6500)
		{
			position.x = SCREEN_WIDTH / 2- 55;
			position.y = SCREEN_HEIGHT / 2 - 100;
			current_animation = &dance5;
		}
		else
		{
			App->fade->FadeToBlack(App->scene, App->end);
		}

	}

		/*if (App->input->keyboard[SDL_SCANCODE_LCTRL] == KEY_STATE::KEY_REPEAT)
		{
			cross_collider->SetPos(cposition.x, cposition.y);
			screen_portion += 7;
			App->audio->PlaySFX(shoot);
			firing = true;
			if (screen_portion <= MIDDLE_F)
				xcorrection += idle[screen_portion - 7].frames[0].w - idle[screen_portion].frames[0].w;
			ycorrection += idle[screen_portion - 7].frames[0].h - idle[screen_portion].frames[0].h;
		}

		current_animation = &idle[screen_portion];

		if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y < position.y + 15)
				cposition.y += speed * 2;

			if (App->input->keyboard[SDL_SCANCODE_W] != KEY_STATE::KEY_REPEAT)
			{
				current_animation = &down[screen_portion];
				ycorrection += 25;
			}

		}

		if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
		{
			if (cposition.y > -35)
				cposition.y -= speed * 2;

		}

		if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
		{
			if (position.x > 0 && !firing)
				position.x -= speed;
			if (cposition.x > -35) //middle of the cross
				cposition.x -= speed * 2;

			if (App->input->keyboard[SDL_SCANCODE_D] != KEY_STATE::KEY_REPEAT && !firing)
				current_animation = &walk_left;
		}

		if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
		{
			if (position.x < SCREEN_WIDTH - 87 && !firing)
				position.x += speed;
			if (cposition.x < SCREEN_WIDTH - 35) //the middle of the cross
				cposition.x += speed * 2;

			if (App->input->keyboard[SDL_SCANCODE_A] != KEY_STATE::KEY_REPEAT && !firing)
				current_animation = &walk_right;
		}*/
	/*else
	{
		current_animation = &dead;
		if (dead.Finished())
		{
			App->useri->hitpoints -= 1;
			hit = false;
			dead.Reset();
		}
	}*/

	// Draw everything --------------------------------------
	
	App->render->Blit(crosstexture, cposition.x, cposition.y, &(cross.GetCurrentFrame()));
	if (state == ST_FIRE_STANDING || state == ST_FIRE_CROUCH)
		App->render->Blit(crosstexture, cposition.x, cposition.y, &(fcross.GetCurrentFrame()));
	if (current_time >= damage_cooldown || blink)
		App->render->Blit(player, position.x + xcorrection, position.y + ycorrection, &(current_animation->GetCurrentFrame()));
	else
		current_animation->GetCurrentFrame();

	return UPDATE_CONTINUE;
}

void ModulePlayer::Collision()
{
	if (!hit && !godmode)
	{
		hit = true;
		App->audio->PlaySFX(hit_sound);
	}
}