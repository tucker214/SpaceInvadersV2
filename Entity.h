#pragma once

class LTexture;

class Entity
{
public:
	Entity();
	Entity(const char* file_path);
	Entity(const char* file_path, int x_pos, int y_pos);
	virtual ~Entity();
	void set_l_texture(const char* file_path);
	LTexture* get_l_texture() const;
	int get_x_pos() const;
	int get_y_pos() const;
	int get_width() const;
	int get_height() const;
	int get_width_2() const;
	int get_height_2() const;
	void set_x_pos(const int x_pos);
	void set_y_pos(const int y_pos);
	void increment_y_pos(const int y_delta);
	int health = 1;
protected:
	int m_x_pos = 0;
	int m_y_pos = 0;
	LTexture* m_l_texture;
};

