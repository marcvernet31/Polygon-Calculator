#include "ConvexPoligon.hh"
#include "Point.hh"

#include <iostream>
#include <vector>
using namespace std;

//precision
const double E = 1e-12;

// Constructor.
ConvexPoligon::ConvexPoligon(vector <Point> v): _v(v){};

//Polygon perimeter
double ConvexPoligon::getPerimeter() const{
  double perimeter = 0;
  double length = _v.size();
  if(length > 1){
    for(unsigned int i = 1; i < length; ++i)  perimeter += _v[i-1].distance(_v[i]);
    if(length != 2) perimeter += _v[0].distance(_v[length-1]);
  }
  return perimeter;
}

//Polygon area
double ConvexPoligon::getArea() const{
  double polygonSize = _v.size();
  double a = 0, b = 0;
  if(polygonSize > 1){
    for(unsigned int i = 0; i < polygonSize-1; ++i){
      a += _v[i].get_x() * _v[i+1].get_y();
      b += _v[i].get_y() * _v[i+1].get_x();
    }
    return abs((a - b)/2);
  }
  else return 0;
}

//Polygon number of vertexs
int ConvexPoligon::getVertexNumber() const{
  return _v.size();
}

//Polygon number of edges;
int ConvexPoligon::getEdgesNumber() const{
  return _v.size();
}

//True if a polygon is regular;
bool ConvexPoligon::isRegular() const{
  bool regular = true;
  if(_v.size() > 1){
    double expectedDistance = _v[0].distance(_v[1]);
    for(unsigned int i = 1; i < _v.size(); ++i){
      if(_v[i-1].distance(_v[i]) != expectedDistance) regular = false;
    }
    return regular;
  }
  return false;
}

//Returns the coordinates of the centroid
Point ConvexPoligon::getCentroid() const{
  double sumX = 0;
  double sumY = 0;
  int polygonSize = _v.size();

  if(polygonSize > 0){
    for(Point a : _v){
      sumX += a.get_x();
      sumY += a.get_y();
    }
    return Point({sumX / polygonSize, sumY / polygonSize});
  }
  return 0;
}

//Returns the vector of Points of a polygon
vector<Point> ConvexPoligon::getPoints(){
  vector<Point> AUX;
  for(Point a : _v){
    AUX.push_back(a);
  }
  return AUX;
}

//Returns the color of a polygon
vector<double> ConvexPoligon::getColor(){
  vector<double> AUX;
  for(double& a : _RGB){
    AUX.push_back(a);
  }
  return AUX;
}

/*Line used for intersection function
  y = mx + b; */
struct Line {
  //y = mx + b;
  double m; //slope
  double b; //y-intercept
};

/* Auxiliar for instersection
  Tells if a two Point segment is vertical */
bool isVertical(Point P, Point Q){
  if(P.get_x() == Q.get_x()) return true;
  return false;
}

/* Auxiliar for instersection
  Returns True if A and B are in the same side respect PQ */
static bool sameSide(Point P, Point Q, Point A, Point B){
  double dx = P.get_x() - Q.get_x();
  double dy = P.get_y() - Q.get_y();
  double dxA = A.get_x() - P.get_x();
  double dyA = A.get_y() - P.get_y();
  double dxB = B.get_x() - P.get_x();
  double dyB = B.get_y() - P.get_y();
  return (dy*dxA - dx*dyA > 0) == (dy*dxB - dx*dyB > 0);
}

/* Auxiliar for instersection
  Returns True if segment AB intersects PQ */
static bool intersect(Point P, Point Q, Point A, Point B){
  return not (sameSide(P, Q, A, B) or sameSide(A, B, P, Q));
}

/*Auxiliar for instersection
  Creates a Line from a segment of two points*/
Line segmentToLine(Point P, Point Q){
  Line AUX;
  AUX.m = (P.get_y() - Q.get_y()) / (P.get_x() - Q.get_x());
  AUX.b = P.get_y() - AUX.m * P.get_x();
  return AUX;
}

/*Auxiliar for instersection
  Creates the cutting Points in intersecting segments(works for vertical segments)*/
void createPointVertical(vector <Point>& punts, Point Q, Point W, Point E, Point R){
  if(isVertical(Q, W) and not isVertical(E, R)){
    Line inclinada = segmentToLine(E,R);
    double x = Q.get_x();
    double y = inclinada.m * x + inclinada.b;
    punts.push_back(Point{x,y});
  }
  else if(not isVertical(Q, W) and isVertical(E, R)){
    Line inclinada = segmentToLine(Q,W);
    double x = E.get_x();
    double y = inclinada.m * x + inclinada.b;
    punts.push_back(Point{x,y});
  }
}

/*Auxiliar for instersection
  Creates the cutting Points in intersecting segments*/
static void createPoint(vector <Point>& punts, Point Q, Point W, Point E, Point R){
  if(not isVertical(Q,W) and not isVertical(E,R)){
    Line A = segmentToLine(Q, W);
    Line B = segmentToLine(E, R);

    double x = (B.b - A.b) / (A.m - B.m);
    double y = x * B.m + B.b;
    if(x == -0.0) x = 0.0;
    if(y == -0.0) y = 0.0;
    punts.push_back(Point({x,y}));
  }
  else createPointVertical(punts, Q, W, E, R);
}

/*Creates the cutting Points in intersecting segments for all the polygon*/
vector <Point> ConvexPoligon::puntsDeTall (ConvexPoligon P){
  vector <Point> AUX;
  vector <Point> A = _v;
  vector <Point> B = P.getPoints();

  int sizeA = A.size();
  int sizeB = B.size();
  if(sizeA > 0 and sizeB > 0){
    for(int i = 1; i < sizeA; ++i){
      for(int j = 1; j < sizeB; ++j) if(intersect(A[i-1], A[i], B[j-1], B[j])){
        createPoint(AUX, A[i-1], A[i], B[j-1], B[j]);
      }
      if(intersect(A[i-1], A[i], B[0], B[sizeB - 1]) and sizeB > 2) createPoint(AUX, A[i-1], A[i], B[0], B[sizeB -1]);
    }
    for(int j = 1; j < sizeB; ++j) if(intersect(A[0], A[sizeA - 1], B[j-1], B[j]) and sizeA > 2){
      createPoint(AUX, A[0], A[sizeA -1 ], B[j-1], B[j]);
    }
    if(intersect(A[0], A[sizeA - 1], B[0], B[sizeB - 1]) and sizeA > 2 and sizeB > 2) createPoint(AUX, A[0], A[sizeA - 1], B[0], B[sizeB -1]);
    return AUX;
  }
  else return AUX;
}

//Returns True if a Points is inside a polygon
bool ConvexPoligon::isPointInside(Point B){
  vector <Point> v = {Point({B.get_x(), B.get_y()}), Point({99999999999, 0})}; //line to inf
  ConvexPoligon AUX (v);
  ConvexPoligon A (_v);
  if(A.puntsDeTall(AUX).size() == 1) return true;
  for(Point a : _v) if(B == a){
    return true;
  }
  return false;
}

//Changes the color of a polygon
void ConvexPoligon::changeColor(double r, double g, double b){
  _RGB = {r, g, b};
}

//Chenages the points of a polygon
void ConvexPoligon::changePoints(vector<Point> v){
  _v = v;
}

//Tells if a polygon is empty
bool ConvexPoligon::isEmpty(){
  if(_v.size() == 0) return true;
  return false;
}
