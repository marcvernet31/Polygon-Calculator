#include "Point.hh"
#include "ConvexPoligon.hh"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <pngwriter.h>
#include <map>
#include <fstream>
#include <vector>
using namespace std;

//precision
const double E = 1e-12;

//ERRORS
const vector <string> errorReturn{
	"error: undefined polygon identifier",
	"error: invalid command",
	"error: wrong color format"
};

/* Auxiliar for convexHull,
 calculates the cross product of AB and AC*/
double crossProduct(Point A, Point B, Point C) {
	return (B.get_x() - A.get_x()) * (C.get_y() - A.get_y()) - (B.get_y() - A.get_y()) * (C.get_x() - A.get_x());
}

/*Auxiliar for union,
returns the vector of points of the polygon called */
vector<Point> points(const map <string, ConvexPoligon>& poligons, string name){
	auto it = poligons.find(name);
	ConvexPoligon p = it->second;
	return p.getPoints();
}

/*Auxiliar for union,
 concatenates two vectors	*/
vector <Point> unionVectors(const vector<Point>& A, const vector<Point>& B){
	vector<Point> AB;
	AB.reserve( A.size() + B.size() ); // preallocate memory
	AB.insert( AB.end(), A.begin(), A.end() );
	AB.insert( AB.end(), B.begin(), B.end() );
	return AB;
}

/* Returns a vector with the points that form the convex hull of the input,
uses the monotone chain algorithm*/
vector<Point> convexHull(vector<Point> input){
	int inputSize = input.size();
	if(inputSize <= 2) return input;
	else{
		int k = 0;
		vector<Point> Hull(2*inputSize);
		sort(input.begin(), input.end());

		for (int i = 0; i < inputSize; ++i) {
			while (k >= 2 and crossProduct(Hull[k-2], Hull[k-1], input[i]) <= 0) --k;
			Hull[k] = input[i];
			++k;
		}
		int t = k+1;
		for (int i = inputSize-2; i >= 0; --i) {
			while (k >= t and crossProduct(Hull[k-2], Hull[k-1], input[i]) <= 0) --k;
			Hull[k] = input[i];
			++k;
		}
		Hull.resize(k);
		reverse(Hull.begin(), Hull.end());
		Hull.pop_back();
		return Hull;
	}
}

/*Auxiliar for intersection,
returns a vector with all points of A inside B and all points of B inside A */
vector <Point> pointsInside(ConvexPoligon A, ConvexPoligon B){
	vector<Point> AUX;
	for(Point p : A.getPoints()){
		if(B.isPointInside(p)) AUX.push_back(p);
	}
	for(Point p : B.getPoints()){
		if(A.isPointInside(p)) AUX.push_back(p);
	}
	return AUX;
}


/* Returns the bounding box of a vector of Points*/
vector<Point> boundingBox(const vector<Point>& pointsBox){
	if(pointsBox.size() != 0){
		double xMinima = pointsBox[0].get_x();
		double xMaxima = pointsBox[0].get_x();
		double yMinima = pointsBox[0].get_y();
		double yMaxima = pointsBox[0].get_y();

		int n = pointsBox.size();
		for(int i = 0; i < n; ++i){
			if(pointsBox[i].get_x() < xMinima) xMinima = pointsBox[i].get_x();
			if(pointsBox[i].get_x() > xMaxima) xMaxima = pointsBox[i].get_x();
			if(pointsBox[i].get_y() < yMinima) yMinima = pointsBox[i].get_y();
			if(pointsBox[i].get_y() > yMaxima) yMaxima = pointsBox[i].get_y();
		}
		vector <Point> vertex;
		vertex.push_back(Point({xMaxima, yMaxima}));
		vertex.push_back(Point({xMaxima, yMinima}));
		vertex.push_back(Point({xMinima, yMinima}));
		vertex.push_back(Point({xMinima, yMaxima}));
		return vertex;
	}
	else return pointsBox;
}

