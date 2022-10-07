// ConvexHull.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "SDL.h"

#include "point_drawing_window.h"
#include "quickhull_window.h"


void run_quickhull(std::vector<vector2f> point_cloud,
	std::shared_ptr<quickhull_window> window)
{
	if (point_cloud.size() == 1)
		return;

	std::vector<std::pair<vector2f, vector2f>> hull;
	std::sort(
		point_cloud.begin(),
		point_cloud.end(),
		[](const vector2f& left, const vector2f& right) {return left.X() < right.X(); }
	);

	
	std::pair<vector2f, vector2f> maxima = std::make_pair(*point_cloud.begin(), *point_cloud.rbegin());
	hull.push_back(maxima);
	window->set_hull(hull);

	
}


int main(int argc, char* argv[])
{
	std::vector<vector2f> point_cloud;

	point_drawing_window point_drawing("Point Drawing");
	point_drawing.display();
	point_cloud = point_drawing.get_vectors();



	/*quickhull_window q_window("Quickhull", point_cloud);*/
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
