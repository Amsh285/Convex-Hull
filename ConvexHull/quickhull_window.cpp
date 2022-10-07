#include "quickhull_window.h"

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
	for (auto it = m_point_cloud.begin(); it < m_point_cloud.end(); ++it)
		context.draw_point(*it, 0, 0, 0, 255, 3.0f, 3.0f);

	std::lock_guard<std::mutex> guard(m_mut);

	for (auto it = m_hull.begin(); it < m_hull.end(); ++it)
		context.draw_line(it->first, it->second, 0, 0, 0, 255, 1.0f, 1.0f);


}
