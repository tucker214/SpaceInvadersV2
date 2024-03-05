#include "Render.h"
#include <cstdio>
#include <queue>
#include "Enemy.h"
#include "Entity.h"
#include <SDL.h>
#include "Window.h"
#include "LTexture.h"
#include "Player.h"

SDL_Renderer* Render::s_renderer;

Render::~Render()
{
	if (s_renderer != nullptr)
	{
		SDL_DestroyRenderer(s_renderer);
		s_renderer = nullptr;
	}
}

Render& Render::get()
{
	static Render s_render_instance;
	return s_render_instance;
}


void Render::create_renderer()
{
	if (get().s_renderer == nullptr)
	{
		get().s_renderer = SDL_CreateRenderer(Window::get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		printf("Renderer successfully created\n");
	}
	else
	{
		printf("Render instance already exists. Couldn't create renderer\n");
	}

}

SDL_Renderer* Render::get_renderer()
{
	return get().s_renderer;
}

void Render::render_sprite(LTexture* sprite_texture, const int& x_pos, const int& y_pos)
{
	int width = 0;
	int height = 0;

	SDL_QueryTexture(sprite_texture->get_texture(), nullptr, nullptr, &width, &height);

	{
		const SDL_Rect temp_rect = {x_pos,y_pos, width, height};
		SDL_SetTextureBlendMode(sprite_texture->get_texture(), SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(get_renderer(), sprite_texture->get_texture(), nullptr, &temp_rect);
	}
}

void Render::render_rect(const SDL_Rect* const& rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(get_renderer(), color.r, color.g, color.b, color.a);

	if (SDL_RenderFillRect(get_renderer(), rect) != 0)
	{
		printf("Couldn't render Rect. Error log: %s", SDL_GetError());
	}

}

void Render::render_score(SDL_Texture* texture_text)
{
	int x_pos = Window::win_surface()->w / 25;
	int y_pos = Window::win_surface()->h / 25;
	int width = 0;
	int height = 0;
	SDL_QueryTexture(texture_text, nullptr, nullptr, &width, &height);
	const SDL_Rect temp_rect = {x_pos, y_pos, width, height};
	SDL_RenderCopy(get_renderer(), texture_text, nullptr, &temp_rect);

}

void Render::render_enemy(Enemy*& enemy)
{
	constexpr int width_padding = 1; //1 pixel width
	constexpr int height_padding = 1; //1 pixel height
	constexpr int sprite_max_width = 256;
	constexpr int sprite_max_height = 256;
	constexpr int sprite_max_y_pos = 256;
	const SDL_Rect* sprite_src = nullptr;
	

	if (!Enemy::alternate_texture && enemy->health == 1)
	{
		switch (enemy->get_enemy_type())
		{
		case Enemy::CUTTLE:
			sprite_src = new SDL_Rect{ width_padding, 0 * sprite_max_y_pos + 1 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::SQUID:
			sprite_src = new SDL_Rect{ width_padding, 1 * sprite_max_y_pos + 2 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::CRAB:
			sprite_src = new SDL_Rect{ width_padding, 2 * sprite_max_y_pos + 3 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::OCTOPUS:
			sprite_src = new SDL_Rect{ width_padding, 3 * sprite_max_y_pos + 4 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::UFO:
			sprite_src = new SDL_Rect{ width_padding, 4 * sprite_max_y_pos + 5 * height_padding, sprite_max_width * 2, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		default:
			printf("Couldn't render enemy\n");
			break;
		}
	}
	else if (Enemy::alternate_texture && enemy->health == 1)
	{
		switch (enemy->get_enemy_type())
		{
		case Enemy::CUTTLE:
			sprite_src = new SDL_Rect{ sprite_max_width + 2 * width_padding, 0 * sprite_max_y_pos + 1 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::SQUID:
			sprite_src = new SDL_Rect{ sprite_max_width + 2 * width_padding, 1 * sprite_max_y_pos + 2 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::CRAB:
			sprite_src = new SDL_Rect{ sprite_max_width + 2 * width_padding, 2 * sprite_max_y_pos + 3 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::OCTOPUS:
			sprite_src = new SDL_Rect{ sprite_max_width + 2 * width_padding, 3 * sprite_max_y_pos + 4 * height_padding, sprite_max_width, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		case Enemy::UFO:
			sprite_src = new SDL_Rect{ width_padding, 4 * sprite_max_y_pos + 5 * height_padding, sprite_max_width * 2, sprite_max_height };
			SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, enemy->get_image_rect());
			if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
			break;

		default:
			printf("Couldn't render enemy\n");
			break;
		}
	}
	
	else if (enemy->health == 0)
	{
		sprite_src = new SDL_Rect{ width_padding, 5 * sprite_max_height + 6 * height_padding, 16, 16};
		const SDL_Rect* sprite_dest = new SDL_Rect{ enemy->get_image_rect()->x, enemy->get_image_rect()->y, enemy->get_image_rect()->h, enemy->get_image_rect()->h };
		SDL_RenderCopy(get_renderer(), Enemy::get_sprite_sheet()->get_texture(), sprite_src, sprite_dest);
		if (sprite_src) { delete sprite_src; sprite_src = nullptr; }
		if (sprite_dest) { delete sprite_dest; sprite_dest = nullptr; }
 	}
}

void Render::render_player_destroyed(Player*& player, int sprite_number)
{
	constexpr int max_sprite_width = 44;
	constexpr int max_sprite_height = 28;
	const SDL_Rect* src_rect = new SDL_Rect{1 * sprite_number + (sprite_number - 1) * max_sprite_width, 1, max_sprite_width, max_sprite_height };
	const SDL_Rect* dest_rect = new SDL_Rect{ player->get_x_pos(), player->get_y_pos(), player->get_width(), player->get_height() };

	SDL_RenderCopy(get_renderer(), Player::get_sprite_sheet()->get_texture(), src_rect, dest_rect);

	if (src_rect) { delete src_rect; src_rect = nullptr; }
	if (dest_rect) { delete dest_rect; dest_rect = nullptr; }
}


void Render::render_formation(const std::vector<Enemy*>& formation)
{
	for (Enemy* enemy_ptr : formation)
	{
		if (enemy_ptr)
		{
			render_enemy(enemy_ptr);
		}
	}
}


