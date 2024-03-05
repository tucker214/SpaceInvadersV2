#pragma once
#include <SDL_rect.h>
struct SDL_Rect;
struct SDL_Texture;
class Entity;
class Enemy;
class Player;
class Collision
{
public:
	static bool is_colliding(SDL_Rect* rect_1, SDL_Rect* rect_2);
	static bool is_colliding(SDL_Rect* rect, SDL_Texture* texture);
	static bool is_colliding(Entity* entity_1, Entity* entity_2);
	static bool is_colliding(SDL_Rect* rect, Enemy* enemy);
	static bool is_colliding(SDL_Rect* rect, Player* player);
	static bool is_colliding_enemy_rect(SDL_Rect* rect, Enemy* enemy);
	static bool is_colliding_func_ptr(SDL_Rect* rect, Enemy (*&enemy_arr)[11], void *func);
	static bool is_colliding_y_axis(Enemy* enemy, Entity* player);
	static bool is_colliding_y_axis(SDL_Rect* rect_1, Entity* player);
	static void update(Collision(*)[11], Player* player);
};

