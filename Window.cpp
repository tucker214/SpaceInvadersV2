#include "Window.h"
#include <cstdio>
#include <SDL_video.h>

//static SDL_Window* s_window;
int Window::s_window_divisions = 19;

Window& Window::get()
{
	
	static Window s_window_instance;
	return s_window_instance;
}

SDL_Window* Window::get_window()
{
	return get().s_window;
}

bool Window::create_window(const char* name, int x_pos, int y_pos, int width, int height, Uint32 window_flag)
{
	return get().create_window_impl(name, x_pos, y_pos, width, height, window_flag);
}

SDL_Surface* Window::win_surface()
{
	return SDL_GetWindowSurface(get_window());
}

int Window::get_window_divisions()
{
	return get().s_window_divisions;
}



bool Window::create_window_impl(const char* name, int x_pos, int y_pos, int width, int height, Uint32 window_flag)
{
	bool success = true;

	if (get().s_window == nullptr)
	{
		get().s_window = SDL_CreateWindow(name, x_pos, y_pos, width, height, window_flag);
		printf("Window successfully created\n");
	}
	else
	{
		printf("Window instance already created. Couldn't create window\n");
		success = false;
	}

	if (get().s_window == nullptr)
	{
		printf("s_window couldn't be created. Error: %s\n", SDL_GetError());
		success = false;
	}
	return success;
}
