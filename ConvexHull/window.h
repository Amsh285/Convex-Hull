#pragma once


#include <memory>
#include <string>

#include "SDL.h"

#include "drawing_context.h"
#include "window_eventargs.h"

namespace ch_project
{
	class window
	{
	public:
		window() : window("untitled") {};
		window(const std::string& title) : m_title(title), m_width(1024), m_height(820) {};

		void display();

	protected:
		virtual void handle_event(const SDL_Event& event, window_eventargs& args) {};
		virtual void update(const ch_project::drawing_context& context) {};
	private:
		std::string m_title;
		std::size_t m_width, m_height;
	};
}

