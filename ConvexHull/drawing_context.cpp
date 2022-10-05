#include "drawing_context.h"


void ch_project::drawing_context::draw_point(const vector2f& position_vector, const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a, const float& scale_x, const float& scale_y) const
{
	SDL_SetRenderDrawColor(m_renderer.get(), r, g, b, a);
	SDL_RenderSetScale(m_renderer.get(), scale_x, scale_y);
	SDL_RenderDrawPoint(m_renderer.get(), position_vector.X() / scale_x, position_vector.Y() / scale_y);
}
