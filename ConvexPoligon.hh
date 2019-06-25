#include "Point.hh"

#include <iostream>
#include <vector>
using namespace std;

#ifndef ConvexPoligon_hh
#define ConvexPoligon_hh

class ConvexPoligon {
public:

  //constructor
  ConvexPoligon(vector <Point> _v);

  //Polygon perimeter
  double getPerimeter() const;

  //Polygon area;
  double getArea() const;

  //Polygon number of vertex
  int getVertexNumber() const;

  //Polygon number of edges;
  int getEdgesNumber() const;

  //Tells if a poligon is regular;
  bool isRegular() const;

  //Coordinates of the centroid (average mean of all points)
  Point getCentroid() const;

  //Vector of Points in the polygon
  vector<Point> getPoints();

  //Returns the color of the polygon
  vector<double> getColor();

  //Vector of the cutting points in two ConvexPoligons
  vector <Point> puntsDeTall (ConvexPoligon P);

  //Tells if a point is inside a ConvexPoligon
  bool isPointInside(Point B);

  //Changes the color of a ConvexPoligon
  void changeColor(double r, double g, double b);

  //Changes the Points of a ConvexPoligon
  void changePoints(vector<Point> v);

  //Tells if a polygon is empty
  bool isEmpty();

private:
  vector <Point> _v;
  vector <double> _RGB = {0.0, 0.0, 0.0};
};

#endif
