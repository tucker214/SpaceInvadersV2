#include "Entity.h"
#include "LTexture.h"
#include "Render.h"


Entity::Entity()
{
}

Entity::Entity(const char* file_path)
{
	m_l_texture = LTexture::create_l_texture(file_path);
}

Entity::Entity(const char* file_path, int x_pos, int y_pos) : m_x_pos(x_pos), m_y_pos(y_pos)
{
	m_l_texture = LTexture::create_l_texture(file_path);
}

Entity::~Entity()
{
	if (m_l_texture != nullptr)
	{
		SDL_DestroyTexture(m_l_texture->get_texture());
		delete m_l_texture;
		m_l_texture = nullptr;
	}
}

void Entity::set_l_texture(const char* file_path)
{
	m_l_texture = LTexture::create_l_texture(file_path);
}

LTexture* Entity::get_l_texture() const
{
	return m_l_texture;
}

int Entity::get_x_pos() const
{
	return m_x_pos;
}

int Entity::get_y_pos() const
{
	return m_y_pos;
}

int Entity::get_width() const
{
	int width = 0;
	if (m_l_texture)
	{
		SDL_QueryTexture(m_l_texture->get_texture(), nullptr, nullptr, &width, nullptr);
	}

	return width;
}

int Entity::get_height() const
{
	int height = 0;
	if (m_l_texture)
	{
		SDL_QueryTexture(m_l_texture->get_texture(), nullptr, nullptr, nullptr, &height);
	}

	return height;
}

int Entity::get_width_2() const
{
	return get_width() / 2;
}

int Entity::get_height_2() const
{
	return get_height() / 2;
}

void Entity::set_x_pos(const int x_pos)
{
	m_x_pos = x_pos;
}

void Entity::set_y_pos(const int y_pos)
{
	m_y_pos = y_pos;
}

void Entity::increment_y_pos(const int y_delta)
{
	m_y_pos += y_delta;
}

