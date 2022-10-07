#include "quickhull_window.h"

void quickhull_window::set_hull(ch_project::node<vector2f>* head)
{
	std::vector<vector2f> tmp;
	std::vector<std::pair<vector2f, vector2f>> hull;

	ch_project::node<vector2f>* tail = head;

	while (tail != nullptr)
	{
		tmp.push_back(tail->get_data());
		tail = tail->get_next();
	}

	std::size_t line_counter = 0;

	for (auto it = tmp.begin(); it < tmp.end() - 1; ++it)
	{
		++line_counter;
		hull.push_back(std::make_pair(*it, *(it + 1)));
	}

	if (line_counter > 1)
		hull.push_back(std::make_pair(*tmp.rbegin(), head->get_data()));

	set_hull(hull);
}

void quickhull_window::set_hull(const std::vector<std::pair<vector2f, vector2f>>& hull)
{
	std::lock_guard<std::mutex> guard(m_mut);
	m_hull = hull;
}

void quickhull_window::handle_event(const SDL_Event& event, ch_project::window_eventargs& args)
{
}

void quickhull_window::update(const ch_project::drawing_context& context)
{
	//Todo: fix that shit
	{
		for (auto it = m_point_cloud.begin(); it < m_point_cloud.end(); ++it)
			context.draw_point(*it, 0, 0, 0, 255, 3.0f, 3.0f);

		std::lock_guard<std::mutex> guard(m_mut);

		for (auto it = m_hull.begin(); it < m_hull.end(); ++it)
			context.draw_line(it->first, it->second, 0, 0, 0, 255, 1.0f, 1.0f);


		/*context.draw_line(vector2f(10.0f, 10.0f), vector2f(1000.0f, 10.0f),
			0, 0, 255, 255, 1.0f, 1.0f);*/

		context.draw_line(vector2f(900.0f, 10.0f), vector2f(1000.0f, 10.0f),
			0, 0, 255, 255, 1.0f, 1.0f);

		context.draw_line(vector2f(800.0f, 20.0f), vector2f(1000.0f, 20.0f),
			0, 0, 255, 255, 1.0f, 1.0f);
	}

	//this too
	ch_project::window::update(context);
}
