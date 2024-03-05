#pragma once
#include <vector>
struct SDL_Rect;
class Blockade;
class Entity;
class Enemy;

class Projectile
{
public:

	Projectile();
	Projectile(int projectile_type);
	Projectile(SDL_Rect* projectile_rect, int projectile_type);
	Projectile(SDL_Rect* projectile_rect, int projectile_type, int horizontal_delta);
	enum {ZERO, SINGLE, BURST, SPREAD, EXPLOSIVE} projectile_enum;
	void set_projectile_rect(SDL_Rect* rect);
	void load_projectile(const Entity* entity_obj);
	static void load_projectile(const Entity* projectile_owner, std::vector<Projectile*>& projectile_vector, int projectile_type);
	void set_projectile_velocity(int projectile_velocity);
	void set_projectile_type(int projectile_type);
	void destroy_projectile();
	void shoot();
	void shoot_downwards();
	void increment_projectile(int delta, Blockade* blockade_1 = nullptr, Blockade* blockade_2 = nullptr, Blockade* blockade_3 = nullptr, Blockade* blockade_4 = nullptr, Enemy
		* UFO = nullptr);
	void increment_projectile(unsigned game_timer = 0, int origin_x_pos = 0, int origin_y_pos = 0);
	void render_projectile() const;
	int get_projectile_type() const;
	SDL_Rect* get_projectile_rect() const;
	int get_projectile_velocity() const;
	int get_health() const;
	void set_health(int health);
	static std::vector<Projectile*>& get_player_projectiles_vector();
	static std::vector<Projectile*>& get_enemy_projectiles_vector();
private:
	int m_projectile_type = SINGLE;
	int m_health = 1;
	unsigned projectile_timer = 0;
	bool m_is_active_projectile = false;
	bool m_is_shooting_downwards = false;
	bool m_is_shooting_right = false;
	bool m_is_shooting_left = false;
	bool m_is_player_projectile = true;
	int m_projectile_velocity;
	int m_vertical_delta = 0;
	int m_horizontal_delta = 0;
	SDL_Rect* m_projectile_rect;
	static std::vector<Projectile*> s_player_projectiles_vector;
	static std::vector<Projectile*> s_enemy_projectiles_vector;
};

