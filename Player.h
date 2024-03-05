#pragma once
#include <SDL_rect.h>

#include "Entity.h"
struct SDL_Rect;
class Projectile;
class Blockade;
class Enemy;

class Player : public Entity
{
public:
	Player(const char* file_path);
	static void set_sprite_sheet(LTexture* sprite_sheet);
	static LTexture* get_sprite_sheet();
	void shoot();
	void reset_projectile();
	SDL_Rect* get_projectile() const;
	Projectile* get_player_projectile() const;
	void destroy_projectile();
	void set_projectile_type(int projectile_type);
	void add_player_velocity(int velocity, int scalar = 1);
	void clamp_position(int window_width);
	bool b_destroy_projectile();
	void handle_input(int window_width);
	void update(unsigned& game_timer);
	void set_projectile_velocity(int projectile_velocity);
	int get_lives() const;
	int get_projectile_type() const;
	unsigned get_projectile_power_timer() const;
	SDL_Rect*& get_power_up_bar();
	void set_projectile_power_timer(unsigned int time);
	void reset_power_up_timer();
	void decrement_lives();
	static bool is_active_projectile;
	bool is_cheating() const;
	void reset_cheat();
	void reset_lives();
private:
	Projectile* m_player_projectile;
	static LTexture* s_destroyed_player_sheet;
	SDL_Rect* m_projectile;
	SDL_Rect* m_power_up_rect;
	unsigned int m_power_up_timer = 0;
	int m_projectile_velocity_multiplier = 1;
	static int s_lives;

	int m_projectile_type;
	bool m_cheat = false;
};

