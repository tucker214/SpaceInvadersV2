#include "Enemy.h"
#include <cstdio>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include "Collision.h"
#include "LTexture.h"
#include "Window.h"
#include "Player.h"
#include "Projectile.h"

bool Enemy::alternate_texture;
bool Enemy::s_is_colliding_player = false;
LTexture* Enemy::s_enemy_sprite_sheet = nullptr;
Enemy* Enemy::enemy_arr[5 * 11];

Enemy::Enemy()
{
	is_moving_right = true;
}

Enemy::Enemy(int enemy)
{

	switch (enemy)
	{
	case CUTTLE:
		m_score = 40;
		m_type = CUTTLE;
		break;

	case SQUID:
		m_score = 30;
		m_type = SQUID;
		break;

	case CRAB:
		m_score = 20;
		m_type = CRAB;
		break;

	case OCTOPUS:
		m_score = 10;
		m_type = OCTOPUS;
		break;

	case UFO:
		m_score = 200;
		m_type = UFO;
		m_y_pos = Window::win_surface()->h * 1 / 10 + Window::win_surface()->h * 1 / 50;
		break;

	default:
		break;
	}

	m_shooting_cooldown = 0;

}

Enemy::Enemy(const char* file_path) : Entity(file_path)
{
	is_moving_right = true;
}

Enemy::~Enemy() = default;


void Enemy::format_formation(Enemy* arr[55])
{
	alternate_texture = false;

	constexpr float spacing = 1.3f;
	constexpr int max_width = 11;
	constexpr int max_height = 5;
	const auto centering = (static_cast<float>(Window::get_window_divisions()) - static_cast<float>(max_width) * spacing) / 2;


	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 11; col++)
		{
			if (&arr[max_width * row + col])
			{
				if (row == 0 || row == 1)
				{
					arr[max_width * row + col] = new Enemy(CUTTLE);
				}

				else if (row == 2 || row == 3)
				{
					arr[max_width * row + col] = new Enemy(CRAB);
				}

				else
				{
					arr[max_width * row + col] = new Enemy(SQUID);
				}

				arr[max_width * row + col]->set_x_pos(static_cast<int>(static_cast<float>(Window::win_surface()->w) / static_cast<float>(Window::get_window_divisions()) * (static_cast<float>(col) * spacing + centering)));
				arr[max_width * row + col]->set_y_pos(static_cast<int>(static_cast<float>(Window::win_surface()->h)) / 12 * row + Window::win_surface()->h / 5);
				arr[max_width * row + col]->set_x_increment(Window::win_surface()->w / 160);
				arr[max_width * row + col]->set_y_increment(Window::win_surface()->h / 15);
				arr[max_width * row + col]->set_image_rect(new SDL_Rect{ arr[max_width * row + col]->get_x_pos(), arr[max_width * row + col]->get_y_pos(), Window::win_surface()->h / (Window::get_window_divisions()), Window::win_surface()->h / Window::get_window_divisions() });
			}
		}
	}
}

void Enemy::shift_formation(Enemy* arr[][11], int rows, int cols, Player*& player)
{
	
	if (is_moving_right)
	{
		bool crosses_window_border = false;
		for (int col = cols - 1; col >= 0; col--)
		{
			for (int row = rows - 1; row >= 0; row--)
			{
				if (arr[row][col])
				{
					if (arr[row][col]->get_x_pos() + arr[row][col]->get_width() + arr[row][col]->m_increment_x_amount > Window::win_surface()->w)
					{
						crosses_window_border = true;
					}
				}
			}
		}

		if (crosses_window_border)
		{
			for (int col = cols - 1; col >= 0; col--)
			{
				for (int row = rows - 1; row >= 0; row--)
				{
					if (arr[row][col])
					{
						if (arr[row][col]->health > 0)
							arr[row][col]->increment_y_position();

						is_moving_right = false;
					}
				}
			}
		}

		else
		{
			for (int col = cols - 1; col >= 0; col--)
			{
				for (int row = rows - 1; row >= 0; row--)
				{
					if (arr[row][col])
					{
						if(arr[row][col]->health > 0)
							arr[row][col]->increment_x_position();

						if (Collision::is_colliding_y_axis(arr[row][col], player))
						{
							s_is_colliding_player = true;
						}
					}
				}
			}
		}
	}

	else
	{
		bool crosses_window_border = false;
		for (int col = cols - 1; col >= 0; col--)
		{
			for (int row = rows - 1; row >= 0; row--)
			{
				if (arr[row][col])
				{
					if (arr[row][col]->get_x_pos() - arr[row][col]->m_increment_x_amount < 0)
					{
						crosses_window_border = true;
					}
				}
			}
		}

		if (crosses_window_border)
		{
			for (int col = cols - 1; col >= 0; col--)
			{
				for (int row = rows - 1; row >= 0; row--)
				{
					if (arr[row][col])
					{
						if (arr[row][col]->health > 0)
							arr[row][col]->increment_y_position();

						is_moving_right = true;
					}
				}
			}
		}

		else
		{
			for (int col = cols - 1; col >= 0; col--)
			{
				for (int row = rows - 1; row >= 0; row--)
				{
					if (arr[row][col])
					{
						if (arr[row][col]->health > 0)
							arr[row][col]->decrement_x_position();

						if (Collision::is_colliding_y_axis(arr[row][col], player))
						{
							s_is_colliding_player = true;
						}
					}
				}
			}
		}
	}
}

