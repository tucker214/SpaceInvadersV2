#pragma once
#include <SDL_rect.h>

struct _TTF_Font;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;
class Entity;
class Player;
class Enemy;
class Blockade;

class Game
{
public:
	Game();
	~Game();

	void update();
	void render();
	void collision();
	static Player* get_player();
	static bool get_is_game_finished();
	static bool get_is_game_won();
	static void set_game_finished();
	void load_font(const char* file_path);
	void create_surface_from_font(const char* text);
	void render_score();
	void render_player_death();
	int get_casualties() const;


private:

	Blockade* m_blockade_1;
	Blockade* m_blockade_2;
	Blockade* m_blockade_3;
	Blockade* m_blockade_4;
	static Player* s_player;
	Enemy* m_enemies;
	Enemy* m_UFO;
	Entity* m_UFO_power_up;
	_TTF_Font* m_font;
	SDL_Surface* m_surface_text;
	SDL_Texture* m_texture_text;
	static bool s_is_game_finished;
	static bool s_is_game_won;
	static int s_player_x_pos;
	static int s_level;
	bool m_player_died;
	int m_enemy_casualties = 0;
	int player_death_timer = 420;
	static int s_score;
	int UFO_cooldown = 25000;
	bool m_should_revive;
	unsigned int game_timer;

};

