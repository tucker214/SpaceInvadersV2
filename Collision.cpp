#include "Collision.h"

#include <memory>
#include <SDL_render.h>
#include "Enemy.h"
#include "Entity.h"
#include "Player.h"
#include "LTexture.h"

bool Collision::is_colliding(SDL_Rect* rect_1, SDL_Rect* rect_2)
{
	if (rect_2->w + rect_2->x < rect_1->x)
		return false;

	if (rect_2->x > rect_1->x + rect_1->w)
		return false;

	if (rect_2->y + rect_2->h < rect_1->y)
		return false;

	if (rect_2->y > rect_1->y + rect_1->h)
		return false;

	return true;
}

bool Collision::is_colliding(SDL_Rect* rect, SDL_Texture* texture)
{

	return false;
}

bool Collision::is_colliding(Entity* entity_1, Entity* entity_2)
{
	int texture_1_width;
	int texture_1_height;
	int texture_2_width;
	int texture_2_height;
	SDL_QueryTexture(entity_1->get_l_texture()->get_texture(), nullptr, nullptr, &texture_1_width, &texture_1_height);
	SDL_QueryTexture(entity_2->get_l_texture()->get_texture(), nullptr, nullptr, &texture_2_width, &texture_2_height);

	if (texture_2_width + entity_2->get_x_pos() < entity_1->get_x_pos())
		return false;
	
	if (entity_2->get_x_pos() > (texture_1_width) + entity_1->get_x_pos())
		return false;

	if (texture_2_height + entity_2->get_y_pos() < entity_1->get_y_pos())
		return false;

	if (entity_2->get_y_pos() > (texture_1_height) + entity_1->get_y_pos())
		return false;

	return true;
}

bool Collision::is_colliding(SDL_Rect* rect, Enemy* enemy)
{
	int enemy_texture_width;
	int enemy_texture_height;

	SDL_QueryTexture(enemy->get_l_texture()->get_texture(), nullptr, nullptr, &enemy_texture_width, &enemy_texture_height);

	if (rect && enemy)
	{
		if (rect->x + rect->w < enemy->get_x_pos())
			return false;
		if (rect->y + rect->h < enemy->get_y_pos())
			return false;
		if (rect->x > enemy->get_x_pos() + enemy->get_width())
			return false;
		if (rect->y > enemy->get_y_pos() + enemy->get_height())
			return false;
	}

	return true;
}

bool Collision::is_colliding(SDL_Rect* rect, Player* player)
{
	int enemy_texture_width;
	int enemy_texture_height;

	SDL_QueryTexture(player->get_l_texture()->get_texture(), nullptr, nullptr, &enemy_texture_width, &enemy_texture_height);

	if (rect && player)
	{
		if (rect->x + rect->w < player->get_x_pos())
			return false;
		if (rect->y + rect->h < player->get_y_pos())
			return false;
		if (rect->x > player->get_x_pos() + player->get_width())
			return false;
		if (rect->y > player->get_y_pos() + player->get_height())
			return false;
	}

	return true;
}

bool Collision::is_colliding_enemy_rect(SDL_Rect* rect, Enemy* enemy)
{

	if (rect && enemy)
	{
		if (rect->x + rect->w < enemy->get_image_rect()->x)
			return false;
		if (rect->y + rect->h < enemy->get_image_rect()->y)
			return false;
		if (rect->x > enemy->get_image_rect()->x + enemy->get_image_rect()->w)
			return false;
		if (rect->y > enemy->get_image_rect()->y + enemy->get_image_rect()->h)
			return false;
	}

	return true;
}

bool Collision::is_colliding_func_ptr(SDL_Rect* rect, Enemy (*&enemy_arr)[11], void* func)
{


	return false;
}


bool Collision::is_colliding_y_axis( Enemy* enemy, Entity* player)
{
	if (player && enemy)
	{
		if (enemy->get_y_pos() + enemy->get_height() < player->get_y_pos())
			return false;
	}

	return true;
}

bool Collision::is_colliding_y_axis(SDL_Rect* rect_1, Entity* player)
{
	if (player && rect_1)
	{
		if (rect_1->y + rect_1->h < player->get_y_pos())
			return false;
	}

	return true;
}

void Collision::update(Collision(*)[11], Player* player)
{
}
