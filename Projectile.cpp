#include "Projectile.h"
#include "SDL.h"
#include <cstdio>
#include "Blockade.h"
#include "Collision.h"
#include "Enemy.h"
#include "Render.h"
#include "Window.h"

std::vector<Projectile*> Projectile::s_player_projectiles_vector;
std::vector<Projectile*> Projectile::s_enemy_projectiles_vector;

Projectile::Projectile()
{
	m_is_active_projectile = false;
	m_projectile_rect = nullptr;
}

Projectile::Projectile(int projectile_type)
{
	m_is_active_projectile = false;
	m_projectile_rect = nullptr;

	switch (projectile_type)
	{
	case Projectile::SINGLE:
		m_projectile_type = Projectile::SINGLE;
		break;

	case Projectile::BURST:
		m_projectile_type = Projectile::BURST;
		break;

	case Projectile::SPREAD:
		m_projectile_type = Projectile::SPREAD;
		break;

	case Projectile::EXPLOSIVE:
		m_projectile_type = Projectile::EXPLOSIVE;
		break;

	default:
		break;

	}

}

Projectile::Projectile(SDL_Rect* projectile_rect, int projectile_type) :  m_projectile_type(projectile_type), m_is_active_projectile(false),  m_projectile_rect(projectile_rect)
{
	const int projectile_velocity = Window::win_surface()->h / 300;
	m_projectile_velocity = projectile_velocity;
}

Projectile::Projectile(SDL_Rect* projectile_rect, int projectile_type, int horizontal_delta) : m_projectile_type(projectile_type), m_is_active_projectile(false), m_projectile_velocity(3), m_horizontal_delta(horizontal_delta), m_projectile_rect(projectile_rect)
{
}


void Projectile::load_projectile(const Entity* projectile_owner, std::vector<Projectile*>& projectile_vector, int projectile_type)
{

	const int projectile_x_pos = projectile_owner->get_x_pos() + projectile_owner->get_width_2();
	const int projectile_y_pos = projectile_owner->get_y_pos();
	int projectile_width = projectile_owner->get_width() / 14;
	int projectile_height = projectile_owner->get_height() / 6;

	if (projectile_width == 0 || projectile_height == 0)
	{
		projectile_width = Window::win_surface()->w / 303;
		projectile_height = Window::win_surface()->h / 130;
	}

	if (projectile_height == 0 || projectile_width == 0)
	{
		projectile_height = 1;
		projectile_width = 1;
	}

	int horizontal_delta;
	const auto projectile_rect_normal = new SDL_Rect{ projectile_x_pos, projectile_y_pos, projectile_width, projectile_height };
	SDL_Rect* projectile_rect_burst;
	SDL_Rect* projectile_rect_burst_2;
	SDL_Rect* projectile_rect_spread;
	SDL_Rect* projectile_rect_spread_2;

	switch (projectile_type)
	{
	case Projectile::SINGLE:
		projectile_vector.emplace_back(new Projectile(projectile_rect_normal, projectile_type));

		break;

	case Projectile::BURST:
		projectile_rect_burst = new SDL_Rect{ -1024, projectile_y_pos + projectile_owner->get_height_2(), projectile_width, projectile_height };
		projectile_rect_burst_2 = new SDL_Rect{ -1024, projectile_y_pos + projectile_owner->get_height(), projectile_width, projectile_height };

		projectile_vector.emplace_back(new Projectile(projectile_rect_normal, projectile_type));
		projectile_vector.emplace_back(new Projectile(projectile_rect_burst, projectile_type));
		projectile_vector.emplace_back(new Projectile(projectile_rect_burst_2, projectile_type));

		break;

	case Projectile::SPREAD:
		horizontal_delta = projectile_width / 4;
		if (horizontal_delta == 0) { horizontal_delta = 1; }

		projectile_rect_spread = new SDL_Rect{ projectile_x_pos, projectile_y_pos, projectile_width, projectile_height };
		projectile_rect_spread_2 = new SDL_Rect{ projectile_x_pos, projectile_y_pos, projectile_width, projectile_height };

		projectile_vector.emplace_back(new Projectile(projectile_rect_normal, projectile_type, -1 * horizontal_delta));
		projectile_vector.emplace_back(new Projectile(projectile_rect_spread, projectile_type));
		projectile_vector.emplace_back(new Projectile(projectile_rect_spread_2, projectile_type,  horizontal_delta));

		break;

	case Projectile::EXPLOSIVE:
		projectile_vector.emplace_back(new Projectile(projectile_rect_normal, projectile_type));

		break;

	default:
		break;
	}
	printf("projectile_ loaded\n");
}

