#pragma once
#include "SDL2/SDL2/include/SDL_stdinc.h"

struct SDL_Window;
struct SDL_Surface;
class Window

{
public:
	Window(const Window& other) = delete;
	static Window& get();
	static SDL_Window* get_window();
	static bool create_window(const char* name, int x_pos, int y_pos, int width, int height, Uint32 window_flag);
	static SDL_Surface* win_surface();
	static int get_window_divisions();
private:
	static bool create_window_impl(const char* name, int x_pos, int y_pos, int width, int height, Uint32 window_flag);
	static int s_window_divisions;
	Window(){}
	SDL_Window* s_window;

};

