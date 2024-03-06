#include <iostream>
#include <SDL_ttf.h>
#include "Window.h"
#include "Render.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;


void init_main();


int main(int argc, char* argv[])
{
	Uint64 time_now = SDL_GetPerformanceCounter();
	Uint64 time_last = 0;
	
	//initialize video, audio, window, and renderer
	init_main();

	Game game;

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		game = Game();
		while (!Game::get_is_game_finished() && !quit)
		{
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{

				game.update();
				game.render();
				game.collision();
			}
		}
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	SDL_QuitSubSystem(IMG_INIT_JPG);
	SDL_QuitSubSystem(IMG_INIT_PNG);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;

}

void init_main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){ printf("SDL could not be initialized. Error: %s", SDL_GetError()); }
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) { printf("SDL_image could not be initialized. Error: %s", SDL_GetError()); }
	if (TTF_Init() < 0) { printf("SDL TTF could not be initialized. Error: %s", SDL_GetError()); }
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { printf("Couldn't initialize SDL Mixer. Error: %s", SDL_GetError()); }

	Window::get().create_window("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	Render::create_renderer();


}

