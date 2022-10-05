#include "window.h"

void ch_project::window::display()
{
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	{
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window(
			SDL_CreateWindow("Point drawing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 820, 0),
			[](SDL_Window* win) { SDL_DestroyWindow(win); }
		);

		std::shared_ptr<SDL_Renderer> renderer(
			SDL_CreateRenderer(window.get(), -1, 0),
			[](SDL_Renderer* ren) { SDL_DestroyRenderer(ren); }
		);

		ch_project::drawing_context ctx(renderer);

		while (true)
		{
			SDL_Delay(10);
			SDL_PollEvent(&event);

			window_eventargs args;
			handle_event(event, args);

			if (args.cancel)
				break;

			SDL_SetRenderDrawColor(renderer.get(), 242, 242, 242, 255);
			SDL_RenderClear(renderer.get());

			update(ctx);
			SDL_RenderPresent(renderer.get());
		}
	}

	SDL_Quit();
}
