#pragma once
#include <SDL_rect.h>
#include <SDL_timer.h>
#include <vector>
#include "Entity.h"

struct SDL_Rect;
struct SDL_Surface;

class Blockade
{

public:
	Blockade();
	Blockade(const char* file_path);
	Blockade(const char* file_path, int x_pos, int y_pos);
	~Blockade();
	void set_position(int x, int y);
	bool search_surface_pixels();
	int resize_blockade(int height);
	int get_surface_width() const;
	int get_min_x_pos() const;
	int get_max_x_pos() const;
	static void destroy_blockade_pieces();
	std::vector<SDL_Rect*>& get_blockade_vector();
	void render();
	static void set_destroyed_block(SDL_Rect* destroyed_block);
	static SDL_Rect* s_destroyed_block;
	static void is_within_blast_range(SDL_Rect* nearby_block);
	void clean_blockade_vector();
	int get_max_x_pos();
	int get_min_x_pos();
private:
	static std::vector<SDL_Rect*> s_destroy_rand_block_vector;
	static int max_size;
	int surface_width;
	int m_x_pos, m_y_pos;
	int m_min_x_pos = 1000000, m_max_x_pos = 0;
	SDL_Surface* m_blockade_surface;
	std::vector<SDL_Rect*> m_blockade_vector;
};


