#include "Game.h"

#include <algorithm>
#include <string>
#include <SDL_mixer.h>
#include "Window.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Render.h"
#include "LTexture.h"
#include "Blockade.h"
#include <SDL_ttf.h>
#include "Collision.h"
#include "Projectile.h"


Player* Game::s_player;
bool Game::s_is_game_finished;
bool Game::s_is_game_won;
int Game::s_player_x_pos = -1024;
int Game::s_score = 0;
int Game::s_level = 1;
Uint32 enemy_timer;
Uint32 UFO_creation_timer;
Uint32 UFO_death_timer;
constexpr int MAX_ENEMY_COUNT = 55;

Game::Game()
{
	Enemy::set_sprite_sheet(LTexture::create_l_texture("assets/textures/sprite_sheet2.png"));
	Player::set_sprite_sheet(LTexture::create_l_texture("assets/textures/ship_destroyed_sheet.png"));
	m_surface_text = nullptr; //SDL_Surface for displaying score

	load_font("assets/fonts/Retro Gaming.ttf");
	s_is_game_finished = false;
	s_is_game_won = false;

	if (!s_player){ s_player = new Player("assets/textures/player.png");}

	if (s_player_x_pos > 0)
		s_player->set_x_pos(s_player_x_pos);
	else
		s_player->set_x_pos((Window::win_surface()->w / 2) - s_player->get_width_2());

	s_player->set_y_pos(Window::win_surface()->h * 8 / 10 + (Window::win_surface()->h / 10));
	s_player->set_projectile_velocity(Window::win_surface()->h / 240);
	s_player->get_l_texture()->resize(static_cast<int>(static_cast<float>(Window::win_surface()->w * 0.8 / Window::get_window_divisions())), static_cast<int>(static_cast<float>(1.75 * Window::win_surface()->h) / static_cast<float>(Window::get_window_divisions() * 1.75)));
	m_player_died = false;
	m_should_revive = false;
	if (s_level == 1) { s_player->reset_lives(); }

	m_enemies = new Enemy();
	enemy_timer = SDL_GetTicks();
	Enemy::format_formation(Enemy::enemy_arr);
	m_enemies->is_moving_right = true;

	m_UFO = nullptr;

	m_UFO_power_up = nullptr;
	printf("created UFO\n");

	m_blockade_1 = new Blockade("assets/textures/blockade.png");
	m_blockade_2 = new Blockade("assets/textures/blockade.png");
	m_blockade_3 = new Blockade("assets/textures/blockade.png");
	m_blockade_4 = new Blockade("assets/textures/blockade.png");

	printf("Resizing blockade 1: %d\n", m_blockade_1->resize_blockade(Window::win_surface()->h / Window::get_window_divisions() * 3));
	printf("Resizing blockade 2: %d\n", m_blockade_2->resize_blockade(Window::win_surface()->h / Window::get_window_divisions() * 3));
	printf("Resizing blockade 3: %d\n", m_blockade_3->resize_blockade(Window::win_surface()->h / Window::get_window_divisions() * 3));
	printf("Resizing blockade 4: %d\n", m_blockade_4->resize_blockade(Window::win_surface()->h / Window::get_window_divisions() * 3));

	const int block_x_pos = m_blockade_1->get_min_x_pos() + (m_blockade_1->get_max_x_pos() - m_blockade_1->get_min_x_pos() / 2);
	m_blockade_1->set_position(static_cast<int>(Window::win_surface()->w / 10 * 2) - m_blockade_1->get_surface_width() / 2, Window::win_surface()->h / 12 * 9);
	//m_blockade_1->set_position(Window::win_surface()->w / 4 - block_x_pos, Window::win_surface()->h / 12 * 9);
	m_blockade_2->set_position(static_cast<int>(Window::win_surface()->w / 10 * 4) - m_blockade_2->get_surface_width() / 2, Window::win_surface()->h / 12 * 9);
	m_blockade_3->set_position(static_cast<int>(Window::win_surface()->w / 10 * 6) - m_blockade_3->get_surface_width() / 2, Window::win_surface()->h / 12 * 9);
	m_blockade_4->set_position(static_cast<int>(Window::win_surface()->w / 10 * 8) - m_blockade_4->get_surface_width() / 2, Window::win_surface()->h / 12 * 9);

	m_blockade_1->search_surface_pixels();
	m_blockade_2->search_surface_pixels();
	m_blockade_3->search_surface_pixels();
	m_blockade_4->search_surface_pixels();



	UFO_creation_timer = game_timer = SDL_GetTicks();
}

