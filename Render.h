#pragma once
#include <SDL_render.h>
#include <vector>

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Rect;
class Entity;
class Enemy;
class Player;
class LTexture;

class Render
{
public:
	~Render();
	static Render& get();
	static void create_renderer();
	static SDL_Renderer* get_renderer();
	static void render_sprite(LTexture* sprite_texture, const int& x_pos = 0, const int& y_pos = 0);
	static void render_rect(const SDL_Rect* const& rect, SDL_Color color);
	static void render_score(SDL_Texture* texture_text);
	static void render_enemy(Enemy*& enemy);
	static void render_player_destroyed(Player*& player, int sprite_number);
	static void render_formation(const std::vector<Enemy*>& formation);

private:
	Render(){ }
	static SDL_Renderer* s_renderer;
};




