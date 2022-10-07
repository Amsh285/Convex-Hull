// ConvexHull.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <thread>
#include <vector>

#include "SDL.h"

#include "conversions.h"

#include "point_drawing_window.h"
#include "quickhull_window.h"

#include "matrix3f.h"
#include "matrix3f_factory.h"

#include "math_helpers.h"


void find_hull(const std::vector<vector2f>& remaining_data, const vector2f& from, const vector2f& to,
	std::vector<std::pair<vector2f, vector2f>>& hull, std::shared_ptr<quickhull_window>& window,
	const ch_project::color_rgba& helpline_color)
{
	if (remaining_data.empty())
		return;

	// distanz-abfrage
	// geht so auch:
	/*vector2f separation_line = to - from;

	std::pair<float, std::size_t> d_largest = std::make_pair(0.0f, 0);

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		ch_project::vector3f p = ch_project::conversions::homogenized_vector(*it);
		ch_project::vector3f a = ch_project::conversions::homogenized_vector(to);
		ch_project::vector3f b = ch_project::conversions::homogenized_vector(separation_line);

		float d = ((p - a).cross(b)).norm() / b.norm();

		if (d_largest.first < d)
			d_largest = std::make_pair(d, it - remaining_data.begin());
	}

	window->add_dot(remaining_data[d_largest.second], ch_project::color_rgba(255, 0, 0, 255), 6.0f, 6.0f);*/


	/*Dreiecke für die GUI*/
	std::vector<std::pair<vector2f, vector2f>> helplines;

	vector2f separation_line = to - from;
	vector2f separation_line_normalized = separation_line.normalized();
	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(from * (-1));

	std::pair<float, std::size_t> hull_candidate = std::make_pair(0, 0);

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_hyp = (t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		vector2f hyp_normalized = v_hyp.normalized();
		float hyp = v_hyp.norm();
		float cosine = cos(hyp_normalized.angle_r(separation_line_normalized));
		float adjacent = cosine * hyp;

		vector2f normal(separation_line_normalized.Y(), separation_line_normalized.X() * (-1));
		float opposite = sqrt(pow(hyp, 2) - pow(adjacent, 2));

		vector2f hl_from = from + (separation_line_normalized * adjacent);
		vector2f hl_to = hl_from + (normal * opposite);

		helplines.push_back(std::make_pair(hl_from, hl_to));

		if (hull_candidate.first < opposite)
			hull_candidate = std::make_pair(opposite, it - remaining_data.begin());
	}

	vector2f c = remaining_data[hull_candidate.second];

	window->add_lines(helplines, helpline_color, 1.0f, 1.0f);
	window->add_dot(c, ch_project::color::red(), 6.0f, 6.0f);

	hull.push_back(std::make_pair(from, c));
	hull.push_back(std::make_pair(c, to));




	SDL_Delay(2000);
	window->clear_dots();
	window->set_hull(hull);
	SDL_Delay(2000);
	window->clear_lines();

	vector2f p_to_c = c - from;
	std::vector<vector2f> s1;

	vector2f c_to_q = to - c;
	std::vector<vector2f> s2;

	ch_project::matrix3f t_relative_p_to_c = ch_project::prebuilt::homogenized_translate(from * (-1));
	ch_project::matrix3f t_relative_c_to_q = ch_project::prebuilt::homogenized_translate(c * (-1));

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_align_p_c = (t_relative_p_to_c * ch_project::conversions::homogenized_position(*it)).operator vector2f();
		vector2f v_align_c_q = (t_relative_c_to_q * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		float p_to_c_det = p_to_c.cross(v_align_p_c);
		float c_to_q_det = c_to_q.cross(v_align_c_q);

		if (p_to_c_det < 0)
			s1.push_back(*it);

		if (c_to_q_det < 0)
			s2.push_back(*it);
	}

	window->add_dots(s1, ch_project::color::red(), 3.0f, 3.0f);
	window->add_dots(s2, ch_project::color::blue(), 3.0f, 3.0f);
	SDL_Delay(2000);
	window->clear_dots();

	find_hull(s1, from, c, hull, window, helpline_color);
	find_hull(s2, c, to, hull, window, helpline_color);
}


void run_quickhull(std::vector<vector2f> considered_points,
	std::shared_ptr<quickhull_window> window)
{
	std::vector<std::pair<vector2f, vector2f>> hull;
	std::sort(
		considered_points.begin(),
		considered_points.end(),
		[](const vector2f& left, const vector2f& right) {return left.X() < right.X(); }
	);

	std::pair<vector2f, vector2f> maxima = std::make_pair(*considered_points.begin(), *considered_points.rbegin());
	hull.push_back(std::make_pair(maxima.first, maxima.second));
	considered_points.erase(considered_points.begin());
	considered_points.pop_back();

	SDL_Delay(1000);
	window->set_hull(hull);

	vector2f separating_line = maxima.second - maxima.first;

	std::vector<vector2f> left;
	std::vector<vector2f> right;

	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(maxima.first * (-1));

	for (auto it = considered_points.begin(); it < considered_points.end(); ++it)
	{
		ch_project::vector3f relative_to_separation_line =
			t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it);

		float det = separating_line.cross(relative_to_separation_line.operator vector2f());

		if (det < 0)
			right.push_back(*it);
		else
			left.push_back(*it);
	}

	SDL_Delay(1000);

	window->add_dots(right, ch_project::color_rgba(255, 151, 0, 255), 3.0f, 3.0f);
	window->add_dots(left, ch_project::color_rgba(0, 114, 255, 255), 3.0f, 3.0f);

	SDL_Delay(2000);

	find_hull(right, maxima.first, maxima.second, hull, window, ch_project::color_rgba(255, 0, 0, 255));
	find_hull(left, maxima.second, maxima.first, hull, window, ch_project::color_rgba(0, 0, 255, 255));

	/*return hull;*/
}


int main(int argc, char* argv[])
{
	std::vector<vector2f> point_cloud;

	point_drawing_window point_drawing("Point Drawing");
	point_drawing.display();
	point_cloud = point_drawing.get_vectors();

	std::shared_ptr<quickhull_window> q_window(new quickhull_window("Quickhull", point_cloud));

	std::thread t1(run_quickhull, point_cloud, q_window);
	q_window->display();

	t1.join();

	return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