Game::~Game()
{
}


void Game::update()
{

	if (s_player->is_cheating())
	{
		s_is_game_finished = true;
		s_is_game_won = true;
		s_score += 1000;
		s_player->reset_cheat();
	}

	game_timer = SDL_GetTicks();

	if (Enemy::s_is_colliding_player) { set_game_finished(); }

	if(!s_is_game_finished)
	{
		if (!m_player_died)
		{
			if (s_player->get_power_up_bar() && s_player->get_projectile_type() != Projectile::SINGLE)
			{
			
				if (s_player->get_power_up_bar()->h > 0) 
				{
					const int timer_rect_height = s_player->get_power_up_bar()->h;
					const int power_up_bar_height = static_cast<signed int>(Window::win_surface()->h * 2 / 15 * s_player->get_projectile_power_timer() / 1500);
					s_player->get_power_up_bar()->h = power_up_bar_height;
					s_player->get_power_up_bar()->y += timer_rect_height - s_player->get_power_up_bar()->h;
				}
			}


			Enemy::update_UFO(m_UFO, UFO_creation_timer, game_timer, UFO_cooldown);

			if (m_UFO)
			{
				if (m_UFO->health == 1) { UFO_death_timer = SDL_GetTicks(); }

				else if (m_UFO->health == 0)
				{
					if (game_timer - UFO_death_timer > 300) { m_UFO->health = -1; }
				}
			}

			m_enemies->update_enemy_array(enemy_timer, get_casualties());
			s_player->update(game_timer);
			const int power_drop_speed = Window::win_surface()->h / 450;
			if (m_UFO_power_up) { power_drop_speed != 0 ? m_UFO_power_up->increment_y_pos(power_drop_speed) : m_UFO_power_up->increment_y_pos(1); }

			if (m_enemy_casualties >= MAX_ENEMY_COUNT)
			{
				s_is_game_finished = true;
				s_is_game_won = true;
				s_player_x_pos = s_player->get_x_pos();
			}

			for (Enemy*& enemy : Enemy::enemy_arr)
			{
				if (enemy)
					enemy->decrement_shoot_timer();
			}

		}

		if (m_should_revive)
		{
			if (s_player->get_lives() > 0) { m_player_died = false; }
			else if (s_player->get_lives() == 0) { s_is_game_finished = true; s_is_game_won = false; s_score = 0; }
			m_should_revive = false;
		
		}
	}

	if (m_texture_text) { SDL_DestroyTexture(m_texture_text); m_texture_text = nullptr; }
	
}

void Game::render()
{
	
	if (!s_is_game_finished)
	{
		SDL_SetRenderDrawColor(Render::get_renderer(), 0, 0, 0, 0);
		SDL_RenderClear(Render::get_renderer());

		if (m_UFO)
		{
			Render::render_enemy(m_UFO);
		}
		for (Enemy*& enemy_ptr : Enemy::enemy_arr) { if (enemy_ptr) { Render::render_enemy(enemy_ptr); } }

		if (m_UFO_power_up) { Render::render_sprite(m_UFO_power_up->get_l_texture(), m_UFO_power_up->get_x_pos(), m_UFO_power_up->get_y_pos()); }
		render_score();

		if(m_blockade_1){ m_blockade_1->render(); }
		if(m_blockade_2){ m_blockade_2->render(); }
		if(m_blockade_3){ m_blockade_3->render(); }
		if(m_blockade_4){ m_blockade_4->render(); }

		for (Projectile*& projectile : Projectile::get_player_projectiles_vector())
		{
			if (projectile) { projectile->render_projectile(); }
		}

		for (Projectile*& projectile : Projectile::get_enemy_projectiles_vector())
		{
			if (projectile) { projectile->render_projectile(); }
		}


		for (int index = s_player->get_lives(); index > 0; index--) { Render::render_sprite(s_player->get_l_texture(), static_cast<int>(Window::win_surface()->w * (18 + 1.4 * index) / 25), Window::win_surface()->h / 25); }

		if (s_player)
			if (s_player->get_projectile_power_timer() > 0) { Render::render_rect(s_player->get_power_up_bar(), SDL_Color{ 0, 150, 255, 30}); }



		if (!m_player_died) { Render::render_sprite(s_player->get_l_texture(), s_player->get_x_pos(), s_player->get_y_pos()); }
		else { render_player_death(); }


		SDL_RenderPresent(Render::get_renderer()); //RenderPresent must always be the last render operation

	}

	else
	{
		SDL_SetRenderDrawColor(Render::get_renderer(), 0, 255, 0, 255);
		SDL_RenderClear(Render::get_renderer());
		SDL_RenderPresent(Render::get_renderer());
	}
}

