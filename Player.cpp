#include "Player.h"
#include <cstdio>
#include "Blockade.h"
#include "Enemy.h"
#include "Collision.h"
#include "LTexture.h"
#include <SDL.h>

#include "Projectile.h"
#include "Window.h"

bool Player::is_active_projectile = false;
LTexture* Player::s_destroyed_player_sheet = nullptr;
int Player::s_lives;

Player::Player(const char* file_path) : Entity(file_path), m_player_projectile(nullptr), m_projectile(nullptr), m_projectile_type(Projectile::SINGLE)
{
	
}

void Player::set_sprite_sheet(LTexture* sprite_sheet)
{
	s_destroyed_player_sheet = sprite_sheet;
}

LTexture* Player::get_sprite_sheet()
{
	return s_destroyed_player_sheet;
}

void Player::shoot()
{
	if (!is_active_projectile)
	{
		is_active_projectile = true;
		const int projectile_x_pos = get_x_pos() + get_width_2() - get_width() / 28;
		const int projectile_width = get_width() / 14;
		const int projectile_height = get_height() / 6;
		m_projectile = new SDL_Rect{ projectile_x_pos, get_y_pos(), projectile_width, projectile_height };
		printf("m_projectile created\n");
	}


}

void Player::reset_projectile()
{
	m_player_projectile = nullptr;
}

SDL_Rect* Player::get_projectile() const
{
	return m_projectile;
}

Projectile* Player::get_player_projectile() const
{
	return m_player_projectile;
}


void Player::destroy_projectile()
{
	if (m_projectile != nullptr)
	{
		delete m_projectile;
		m_projectile = nullptr;
		printf("m_projectile destroyed\n");
		is_active_projectile = false;
	}
}

void Player::set_projectile_type(int projectile_type)
{
	m_projectile_type = projectile_type;
}

void Player::add_player_velocity(int velocity, int scalar)
{
	m_x_pos = m_x_pos + (velocity * scalar);
	
}

void Player::clamp_position(int window_width)
{

	int width = 0;
	SDL_QueryTexture(m_l_texture->get_texture(), nullptr, nullptr, &width, nullptr);
	const int texture_padding = width / 10;
	if (m_x_pos + width - texture_padding > window_width)
	{
		m_x_pos = window_width - width + texture_padding;
	}

	else if (m_x_pos + texture_padding < 0)
	{
		m_x_pos = -1 * texture_padding;
	}

}

bool Player::b_destroy_projectile()
{
	if (is_active_projectile)
	{
		if (get_projectile() != nullptr)
		{
			if (get_projectile()->y < 0)
			{
				is_active_projectile = false;
				printf("miss\n");
				destroy_projectile();
				return true;
			}
		}
	}

	return false;
}

void Player::handle_input(int window_width)
{
	int movement_speed = Window::win_surface()->w / Window::get_window_divisions() / 35;
	if (movement_speed < 1) { movement_speed = 1; }

	const auto key_states = SDL_GetKeyboardState(nullptr);
	//printf("%d\n", size);
	if (key_states[SDL_SCANCODE_LEFT])
	{
		add_player_velocity(-1 * movement_speed);
		clamp_position(window_width);
	}
	if (key_states[SDL_SCANCODE_RIGHT])
	{
		add_player_velocity(movement_speed);
		clamp_position(window_width);
	}
	if (key_states[SDL_SCANCODE_SPACE])
	{

		if (Projectile::get_player_projectiles_vector().empty())
		{
			Projectile::load_projectile(this, Projectile::get_player_projectiles_vector(), m_projectile_type);
			for (const auto projectile : Projectile::get_player_projectiles_vector())
			{
				projectile->shoot();
			}
			
		}
	}

	if (key_states[SDL_SCANCODE_LCTRL] && key_states[SDL_SCANCODE_W])
	{
		m_cheat = true;
	}

}

void Player::update(unsigned& game_timer)
{
	handle_input(SDL_GetWindowSurface(Window::get_window())->w);
	//if (m_player_projectile)
	//{
	//	if (m_player_projectile->get_projectile_rect())
	//		m_player_projectile->increment_projectile();
	//}
	//b_destroy_projectile();
	//shoot();
	if (!Projectile::get_player_projectiles_vector().empty())
	{
		for (const auto& projectile : Projectile::get_player_projectiles_vector())
		{
			if (projectile)
			{
				if (projectile->get_projectile_type() == Projectile::BURST) 
				{
					const int projectile_velocity = Window::win_surface()->h / 180;
					projectile->set_projectile_velocity(projectile_velocity);
					projectile->increment_projectile(game_timer, get_x_pos() + get_width_2(), get_y_pos());
				}

				else { projectile->increment_projectile(); }
			}
		}
	}
}

void Player::set_projectile_velocity(int projectile_velocity)
{
	m_projectile_velocity_multiplier = projectile_velocity;
}

int Player::get_lives() const
{
	return s_lives;
}

int Player::get_projectile_type() const
{
	return m_projectile_type;
}

unsigned Player::get_projectile_power_timer() const
{
	return m_power_up_timer;
}

SDL_Rect*& Player::get_power_up_bar()
{
	return m_power_up_rect;
}

void Player::set_projectile_power_timer(unsigned time)
{
	m_power_up_timer = time;
}

void Player::reset_power_up_timer()
{
	m_power_up_timer = 1500;
	delete m_power_up_rect;
	m_power_up_rect = new SDL_Rect{ Window::win_surface()->w * 23 / 25,  Window::win_surface()->h * 17 / 20, Window::win_surface()->w / 40, Window::win_surface()->h * 2 / 15};
}



void Player::decrement_lives()
{
	s_lives--;
}

bool Player::is_cheating() const
{
	return m_cheat;
}

void Player::reset_cheat()
{
	m_cheat = false;
}

void Player::reset_lives()
{
	s_lives = 3;
}