/*Prints the name and points the polygon.*/
void print(map <string, ConvexPoligon>& poligons){
  string name;
  cin >> name;

  auto it = poligons.find(name);
  if(it != poligons.end()){
    vector <Point> AUX = it->second.getPoints();
    cout << name;
    for(Point p : AUX) cout << " " << p.get_x() << " " << p.get_y();
    cout << endl;
  }
	else cerr << errorReturn[0] << endl;
}

/*Calculates de ConvexHull of a group of points and creates a new polygon with it
or modifies an existent one*/
void createPolygon(map <string, ConvexPoligon>& poligons){
	string line;
	getline(cin, line);
	istringstream iss (line);

	string name;
	vector <Point> AUX;
	double coor_x, coor_y;
	iss >> name;
	while(iss >> coor_x >> coor_y) AUX.push_back(Point({coor_x, coor_y}));

  ConvexPoligon v (convexHull(AUX));
  auto it = poligons.find(name);
  if(it == poligons.end()) poligons.insert({name, v});
  else it->second = v;
	//no answer
	cout << "ok" << endl;
}

/* Prints the area of the polygon. */
void area(const map <string, ConvexPoligon>& poligons){
  string name;
  cin >> name;
  auto it = poligons.find(name);
	if(it != poligons.end()){
  	ConvexPoligon AUX = it->second;
  	cout << AUX.getArea() << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Prints the perimeter of the polygon. */
void perimeter(const map <string, ConvexPoligon>& poligons){
  string name;
  cin >> name;
  auto it = poligons.find(name);
	if(it != poligons.end()){
  	ConvexPoligon AUX = it->second;
  	cout << AUX.getPerimeter() << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Prints the number of vertices of the polygon. */
void vertices(const map <string, ConvexPoligon>& poligons){
  string name;
  cin >> name;
  auto it = poligons.find(name);
	if(it != poligons.end()){
  	ConvexPoligon AUX = it->second;
  	cout << AUX.getVertexNumber() << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Prints the coordenates of the centroid of the polygon. */
void centroid(const map <string, ConvexPoligon>& poligons){
  string name;
  cin >> name;
  auto it = poligons.find(name);
	if(it != poligons.end()){
  	ConvexPoligon AUX = it->second;
  	Point centroid = AUX.getCentroid();
  	cout << centroid.get_x() << " " << centroid.get_y() << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Prints a list of all the polygon names in the map. */
void list(const map <string, ConvexPoligon>& poligons){
  auto it = poligons.begin();
  while(it != poligons.end()){
    cout << it->first << " ";
    ++it;
  }
  cout << endl;
}

/* Saves the specified polygons in a .txt file*/
void save(map <string, ConvexPoligon>& poligons){
	string line, filename, name;
	getline(cin, line);
	istringstream iss (line);

	bool error = false;
	iss >> filename;
	ofstream File (filename, ios::app);
	while(iss >> name){
		auto it = poligons.find(name);
		if(it != poligons.end()){
			vector <Point> AUX = it->second.getPoints();
			File << name;
			for(Point a : AUX) File << " " << a.get_x() << " " << a.get_y();
			//no answer
			cout << "ok" << endl;
		}
		else error = true;
		File << endl;
	}
	if(error) cerr << errorReturn[0] << endl;
	File.close();
}

/* Imports polygons from a .txt file to the map.
Precondition: the first line of .txt must be blank*/
void load(map <string, ConvexPoligon>& poligons){
	string filename;
	cin >> filename;

	ifstream f(filename);
	string line;
	while(getline(f, line)){
		istringstream iss(line);
		string name;
		double x, y;
		vector<Point> AUX;
		iss >> name;
		while(iss >> x >> y) AUX.push_back(Point({x,y}));
		AUX = convexHull(AUX);
		poligons.insert({name, ConvexPoligon(AUX)});
	}
	f.close();
	//no answer
	cout << "ok" << endl;
}

/* Changes the color of a specified polygon*/
void setcol(map <string, ConvexPoligon>& poligons){
	string name;
	double r, g, b;
	cin >> name >> r >> g >> b;
	if((r>1 or g>1 or b>1) or (r<0 or g<0 or b<0)) cerr << errorReturn[2] << endl;
	auto it = poligons.find(name);	ConvexPoligon AUX = it->second;
	if(it != poligons.end()){
		it->second.changeColor(r,g,b);
		//no answer
		cout << "ok" << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Determines the size for scaling the polygons*/
double scaling(vector <Point> boundingBox){
	double distanceX = abs(boundingBox[0].get_x() - boundingBox[2].get_x());
	double distanceY = abs(boundingBox[0].get_y() - boundingBox[1].get_y());

	if (distanceX > distanceY) return (496 / distanceX);
	//else if (distanceX <= distanceY) return (498 / distanceY);
	else return (496 / distanceY);
}

/* Draws a 2px circle,
Used for drawing polygon's vertexs*/
void drawPoint(Point A, pngwriter& image, vector<double>color){
	image.filledcircle(A.get_x(), A.get_y(), 3, color[0], color[1], color[2]);
}

/* Draws a scaled and centred polygon*/
void drawPolygon(ConvexPoligon N, vector <Point> boundingBox, pngwriter& image){
	Point LL = boundingBox[3];
	double sc = scaling(boundingBox);
	vector<Point> AUX;
	for(Point& a : N.getPoints())	AUX.push_back(Point({(a.get_x() - LL.get_x()) * sc, (a.get_y() - LL.get_x()) * sc})+ Point({3,3}));

	int polygonSize = AUX.size();
	for(int i = 1; i < polygonSize; ++i){
		Point A = AUX[i-1];
		Point B = AUX[i];
		image.line(B.get_x(), B.get_y(), A.get_x(), A.get_y(), N.getColor()[0], N.getColor()[1], N.getColor()[2]);
	}
	image.line(AUX[polygonSize-1].get_x(), AUX[polygonSize-1].get_y(), AUX[0].get_x(), AUX[0].get_y(), N.getColor()[0], N.getColor()[1], N.getColor()[2]);
	for(Point a : AUX) drawPoint(a, image, N.getColor());
}

/* Draws the specified polygons in a png*/
void draw(map <string, ConvexPoligon> poligons){
	string line;
	getline(cin, line);
	istringstream iss (line);
	string name, filename;

	iss >> filename;
	pngwriter image(500, 500, 1.0, filename.c_str());

	vector<ConvexPoligon> elements;
	vector<Point> puntsBox;
	while(iss >> name){
		auto it = poligons.find(name);	elements.push_back(it->second);
		for(Point a : it->second.getPoints()) puntsBox.push_back(a);
	}
	puntsBox = boundingBox(puntsBox);
	for(ConvexPoligon A : elements) if(not A.isEmpty()){
		drawPolygon(A, puntsBox, image);
	}
	image.close();
	//no answer
	cout << "ok" << endl;
}

/* Returns the color of a specified polygon*/
void checkColor(map <string, ConvexPoligon>& poligons){
	string name;
	cin >> name;
	auto it = poligons.find(name);	ConvexPoligon p = it->second;
	vector <double> AUX = p.getColor();
	for(double a : AUX) cout << " " << a;
	cout << endl;
}

/* Calculates the intersection of two polygons*/
void intersection(map <string, ConvexPoligon>& poligons){
	string line, name;
	getline(cin, line);
	istringstream iss (line);
	vector <string> identifiers;
	while(iss >> name) identifiers.push_back(name);

	bool error = false;
	vector <Point> resultat;
	auto it = poligons.find(identifiers[0]);	ConvexPoligon p1 = it->second;
	if(it == poligons.end()) error = true;
	auto it2 = poligons.find(identifiers[1]);	ConvexPoligon p2 = it2->second;
	if(it == poligons.end()) error = true;

	if(identifiers.size() == 3){
		//three elements
		auto it3 = poligons.find(identifiers[2]);	ConvexPoligon p3 = it3->second;
		if(it3 == poligons.end()) error = true;

		if(not error){
			resultat = convexHull(unionVectors(p3.puntsDeTall(p2), pointsInside(p2,p3)));
			ConvexPoligon AUX(resultat);
			auto itn = poligons.find(identifiers[0]);
			itn->second = AUX;
			//no answer
			cout << "ok" << endl;
		}
		else cerr << errorReturn[0] << endl;
	}
	else if(identifiers.size() == 2){
		//two elements
		if(not error){
			resultat = convexHull(unionVectors(p1.puntsDeTall(p2), pointsInside(p1,p2)));
			ConvexPoligon AUX(resultat);
			auto itn = poligons.find(identifiers[0]);
			itn->second = AUX;
			//no answer
			cout << "ok" << endl;
		}
		else cerr << errorReturn[0] << endl;
	}
}

/* Calculates the convexx union of two polygons*/
void unio (map <string, ConvexPoligon>& poligons){
	string line;
	getline(cin, line);
	istringstream iss (line);
	string name;
	vector <string> identifiers;
	while(iss >> name) identifiers.push_back(name);

	if(identifiers.size() == 3){
		//three elements
		vector<Point> resposta = convexHull(unionVectors(points(poligons, identifiers[1]), points(poligons, identifiers[2])));
		ConvexPoligon AUX (resposta);
		auto it = poligons.find(identifiers[0]);
		it->second = AUX;
	}
	else if(identifiers.size() == 2){
		//two elements
		vector<Point> resposta = convexHull(unionVectors(points(poligons, identifiers[0]), points(poligons, identifiers[1])));
		ConvexPoligon AUX (resposta);
		auto it = poligons.find(identifiers[0]);
		it->second = AUX;
	}
	//no answer
	cout << "ok" << endl;
}

/* Checks if the second polygon is inside the first*/
void inside(const map <string, ConvexPoligon>& poligons){
	string name1, name2;
	cin >> name1 >> name2;

	bool error = false;
	auto it = poligons.find(name1);	ConvexPoligon p1 = it->second;
	auto it2 = poligons.find(name2);	ConvexPoligon p2 = it2->second;
	if(it == poligons.end() or it2 == poligons.end()) error = true;

	if(not error){
		bool checkInside = true;
		for(Point a : p2.getPoints()){
			if(not p1.isPointInside(a)) checkInside = false;
		}
		if(checkInside) cout << "yes" << endl;
		else cout << "no" << endl;
	}
	else cerr << errorReturn[0] << endl;
}

/* Creates a polygon bbox with the bounding box coordenates of
the specified polygons*/
void bbox(map <string, ConvexPoligon>& poligons){
	string line;
	getline(cin, line);
	istringstream iss (line);

	bool error = false;
	string polygon;
	vector <Point> pointsBox;
	while(iss >> polygon){
		auto it = poligons.find(polygon);	ConvexPoligon AUX = it->second;
		if(it != poligons.end()) pointsBox = unionVectors(pointsBox, AUX.getPoints());
		else error = true;
	}
	if(not error){
		ConvexPoligon box (boundingBox(pointsBox));
		poligons.insert({"bbox", box});
		//no answer
		cout << "ok" << endl;
	}
	else cerr << errorReturn[0] << endl;
}

int main(){
  cout.setf(ios::fixed);
  cout.precision(3);

  map <string, ConvexPoligon> poligons;

	string command;
  while(cin >> command){

    		 if(command == "polygon")    createPolygon(poligons);
    else if(command == "print")      print(poligons);
    else if(command == "area")       area(poligons);
    else if(command == "perimeter")  perimeter(poligons);
    else if(command == "vertices")   vertices(poligons);
    else if(command == "centroid")   centroid(poligons);
    else if(command == "save")       save(poligons);
    else if(command == "load")       load(poligons);
    else if(command == "list")       list(poligons);
    else if(command == "setcol")     setcol(poligons);
    else if(command == "draw")       draw(poligons);
    else if(command == "intersection")intersection(poligons);
    else if(command == "union")      unio(poligons);
    else if(command == "inside")     inside(poligons);
    else if(command == "bbox")       bbox(poligons);
		else if(command == "checkColor")      checkColor(poligons);
		else if(command == "#"){
			string comment;
			getline(cin, comment);
			cout << "#" << endl;
		}
		else{
			string comment;
			getline(cin, comment);
			cerr << errorReturn[1] << endl;
		}
  }
}
