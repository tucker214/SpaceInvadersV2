#pragma once

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Renderer;

class LTexture
{
public:
	LTexture();
	LTexture(const char* file_path);
	~LTexture();
	SDL_Texture* load_from_file(const char* file_path);
	SDL_Texture* get_texture() const;
	void resize(int width, int height);
	static LTexture* create_l_texture(const char* file_path);
private:
	SDL_Texture* m_texture;
	SDL_Surface* m_surface;
	const char* m_file_path;
};