//gets rid of garbage and handles collision
void Game::collision()
{
	if (!s_is_game_finished)
	{
		int player_projectile_index = 0;
		int enemy_index = 0;
		const int explosive_radius = Window::win_surface()->w / 9;
		SDL_Rect temp_explosive;

		for (auto& enemy_ptr : Enemy::enemy_arr)
		{
			if (enemy_ptr)
			{
				if (enemy_ptr->health == 0 && game_timer - enemy_ptr->get_death_timer() > 500)
				{
					s_score += enemy_ptr->get_enemy_score();
					delete Enemy::enemy_arr[enemy_index];
					Enemy::enemy_arr[enemy_index] = nullptr;
					m_enemy_casualties++;
					printf("%d\n", m_enemy_casualties);
				}

				else if (enemy_ptr->health == 1)
				{
					if (Collision::is_colliding_y_axis(enemy_ptr->get_image_rect(), s_player))
					{
						s_is_game_finished = true;
						s_is_game_won = false;
						s_score = 0;
					}
				}

			}
			enemy_index++;
		}

		enemy_index = 0;

		if (!Projectile::get_player_projectiles_vector().empty())
		{
			for (auto projectile : Projectile::get_player_projectiles_vector())
			{
				if (projectile)
				{
					for (Enemy*& enemy_ptr : Enemy::enemy_arr)
					{

						if (enemy_ptr)
						{
							if (enemy_ptr->health > 0)
							{
								if (Collision::is_colliding(projectile->get_projectile_rect(), enemy_ptr->get_image_rect()))
								{
									enemy_ptr->update_death_timer();
									enemy_ptr->health = 0;
									if (projectile->get_projectile_type() == Projectile::EXPLOSIVE)
									{
										temp_explosive = {projectile->get_projectile_rect()->x - explosive_radius / 2, projectile->get_projectile_rect()->y - explosive_radius / 2,
											projectile->get_projectile_rect()->w + explosive_radius, projectile->get_projectile_rect()->h + explosive_radius};
									}
							
									projectile->set_health(0);
									break;
								}
							}
						}

						enemy_index++;
					}
				}

				if (player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1))
					player_projectile_index++;
			}
		}

		player_projectile_index = 0;
		enemy_index = 0;

		if (!Projectile::get_player_projectiles_vector().empty())
		{
			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				if (projectile)
				{
					if (projectile->get_projectile_rect()->y < 0 || projectile->get_projectile_rect()->y > Window::win_surface()->h)
					{
						projectile->set_health(0);
					}

					if (projectile && player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1)) { player_projectile_index++; }
				}
			
			}
		}

		player_projectile_index = 0;

		if (!Projectile::get_player_projectiles_vector().empty())
		{
			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				if (projectile)
				{
					if (m_UFO)
					{
						if (Collision::is_colliding_enemy_rect(projectile->get_projectile_rect(), m_UFO))
						{
							m_UFO->health = 0;
							if (!m_UFO_power_up)
							{
								m_UFO_power_up = new Entity("assets/textures/power_up_icon2.png");
								m_UFO_power_up->get_l_texture()->resize(m_UFO->get_image_rect()->w / 2, m_UFO->get_image_rect()->h / 2);
								m_UFO_power_up->set_y_pos(m_UFO->get_y_pos());
								m_UFO_power_up->set_x_pos(m_UFO->get_x_pos() + m_UFO->get_width_2() - m_UFO_power_up->get_width_2());
							}

							else if (m_UFO_power_up->get_y_pos() > Window::win_surface()->h)
							{
								delete m_UFO_power_up;
								m_UFO_power_up = nullptr;
							}

							projectile->set_health(0);
						}
					}
				}
				if (projectile && player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size())) { player_projectile_index++; }
			}
		}


		if (m_UFO)
		{
			if (m_UFO->get_x_pos() > Window::win_surface()->w + m_UFO->get_width() || m_UFO->get_x_pos() < 0 - m_UFO->get_width() - 1)
			{
				delete m_UFO;
				m_UFO = nullptr;
				printf("UFO deleted\n");
			}

			else if (m_UFO->health == -1)
			{
				s_score += m_UFO->get_enemy_score();
				delete m_UFO;
				m_UFO = nullptr;
				printf("shot down UFO\n");
			}
		}

		if (s_player)
		{
			if (s_player->get_projectile_type() != Projectile::SINGLE && s_player->get_projectile_power_timer() > 0)
			{
				s_player->set_projectile_power_timer(s_player->get_projectile_power_timer() - 1);
			}

			if (s_player->get_projectile_power_timer() <= 0)
			{
				s_player->set_projectile_type(Projectile::SINGLE);
			}

		}

		if (s_player && m_UFO_power_up)
		{
			if (Collision::is_colliding(s_player, m_UFO_power_up))
			{
				const int random_number = static_cast<int>((game_timer % 3)) + 2;
				switch (random_number)
				{
				case Projectile::BURST:
					s_player->set_projectile_type(Projectile::BURST);
					printf("Equipped 3-round burst fire projectiles\n");
					s_player->reset_power_up_timer();
					break;

				case Projectile::SPREAD:
					s_player->set_projectile_type(Projectile::SPREAD);
					printf("Equipped spread fire projectiles\n");
					s_player->reset_power_up_timer();
					break;

				case Projectile::EXPLOSIVE:
					s_player->set_projectile_type(Projectile::EXPLOSIVE);
					printf("Equipped explosive projectile\n");
					s_player->reset_power_up_timer();
					break;

				default:
					s_player->set_projectile_type(Projectile::SINGLE);
					s_player->set_projectile_power_timer(0);
					break;
				}

				delete m_UFO_power_up;
				m_UFO_power_up = nullptr;
			}
		}

		player_projectile_index = 0;
		int blockade_rect_index = 0;

		if (m_blockade_1)
		{

			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_1->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_1->get_blockade_vector().begin(), m_blockade_1->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;

								if (blockade_rect_index < static_cast<int>(m_blockade_1->get_blockade_vector().size()))
								{
									m_blockade_1->get_blockade_vector().erase(m_blockade_1->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1))
						{
							if (Projectile::get_player_projectiles_vector()[player_projectile_index])
								player_projectile_index++;
						}
					
					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			if (m_blockade_1->get_blockade_vector().empty())
			{
				delete m_blockade_1;
				m_blockade_1 = nullptr;
				printf("m_blockade_4 deleted\n");
			}
		}

		player_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_2)
		{

			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_2->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_2->get_blockade_vector().begin(), m_blockade_2->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;
								if (blockade_rect_index < static_cast<int>(m_blockade_2->get_blockade_vector().size()))
								{
									m_blockade_2->get_blockade_vector().erase(m_blockade_2->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}
								projectile->set_health(0);
							}
						}

						if (player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1))
						{
							if (Projectile::get_player_projectiles_vector()[player_projectile_index])
								player_projectile_index++;
						}
					
					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			if (m_blockade_2->get_blockade_vector().empty())
			{
				delete m_blockade_2;
				m_blockade_2 = nullptr;
				printf("m_blockade_4 deleted\n");
			}
		}

		player_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_3)
		{

			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_3->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_3->get_blockade_vector().begin(), m_blockade_3->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;
								if (blockade_rect_index < static_cast<int>(m_blockade_3->get_blockade_vector().size()))
								{
									m_blockade_3->get_blockade_vector().erase(m_blockade_3->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}


								projectile->set_health(0);
							}
						}

						if (player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1))
						{
							if (Projectile::get_player_projectiles_vector()[player_projectile_index])
								player_projectile_index++;
						}
					
					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			if (m_blockade_3->get_blockade_vector().empty())
			{
				delete m_blockade_3;
				m_blockade_3 = nullptr;
				printf("m_blockade_4 deleted\n");
			}
		}

		player_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_4)
		{

			for (const auto& projectile : Projectile::get_player_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_4->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_4->get_blockade_vector().begin(), m_blockade_4->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;
								if (blockade_rect_index < static_cast<int>(m_blockade_4->get_blockade_vector().size()))
								{
									m_blockade_4->get_blockade_vector().erase(m_blockade_4->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (player_projectile_index < static_cast<int>(Projectile::get_player_projectiles_vector().size() - 1))
						{
							if (Projectile::get_player_projectiles_vector()[player_projectile_index])
								player_projectile_index++;
						}
						
					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			if (m_blockade_4->get_blockade_vector().empty())
			{
				delete m_blockade_4;
				m_blockade_4 = nullptr;
				printf("m_blockade_4 deleted\n");
			}
		}

		//-------------------------------------------------------------------------------------
		blockade_rect_index = 0;
		int enemy_projectile_index = 0;

		if (m_blockade_1)
		{

			for (const auto& projectile : Projectile::get_enemy_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_1->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_1->get_blockade_vector().begin(), m_blockade_1->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;

								if (blockade_rect_index < static_cast<int>(m_blockade_1->get_blockade_vector().size()))
								{
									m_blockade_1->get_blockade_vector().erase(m_blockade_1->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (enemy_projectile_index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size() - 1))
						{
							if (Projectile::get_enemy_projectiles_vector()[enemy_projectile_index])
								enemy_projectile_index++;
						}

					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			m_blockade_1->clean_blockade_vector();

			if (m_blockade_1->get_blockade_vector().empty())
			{
				delete m_blockade_1;
				m_blockade_1 = nullptr;
				printf("m_blockade_1 deleted\n");
			}
		}

		enemy_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_2)
		{

			for (const auto& projectile : Projectile::get_enemy_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_2->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_2->get_blockade_vector().begin(), m_blockade_2->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;

								if (blockade_rect_index < static_cast<int>(m_blockade_2->get_blockade_vector().size()))
								{
									m_blockade_2->get_blockade_vector().erase(m_blockade_2->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (enemy_projectile_index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size() - 1))
						{
							if (Projectile::get_enemy_projectiles_vector()[enemy_projectile_index])
								enemy_projectile_index++;
						}

					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			m_blockade_2->clean_blockade_vector();

			if (m_blockade_2->get_blockade_vector().empty())
			{
				delete m_blockade_2;
				m_blockade_2 = nullptr;
				printf("m_blockade_2 deleted\n");
			}
		}

		enemy_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_3)
		{

			for (const auto& projectile : Projectile::get_enemy_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_3->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_3->get_blockade_vector().begin(), m_blockade_3->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;

								if (blockade_rect_index < static_cast<int>(m_blockade_3->get_blockade_vector().size()))
								{
									m_blockade_3->get_blockade_vector().erase(m_blockade_3->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (enemy_projectile_index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size() - 1))
						{
							if (Projectile::get_enemy_projectiles_vector()[enemy_projectile_index])
								enemy_projectile_index++;
						}

					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			m_blockade_3->clean_blockade_vector();

			if (m_blockade_3->get_blockade_vector().empty())
			{
				delete m_blockade_3;
				m_blockade_3 = nullptr;
				printf("m_blockade_3 deleted\n");
			}
		}

		enemy_projectile_index = 0;
		blockade_rect_index = 0;

		if (m_blockade_4)
		{

			for (const auto& projectile : Projectile::get_enemy_projectiles_vector())
			{
				for (auto& blockade_rects : m_blockade_4->get_blockade_vector())
				{
					if (projectile)
					{
						if (blockade_rects)
						{
							if (Collision::is_colliding(projectile->get_projectile_rect(), blockade_rects))
							{
								Blockade::set_destroyed_block(blockade_rects);
								std::for_each(m_blockade_4->get_blockade_vector().begin(), m_blockade_4->get_blockade_vector().end(), &Blockade::is_within_blast_range);
								Blockade::set_destroyed_block(nullptr);
								delete blockade_rects;
								blockade_rects = nullptr;

								if (blockade_rect_index < static_cast<int>(m_blockade_4->get_blockade_vector().size()))
								{
									m_blockade_4->get_blockade_vector().erase(m_blockade_4->get_blockade_vector().begin() + blockade_rect_index);
								}
								else
								{
									printf("index exceeds blockade size\n");
								}

								projectile->set_health(0);
							}
						}

						if (enemy_projectile_index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size() - 1))
						{
							if (Projectile::get_enemy_projectiles_vector()[enemy_projectile_index])
								enemy_projectile_index++;
						}

					}

					else
					{
						break;
					}

					if (blockade_rects) { blockade_rect_index++; }

				}

				blockade_rect_index = 0;
			}

			m_blockade_4->clean_blockade_vector();

			if (m_blockade_4->get_blockade_vector().empty())
			{
				delete m_blockade_4;
				m_blockade_4 = nullptr;
				printf("m_blockade_4 deleted\n");
			}
		}

		//-------------------------------------------------------------------------------------

		for (int index = 0; index < static_cast<int>(Projectile::get_player_projectiles_vector().size());)
		{

			if (Projectile::get_player_projectiles_vector()[index]->get_health() < 1)
			{
				delete Projectile::get_player_projectiles_vector()[index];
				Projectile::get_player_projectiles_vector()[index] = nullptr;
				Projectile::get_player_projectiles_vector().erase(Projectile::get_player_projectiles_vector().begin() + index);
				printf("projectile #%d deleted\n", index);
			}
			else
			{
				index++;
			}
		}

		for (int index = 0; index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size());)
		{

			if (Projectile::get_enemy_projectiles_vector()[index]->get_health() < 1)
			{
				delete Projectile::get_enemy_projectiles_vector()[index];
				Projectile::get_enemy_projectiles_vector()[index] = nullptr;
				Projectile::get_enemy_projectiles_vector().erase(Projectile::get_enemy_projectiles_vector().begin() + index);
				printf("projectile #%d deleted\n", index);
			}
			else
			{
				index++;
			}
		}

		if (s_player->get_projectile_type() == Projectile::EXPLOSIVE)
		{
			for (Enemy*& enemy_ptr : Enemy::enemy_arr)
			{
				if (enemy_ptr)
				{
					if (Collision::is_colliding(&temp_explosive, enemy_ptr->get_image_rect()))
					{
						enemy_ptr->update_death_timer();
						enemy_ptr->health = 0;
					}
				}
			}
		}


		for (int index = 0; index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size()); index++)
		{
			if (Projectile::get_enemy_projectiles_vector()[index])
			{
				if (Projectile::get_enemy_projectiles_vector()[index]->get_projectile_rect()->y > Window::win_surface()->h)
				{
					delete Projectile::get_enemy_projectiles_vector()[index];
					Projectile::get_enemy_projectiles_vector().erase(Projectile::get_enemy_projectiles_vector().begin() + index);
					printf("projectile deleted\n");
					if (index > 0) { --index; }
				}

				else if (index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size()))
				{
					if (Collision::is_colliding(Projectile::get_enemy_projectiles_vector()[index]->get_projectile_rect(), s_player))
					{
						delete Projectile::get_enemy_projectiles_vector()[index];
						Projectile::get_enemy_projectiles_vector().erase(Projectile::get_enemy_projectiles_vector().begin() + index);
						printf("projectile deleted\n");
						if (index > 0) { --index; }
						m_player_died = true;
						Projectile::get_enemy_projectiles_vector().clear();
						if (s_player->get_lives() > 0) { s_player->decrement_lives(); }
						else { s_is_game_finished = true; s_is_game_won = false; s_score = 0; }
					}
				}
			}
		}


			
	}

	if (s_is_game_finished)
	{

		if (s_is_game_won == false)
		{
			s_player->reset_lives();
		}

		else { s_level++; }

		delete Player::get_sprite_sheet();
		delete Enemy::get_sprite_sheet();

		for (int index = 0; index < static_cast<int>(Projectile::get_player_projectiles_vector().size());)
		{
			if (Projectile::get_player_projectiles_vector()[index])
			{
				delete Projectile::get_player_projectiles_vector()[index];
				Projectile::get_player_projectiles_vector()[index] = nullptr;
			}
			else { index++; }
		}

		for (int index = 0; index < static_cast<int>(Projectile::get_enemy_projectiles_vector().size());)
		{
			if (Projectile::get_enemy_projectiles_vector()[index])
			{
				delete Projectile::get_enemy_projectiles_vector()[index];
				Projectile::get_enemy_projectiles_vector()[index] = nullptr;
			}
			else { index++; }
		}

		Projectile::get_player_projectiles_vector().clear();
		Projectile::get_enemy_projectiles_vector().clear();


		delete m_blockade_1;
		delete m_blockade_2;
		delete m_blockade_3;
		delete m_blockade_4;

		//TTF_CloseFont(m_font);

		delete m_UFO;
		m_UFO = nullptr;

		delete m_UFO_power_up;
		m_UFO_power_up = nullptr;

		delete s_player;
		s_player = nullptr;

		delete m_enemies;
		m_enemies = nullptr;

		for (int i = 0; i < MAX_ENEMY_COUNT; i++) { delete Enemy::enemy_arr[i]; Enemy::enemy_arr[i] = nullptr; }

	}


}

Player* Game::get_player()
{
	return s_player;
}

bool Game::get_is_game_finished()
{
	return s_is_game_finished;
}

bool Game::get_is_game_won()
{
	return s_is_game_won;
}

void Game::set_game_finished()
{
	s_is_game_finished = true;
}

void Game::load_font(const char* file_path)
{
	if (!m_font)
		m_font = TTF_OpenFont(file_path, Window::win_surface()->w / 50);
	
	if (!m_font)
	{
		printf("Couldn't open font. Error: %s\n", SDL_GetError());
	}
}

void Game::create_surface_from_font(const char* text)
{
	if (m_texture_text)
	{
		SDL_DestroyTexture(m_texture_text);
		m_texture_text = nullptr;

	}

	if (m_surface_text)
	{
		SDL_FreeSurface(m_surface_text);
		m_surface_text = nullptr;

	}
	
	m_surface_text = TTF_RenderText_Solid(m_font, text, SDL_Color{ 255, 255, 255, 255 });

	if(!m_surface_text)
	{
		printf("Couldn't create text surface. Error: %s\n", SDL_GetError());
	}

	else
	{
		m_texture_text = SDL_CreateTextureFromSurface(Render::get_renderer(), m_surface_text);

		if(!m_texture_text)
		{
			printf("Couldn't create text texture. Error: %s\n", SDL_GetError());
		}
	}

	if (m_surface_text)
	{
		SDL_FreeSurface(m_surface_text);
		m_surface_text = nullptr;

	}
	
}

void Game::render_score()
{

	const std::string score_string = std::to_string(s_score);
	const std::string score = "score: ";
	const std::string temp = score + score_string;
	create_surface_from_font(temp.c_str());
	if (m_texture_text){ Render::render_score(m_texture_text); }
}

void Game::render_player_death()
{

	constexpr int death_max_timer = 420;

	if (player_death_timer > 0)
	{
		if (player_death_timer % 40 >= 30)
			Render::render_player_destroyed(s_player, 1);

		else if (player_death_timer % 40 < 30 && player_death_timer % 40 >= 20)
			Render::render_player_destroyed(s_player, 2);

		else if (player_death_timer % 40 < 20 && player_death_timer % 40 >= 10)
			Render::render_player_destroyed(s_player, 3);

		else
			Render::render_player_destroyed(s_player, 4);

		--player_death_timer;
	}

	else
	{
		player_death_timer = 420;
		m_should_revive = true;
	}
	
}



int Game::get_casualties() const
{
	return m_enemy_casualties;
}



