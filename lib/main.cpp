#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <stdlib.h>  
#include <array>

#include <SFML/Graphics.hpp>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"

typedef Vector2<double> Vec2f;

double RandomFloat(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

int main()
{
	srand (time(NULL));
	double numberPoints = roundf(RandomFloat(4, 40));

	std::cout << "Generating " << numberPoints << " random points" << std::endl;

	std::vector<Vec2f> points;
	for(int i = 0; i < numberPoints; i++) {
		points.push_back(Vec2f(RandomFloat(0, 800), RandomFloat(0, 600)));
	}

	Delaunay triangulation;
	std::vector<Triangle> triangles = triangulation.triangulate(points);
	std::cout << triangles.size() << " triangles generated\n";
	std::vector<Edge> edges = triangulation.getEdges();
	
	std::cout << " ========= ";
	
	std::cout << "\nPoints : " << points.size() << std::endl;
	for(auto &p : points)
		std::cout << p << std::endl;
	
	std::cout << "\nTriangles : " << triangles.size() << std::endl;
	for(auto &t : triangles)
		std::cout << t << std::endl;

	std::cout << "\nEdges : " << edges.size() << std::endl;
	for(auto &e : edges)
		std::cout << e << std::endl;
			
	// SFML window
    	sf::RenderWindow window(sf::VideoMode(800, 600), "Delaunay triangulation");

	// Transform each points of each vector as a rectangle
	std::vector<sf::RectangleShape*> squares;

	for(auto p = begin(points); p != end(points); p++) {
		sf::RectangleShape *c1 = new sf::RectangleShape(sf::Vector2f(4, 4));
		c1->setPosition(p->x, p->y);
		squares.push_back(c1);
	}
	
	// Make the lines
	std::vector<std::array<sf::Vertex, 2> > lines;
	for(auto e = begin(edges); e != end(edges); e++) {
		lines.push_back({{
			sf::Vertex(sf::Vector2f((*e).p1.x + 2, (*e).p1.y + 2)),	
			sf::Vertex(sf::Vector2f((*e).p2.x + 2, (*e).p2.y + 2))	
		}});
	}
 
	while (window.isOpen())
	{
	        sf::Event event;
	        while (window.pollEvent(event))
	        {
	            if (event.type == sf::Event::Closed)
	                window.close();
	        }
	
	        window.clear();
	
		// Draw the squares
		for(auto s = begin(squares); s != end(squares); s++) {
			window.draw(**s);
		}
	
		// Draw the lines
		for(auto l = begin(lines); l != end(lines); l++) {
			window.draw((*l).data(), 2, sf::Lines);
		}
	       	
		window.display();
	}
	
	return 0;
}