void Enemy::shift_formation(Enemy* const (&arr)[55])
{
	constexpr int max_width = 11;
	constexpr int max_height = 5;

	if (is_moving_right)
	{
		bool crosses_window_border = false;
		for (int col = max_width - 1; col >= 0; col--)
		{
			for (int row = max_height - 1; row >= 0; row--)
			{
				if (arr[max_width * row + col])
				{
					if (arr[max_width * row + col]->get_x_pos() + arr[max_width * row + col]->get_image_rect()->w + arr[max_width * row + col]->m_increment_x_amount > Window::win_surface()->w)
					{
						crosses_window_border = true;
					}
				}
			}
		}

		if (crosses_window_border)
		{
			for (int col = max_width - 1; col >= 0; col--)
			{
				for (int row = max_height - 1; row >= 0; row--)
				{
					if (arr[max_width * row + col])
					{
						if (arr[max_width * row + col]->health > 0)
							arr[max_width * row + col]->increment_y_position();

						is_moving_right = false;
					}
				}
			}
		}

		else
		{
			for (int col = max_width - 1; col >= 0; col--)
			{
				for (int row = max_height - 1; row >= 0; row--)
				{
					if (arr[max_width * row + col])
					{
						if (arr[max_width * row + col]->health > 0)
							arr[max_width * row + col]->increment_x_position();
					}
				}
			}
		}
	}

	else
	{
		bool crosses_window_border = false;
		for (int col = max_width - 1; col >= 0; col--)
		{
			for (int row = max_height - 1; row >= 0; row--)
			{
				if (arr[max_width * row + col])
				{
					if (arr[max_width * row + col]->get_image_rect()->x - arr[max_width * row + col]->m_increment_x_amount < 0)
					{
						crosses_window_border = true;
					}
				}
			}
		}

		if (crosses_window_border)
		{
			for (int col = max_width - 1; col >= 0; col--)
			{
				for (int row = max_height - 1; row >= 0; row--)
				{
					if (arr[max_width * row + col])
					{
						if (arr[max_width * row + col]->health > 0)
							arr[max_width * row + col]->increment_y_position();

						is_moving_right = true;
					}
				}
			}
		}

		else
		{
			for (int col = max_width - 1; col >= 0; col--)
			{
				for (int row = max_height - 1; row >= 0; row--)
				{
					if (arr[max_width * row + col])
					{
						if (arr[max_width * row + col]->health > 0)
							arr[max_width * row + col]->decrement_x_position();
					}
				}
			}
		}
	}
}

void Enemy::update_UFO(Enemy*& param_UFO, Uint32& timer_begin, Uint32& game_timer, Uint32 UFO_cooldown)
{

	const Uint32 UFO_spawn_timer = game_timer - timer_begin;

	if (!param_UFO && UFO_cooldown < UFO_spawn_timer)
	{
		printf("created UFO\n");
		param_UFO = new Enemy(UFO);
		//param_UFO->get_l_texture()->resize(Window::win_surface()->w / Window::get_window_divisions(), Window::win_surface()->h / Window::get_window_divisions());
		//param_UFO->get_destroyed_texture()->resize(Window::win_surface()->w / Window::get_window_divisions(), Window::win_surface()->h / Window::get_window_divisions());
		param_UFO->set_image_rect(new SDL_Rect{ -100, Window::win_surface()->h * 1 / 10 + Window::win_surface()->h * 1 / 50, Window::win_surface()->w / Window::get_window_divisions(), Window::win_surface()->h / Window::get_window_divisions() });
		timer_begin = SDL_GetTicks();
		param_UFO->set_ufo_direction();
	}

	else if (param_UFO)
	{
		if (param_UFO->health == 1)
		{
			param_UFO->increment_x_position();
		}
	}

}

LTexture* Enemy::get_sprite_sheet()
{
	return s_enemy_sprite_sheet;
}

void Enemy::set_sprite_sheet(LTexture* l_texture)
{
	s_enemy_sprite_sheet = l_texture;
}

