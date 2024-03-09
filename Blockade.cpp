#include "Blockade.h"
#include <math.h>
#include <SDL_image.h>
#include "Window.h"
#include "Collision.h"
#include "Render.h"
#include <SDL.h>

std::vector<SDL_Rect*> Blockade::s_destroy_rand_block_vector;
SDL_Rect* Blockade::s_destroyed_block = nullptr;
int Blockade::max_size;

Blockade::Blockade(const char* file_path) : m_x_pos(0), m_y_pos(0)
{
	m_blockade_surface = IMG_Load(file_path);

}

Blockade::Blockade(const char* file_path, int x_pos, int y_pos) : m_x_pos(x_pos), m_y_pos(y_pos)
{
	m_blockade_surface = IMG_Load(file_path);
	
}

Blockade::~Blockade()
{
	SDL_FreeSurface(m_blockade_surface);
	for (int index = 0; index < static_cast<int>(m_blockade_vector.size());)
	{
		if (m_blockade_vector[index])
		{
			delete m_blockade_vector[index];
			m_blockade_vector[index] = nullptr;
		}
		else
			index++;
	}
	m_blockade_vector.clear();
}

void Blockade::set_position(int x, int y)
{
	m_x_pos = x;
	m_y_pos = y;
}

std::vector<SDL_Rect*>& Blockade::get_blockade_vector()
{
	return m_blockade_vector;
}


bool Blockade::search_surface_pixels()
{


	SDL_PixelFormat* fmt = m_blockade_surface->format;
	SDL_LockSurface(m_blockade_surface);
	Uint32* pixels = static_cast<Uint32*>(m_blockade_surface->pixels);
	Uint32* pixel;
	Uint32 temp_pixel;
	Uint8 green;
	int dimension_ratio = m_blockade_surface->w / m_blockade_surface->h;
	const int width = m_blockade_surface->w;
	const int height = m_blockade_surface->h;
	if (m_blockade_surface)
	{
		if (fmt->BytesPerPixel != 4)
			return false;
		
		for (int row = 0; row < m_blockade_surface->h; row = row + height / 64)
		{
			for (int col = 0; col < m_blockade_surface->w; col = col + width / 64)
			{

				pixel = (pixels + row * (m_blockade_surface->pitch / 4) + col);

				temp_pixel = *pixel & fmt->Gmask;
				temp_pixel = temp_pixel >> fmt->Gshift;
				temp_pixel = temp_pixel << fmt->Gloss;
				green = static_cast<Uint8>(temp_pixel);
				
				if (green)
				{
					m_blockade_vector.push_back(new SDL_Rect{ col + m_x_pos, row + m_y_pos, width / 64, height / 64 });
					if (col + m_x_pos < m_min_x_pos) { m_min_x_pos = col + m_x_pos; }
					if (col + m_x_pos > m_max_x_pos) { m_max_x_pos = col + m_x_pos; }
				}

			}
		}
	}
	SDL_UnlockSurface(m_blockade_surface);

	m_x_pos -= m_x_pos - m_blockade_vector[0]->x;
	SDL_FreeSurface(m_blockade_surface);
	m_blockade_surface = nullptr;
	if (s_destroy_rand_block_vector.max_size() == 0) { s_destroy_rand_block_vector.reserve(m_blockade_vector.max_size()); }
	if (max_size < static_cast<int>(m_blockade_vector.size())) { max_size = static_cast<int>(m_blockade_vector.size()); }

	if (m_blockade_vector.empty())
	{
		printf("blockade couldn't be created using SDL_surface pixels.");
		return false;
	}

	return true;
}

int Blockade::resize_blockade(int height)
{
	if (height < 10) { height = 10; }

	int dimension_ratio = m_blockade_surface->w / m_blockade_surface->h;
	SDL_Surface* temp_surface = { SDL_CreateRGBSurface(0, height * dimension_ratio, height, 32, 0, 0, 0, 0) };
	SDL_UpperBlitScaled(m_blockade_surface, nullptr, temp_surface, nullptr);
	SDL_FreeSurface(m_blockade_surface);


	m_blockade_surface = temp_surface;

	if (m_blockade_surface)
	{
		surface_width = m_blockade_surface->w;
		return 1;
	}


	return 0;
}

int Blockade::get_surface_width() const
{
	return surface_width;
}

int Blockade::get_min_x_pos() const
{
	return m_min_x_pos;
}

int Blockade::get_max_x_pos() const
{
	return m_max_x_pos;
}

//void Blockade::destroy_blockade_pieces()
//{
//	int destroy_count = max_size / 4;
//	if (destroy_count == 0) { destroy_count = 1; }
//
//	while (destroy_count > 0 && !s_destroy_rand_block_vector.empty())
//	{
//		s_destroy_rand_block_vector[SDL_GetTicks() % s_destroy_rand_block_vector.size()];
//	}
//}


void Blockade::render()
{
	for (SDL_Rect*& it : get_blockade_vector())
	{
		if (it)
		{
			Render::render_rect(it, {0, 255, 0, 255});
		}
	}
}

void Blockade::set_destroyed_block(SDL_Rect* destroyed_block)
{
	s_destroyed_block = destroyed_block;
}

void Blockade::is_within_blast_range(SDL_Rect* nearby_block)
{
	//const int screen_ratio = Window::win_surface()->h / Window::win_surface()->w;
	int destroy_range_x = Window::win_surface()->w / 100;
	int destroy_range_y = destroy_range_x * 2;
	if (destroy_range_x == 0) { destroy_range_x = 1; destroy_range_y = 1; }


	if (!s_destroyed_block) { }

	else if (abs(nearby_block->x - s_destroyed_block->x) < destroy_range_x)
	{

		if (abs(nearby_block->y - s_destroyed_block->y) < destroy_range_y)
		{
			
			if (rand() % 10 > 3) { nearby_block->x = -1000; }
		}
	}
}

void Blockade::clean_blockade_vector()
{

	for (int index = 0; index < static_cast<int>(m_blockade_vector.size());)
	{
		if (m_blockade_vector[index])
		{
			if (m_blockade_vector[index]->x <= -1000)
			{
				delete m_blockade_vector[index];
				m_blockade_vector[index] = nullptr;
				m_blockade_vector.erase(m_blockade_vector.begin() + index);
			}

			else index++;
		}

		else index++;
	}
}

int Blockade::get_max_x_pos()
{
	return m_max_x_pos;
}

int Blockade::get_min_x_pos()
{
	return m_min_x_pos;
}
