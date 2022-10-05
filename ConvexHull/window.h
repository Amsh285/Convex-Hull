#pragma once

#include <functional>
#include <memory>

#include "SDL.h"

#include "drawing_context.h"
#include "window_eventargs.h"

namespace ch_project
{
	class window
	{
	public:
		void display();
		
	protected:
		virtual void handle_event(const SDL_Event& event, window_eventargs& args) {};
		virtual void update(const ch_project::drawing_context& context) {};
	};
}