void Enemy::shift_enemy_vector_formation(const std::vector<Enemy*>& formation)
{


	if (is_moving_right)
	{
		bool crosses_window_border = false;
		for (const auto& enemy_ptr : formation)
		{
			if (enemy_ptr)
			{
				if (enemy_ptr->get_image_rect()->x + enemy_ptr->get_image_rect()->w + enemy_ptr->m_increment_x_amount > Window::win_surface()->w)
				{
					crosses_window_border = true;
				}
			}
		}

		if (crosses_window_border)
		{
			for (const auto& enemy_ptr : formation)
			{				

				if (enemy_ptr)
				{
					if (enemy_ptr->health > 0)
						enemy_ptr->increment_y_position();

					is_moving_right = false;
				}
				
			}
		}

		else
		{
			for (const auto& enemy_ptr : formation)
			{

					if (enemy_ptr)
					{
						if (enemy_ptr->health > 0)
							enemy_ptr->increment_x_position();
					}
			}
		}
	}

	else
	{
		bool crosses_window_border = false;
		for (const auto& enemy_ptr : formation)
		{

			if (enemy_ptr)
			{
				if (enemy_ptr->get_image_rect()->x - enemy_ptr->m_increment_x_amount < 0)
				{
					crosses_window_border = true;
				}
			}
		}

		if (crosses_window_border)
		{
			for (const auto& enemy_ptr : formation)
			{
				if (enemy_ptr)
				{
					if (enemy_ptr->health > 0)
						enemy_ptr->increment_y_position();

					is_moving_right = true;
				}
				
			}
		}

		else
		{
			for (const auto& enemy_ptr : formation)
			{

				if (enemy_ptr)
				{
					if (enemy_ptr->health > 0)
						enemy_ptr->decrement_x_position();
				}
			}
		}
	}
	
}

void Enemy::set_ufo_direction()
{

	if (SDL_GetTicks() % 2)
	{
		if (m_image_rect) { m_image_rect->x = 0 - m_image_rect->w; }
		m_x_pos = 0 - get_width();
		Window::win_surface()->w / 950 != 0 ? set_x_increment(Window::win_surface()->w / 950) : set_x_increment(1);
	}

	else
	{
		if (m_image_rect) { m_image_rect->x = Window::win_surface()->w; }
		m_x_pos = Window::win_surface()->w;
		Window::win_surface()->w / 950 != 0 ? set_x_increment(-1 * Window::win_surface()->w / 950) : set_x_increment(-1);
	}

}

void Enemy::increment_x_position()
{
	m_x_pos+= m_increment_x_amount;

	if (get_image_rect())
		get_image_rect()->x += m_increment_x_amount;
}

void Enemy::decrement_x_position()
{
	m_x_pos -= m_increment_x_amount;

	if (get_image_rect())
	get_image_rect()->x -= m_increment_x_amount;
}

void Enemy::increment_y_position()
{
	m_y_pos += m_increment_y_amount;

	if (get_image_rect())
		get_image_rect()->y += m_increment_y_amount;
}

int Enemy::get_enemy_score() const
{
	return m_score;
}

int Enemy::get_enemy_type() const
{
	return m_type;
}

void Enemy::set_x_increment(int x_delta)
{

	m_increment_x_amount = x_delta;
}

void Enemy::set_y_increment(int y_delta)
{

	m_increment_y_amount = y_delta;
}


void Enemy::random_enemy_shoot()
{
	constexpr int col_boundary = 11;
	constexpr int row_boundary = 5;
	Enemy* enemy = enemy_arr[SDL_GetTicks() % (row_boundary * col_boundary)];

	while (!enemy)
	{
		enemy = enemy_arr[SDL_GetTicks() % (row_boundary * col_boundary)];
	}

	if (enemy->m_shooting_cooldown <= 0)
	{
		Projectile::load_projectile(enemy, Projectile::get_enemy_projectiles_vector(), Projectile::SINGLE);
		Projectile::get_enemy_projectiles_vector().back()->shoot();
		enemy->m_shooting_cooldown = 2000;
	}


}

void Enemy::decrement_shoot_timer()
{
	if (m_shooting_cooldown > 0) { --m_shooting_cooldown; }
}

unsigned int Enemy::get_death_timer() const
{
	return m_death_timer;
}

void Enemy::update_death_timer()
{
	m_death_timer = SDL_GetTicks();
}

void Enemy::update_enemy_array(Uint32& time_begin, const int& casualties)
{

	Uint32 enemy_movement_interval = 600;
	int fire_rate = 4;

	if (casualties >= 20)
	{
		enemy_movement_interval = 300;
		fire_rate = 2;
	}
	if (casualties >= 40)
	{
		enemy_movement_interval = 100;
		fire_rate = 1;
	}

	const Uint32 time_now = SDL_GetTicks();
	if (SDL_GetTicks() % enemy_movement_interval < static_cast<Uint32>(fire_rate)) { random_enemy_shoot(); }

	if (time_now - time_begin >= enemy_movement_interval)
	{
		shift_formation(enemy_arr);
		alternate_texture = !alternate_texture;

		
		time_begin = SDL_GetTicks();

	}

	for (const auto& projectile : Projectile::get_enemy_projectiles_vector())
	{
		projectile->shoot_downwards();
		projectile->increment_projectile();
	}
}

SDL_Rect* Enemy::get_image_rect() const
{
	return m_image_rect;
}

void Enemy::set_image_rect(SDL_Rect* image_rect)
{
	m_image_rect = image_rect;
}





