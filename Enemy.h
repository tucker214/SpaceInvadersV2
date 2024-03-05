#pragma once
#include <SDL_rect.h>
#include <SDL_stdinc.h>
#include <vector>
#include "Entity.h"

class Player;
class Projectile;
struct SDL_Rect;


class Enemy : public Entity
{
public:
	Enemy();
	Enemy(int enemy);
	Enemy(const char* file_path);
	~Enemy() override;

	static bool alternate_texture;
	bool is_moving_right;
	static enum {ZERO, CUTTLE, SQUID, CRAB, OCTOPUS, UFO} enemy_enum;
	static Enemy* enemy_arr[5 * 11];


	//FUNCTIONS
	static void format_formation(Enemy* arr[55]);
	static bool s_is_colliding_player;
	void shift_formation(Enemy* arr[][11], int rows, int cols, Player*& player);
	void shift_formation(Enemy* const(&arr)[55]);
	static void update_UFO(Enemy*& param_UFO, Uint32& timer_begin, Uint32& game_timer, Uint32 UFO_cooldown);
	static LTexture* get_sprite_sheet();
	static void set_sprite_sheet(LTexture* l_texture);
	void shift_enemy_vector_formation(const std::vector<Enemy*>& formation);
	void set_ufo_direction();
	void update_enemy_array(Uint32& time_begin, const int& casualties);
	SDL_Rect* get_image_rect() const;
	void set_image_rect(SDL_Rect* image_rect);
	void increment_x_position();
	void decrement_x_position();
	void increment_y_position();
	int get_enemy_score() const;
	int get_enemy_type() const;
	void set_x_increment(int x_delta);
	void set_y_increment(int y_delta);
	void random_enemy_shoot();
	void decrement_shoot_timer();
	unsigned int get_death_timer() const;
	void update_death_timer();

private:
	static LTexture* s_enemy_sprite_sheet;
	SDL_Rect* m_image_rect;
	int m_shooting_cooldown = 0;
	int m_increment_x_amount;
	int m_increment_y_amount;
	int m_score = 0;
	int m_type = 0;
	unsigned int m_death_timer = 0;
};