void Projectile::set_projectile_velocity(int projectile_velocity)
{
	m_projectile_velocity = projectile_velocity;
}

void Projectile::set_projectile_type(int projectile_type)
{
	m_projectile_type = projectile_type;
}


void Projectile::shoot()
{

	if (!m_is_active_projectile)
	{
		m_is_active_projectile = true;
		printf("projectile shot\n");
	}

}

void Projectile::shoot_downwards()
{
	m_is_shooting_downwards = true;
}

void Projectile::destroy_projectile()
{
	if (m_projectile_rect != nullptr)
	{
		delete m_projectile_rect;
		m_projectile_rect = nullptr;
		printf("m_projectile destroyed\n");
		m_is_active_projectile = false;
		
	}
}

//void Projectile::increment_projectile(int delta, Blockade* blockade_1, Blockade* blockade_2, Blockade* blockade_3, Blockade* blockade_4, Enemy* UFO)
//{
//	if (m_projectile_rect)
//	{
//		m_projectile_rect->y -= delta;
//		if (m_projectile_rect->y < 0 || m_projectile_rect->y > Window::win_surface()->h)
//		{
//			destroy_projectile();
//		}
//	}
//	if (m_is_active_projectile)
//	{
//		for (int row = 0; row < static_cast<int>(std::size(Enemy::s_formation)); row++)
//		{
//			for (int col = 0; col < static_cast<int>(std::size(Enemy::s_formation[0])); col++)
//			{
//				if (get_projectile_rect())
//				{
//					if (Enemy::s_formation[row][col])
//					{
//						if (Enemy::s_formation[row][col]->health == 1)
//						{
//							if (Collision::is_colliding(get_projectile_rect(), Enemy::s_formation[row][col]))
//							{
//
//								Enemy::s_formation[row][col]->health = 0;
//								Enemy::s_formation[row][col]->update_death_timer();
//
//								destroy_projectile();
//
//
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}

void Projectile::increment_projectile(unsigned game_timer, int origin_x_pos, int origin_y_pos)
{

	if (game_timer - projectile_timer > 450 && m_projectile_rect->y < origin_y_pos)
	{
		if (m_projectile_rect->x < 0)
		{
			m_projectile_rect->x = origin_x_pos;
			m_projectile_rect->y = origin_y_pos;
			projectile_timer = game_timer;
		}
	}

	if (m_is_shooting_downwards)
	{
		m_projectile_rect->y += m_projectile_velocity;
		m_projectile_rect->x += m_horizontal_delta;
	}

	else if (!m_is_shooting_downwards)
	{
		m_projectile_rect->y -= m_projectile_velocity;
		m_projectile_rect->x += m_horizontal_delta;
	}



}

void Projectile::render_projectile() const
{
	const SDL_Rect* const& temp_rect = m_projectile_rect;
	if (m_projectile_rect) { Render::render_rect(temp_rect, SDL_Color{ 255,255,255,255 }); }
}

int Projectile::get_projectile_type() const
{
	return m_projectile_type;
}

SDL_Rect* Projectile::get_projectile_rect() const
{
	return m_projectile_rect;
}

int Projectile::get_projectile_velocity() const
{
	return m_projectile_velocity;
}

int Projectile::get_health() const
{
	return m_health;
}

void Projectile::set_health(int health)
{
	m_health = health;
}

std::vector<Projectile*>& Projectile::get_player_projectiles_vector()
{
	return s_player_projectiles_vector;
}

std::vector<Projectile*>& Projectile::get_enemy_projectiles_vector()
{
	return s_enemy_projectiles_vector;
}
