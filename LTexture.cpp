#include "LTexture.h"
#include <cstdio>
#include "Render.h"
#include "Window.h"
#include <SDL_image.h>
#include <SDL.h>

LTexture::LTexture()
{
	m_texture = nullptr;
	m_surface = nullptr;

}

LTexture::LTexture(const char* file_path) : m_file_path(file_path)
{
	m_texture = load_from_file(file_path);
}

LTexture::~LTexture()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}

	if (m_surface != nullptr)
	{
		SDL_FreeSurface(m_surface);
		m_surface = nullptr;
	}
}

SDL_Texture* LTexture::load_from_file(const char* file_path)
{
	
	SDL_Surface* temp_surface = IMG_Load(file_path);
	SDL_SetColorKey(temp_surface, SDL_TRUE, SDL_MapRGB(temp_surface->format, 0, 0, 0));

	m_texture = SDL_CreateTextureFromSurface(Render::get_renderer(), temp_surface);

	SDL_FreeSurface(temp_surface);
	temp_surface = nullptr;
	
	if (m_texture == nullptr)
	{
		printf("Couldn't load texture into LTexture. Error: %s\n", SDL_GetError());
	}

	return m_texture;

}

SDL_Texture* LTexture::get_texture() const	
{
	if (!m_texture)
		printf("m_texture is null\n");

	return m_texture;

}

void LTexture::resize(int width, int height)
{
	if (m_texture)
	{
		if (m_file_path)
		{
			SDL_DestroyTexture(m_texture);
			SDL_FreeSurface(m_surface);
			m_surface = IMG_Load(m_file_path);

			if (height < 10) { height = 10; }
			const int dimension_ratio = m_surface->w / m_surface->h;

			SDL_Surface* temp_surface = {SDL_CreateRGBSurface(0, width * dimension_ratio, height, 16, 0, 0, 0, 0)};
			SDL_UpperBlitScaled(m_surface, nullptr, temp_surface, nullptr);
			SDL_SetColorKey(temp_surface, SDL_TRUE, SDL_MapRGB(temp_surface->format, 0, 0, 0));
			SDL_DestroyTexture(m_texture);
			m_texture = SDL_CreateTextureFromSurface(Render::get_renderer(), temp_surface);
			SDL_FreeSurface(temp_surface);
			SDL_FreeSurface(m_surface);
			m_surface = nullptr;
			temp_surface = nullptr;
		}
		
	}

	
}

LTexture* LTexture::create_l_texture(const char* file_path)
{
	return new LTexture(file_path);
}
