# Polygon-Calculator


Polygon Calculator is a project developed for the Algorithmic and Programming II class at UPC.
The objective of this program is to provide an intuitive and easy way of doing different calculus related to polygon geometry from the computer terminal. The entire of Polygon Calculator is written in C++.

## Table of contents
1. [Instalation](#instalation)
2. [General use](#use)
3. [Functions](#functions)
4. [Algorithms](#algorithms)
5. [Other information](#other)

## Instalation <a name="introduction"></a>

Polygon Calculator requires the library [pngwriter](https://github.com/pngwriter/pngwriter) to be installed. Pngwriter is a library for C++ that is used for the creation of images. It’s used here for the creation of polygons graphics.
Install the library:
```bash
git clone https://github.com/pngwriter/pngwriter.git
```
Compile:

```bash
cd pngwriter

cmake -DPNGwriter_USE_FREETYPE=OFF -DCMAKE_INSTALL_PREFIX=$HOME/libs .

make

make install 
```
When the library is installed, the code can be erased
``` bash
cd ..
rm -rf pngwriter
```
 
## General use <a name="use"></a>
Polygon Calculator has to be used from the computer terminal. For the commands first you have to access it's folder.
A mekefile is already created for an easy compilation.
Makefile commands are:

- Compile all program:
```bash 
make 
```
- Erase all executable files in the folder:
```bash 
make clean 
```
- Erase all .txt documents in the folder:
```bash 
make cleantxt 
```
- Erase all .png in the folder:
```bash 
make cleanpng 
```
- Execute:
```bash
&> ./output
```

All .png and .txt documents created with the calculator will be stored in the code folder. Also, the .txt files loaded must be in the same folder.

Polygon’s Points are given in pairs (first x-coordinate and second y-coordinate) and are real numbers, polygon colors have to be specified in RGB code (three real numbers from 0 to 1). All polygons created are black(0, 0, 0).

All polygons created must be named with a string.

Files name’s have to be introduced in string format and with the .txt extension.

The commands that don't need answer will output "ok" if the process is done correctly.


## Functions <a name="functions"></a>

### `polygon` 
``` bash
polygon p1 0 0 6 8 3 -2 
ok
```
Creates a new polygon from a set of points or overwrites it if already exists.
Requires a string (polygon name) and an undefined size set of points.

### `print` 
``` bash
print p1 
0.000 0.000 6.000 8.000 3.000 -2.000
```
Prints the specified polygon name and set of points.

### `area` 
``` bash
area p1
18.000
```
Prints the area of the specified polygon.

### `perimeter` 
``` bash
perimeter p1
24.046
```
Prints the perimeter of the specified polygon.	

### `vertices` 
``` bash
veritces p1
3
```
Prints the number of vertex of the specified polygon.

### `centroid` 
``` bash
centroid p1
3.000 2.000
```
Prints the centroid coordinates of the specified polygon.
The centroid is the center of mass of a polygon, the average position of all its points.

### `list`
``` bash
list
p1 p2 p3
```
Prints the names of all the polygons created.

### `save`
``` bash
save s.txt p1 p2 p3
ok
```
Creates a file to save the specified polygon information. If the file already exists overwrites it.
The polygon information is saved in the same format as it shows on terminal.
 
### `load` 
``` bash
load s.txt
ok
```
Takes the polygon information from a .txt file and loads it to the terminal.

### `draw` 
``` bash
draw p.png p1 p2 p3
ok
```
Creates a .png image with a representation of the specified polygons. The polygons are scaled to fit and be centred.
Requires a string(name.png) and a list of polygons. The polygons are draw from the first called to the last, it's important to know because the colors can overlay.

### `intersection`
``` bash
intersection p1 p2 p3
ok
intersection p1 p2
ok
```
Creates a polygon from the intersection of two specified polygons. It can accept two or three parameters
When reciving two paraters(p1 and p2), p1 is updated to p1 and p2 intersection.
If three parameters are recived(p1, p2 and p3) p1 is updated to p2 and p3 intersection. 

### `union` 
``` bash
union p1 p2 p3
ok
union p1 p2
ok
```
Creates a polygon from the convex union of two polygons. It can accept two or three parameters
When reciving two paraters(p1 and p2), p1 is updated to p1 and p2 intersection.
If three parameters are recived(p1, p2 and p3) p1 is updated to p2 and p3 intersection. 

### `bbox` 
``` bash
bbox p1 p2 p3
ok
```
Creates a bounding box for a set of polygons.
The bounding box is the minimun area rectangle that contains all the specified points.

### `inside` 
``` bash
inside p1 p2
ok
```
Given two polygons, the inside command prints yes or not to tell whether the first is inside the second or not.

### `checkColor` 
``` bash
checkColor p1
0.000 0.000 0.000
```
Returns the color of a polygon.

## Algorithms <a name="algorithms"></a>

The whole program is based on the implementation of the classes **Point** and **ConvexPolygon**.
The class **Point** is defined as two doubles x and y (the x-coordinate and the y-coordinate).
The class **ConvexPoligon** consist of a _v vector of points (vertex of the polygon) and a three element _RGB double vector(with numbers between 0 and 1). The constructor for ConvexPolygon requires the _v vector, that has to be already a set of points in clockwise order, starting from the vertex will lower x-coordinate.

When a new polygon is created, a convex polygon is created although the set of points introduced can have more points than necessary and it doesn’t have to be ordered. The creation of a convex polygon from any set of points is done with the convexPolygon function (a convex hull is the smallest convex polygon that encloses all points of a set).

The algorithm used for the convexHull function is the **Andrew's monotone chain** algorithm.  This algorithm creates a convex hull in complexity O(nlogn). First, it sorts the points from smallest to biggest x-coordinate, and then constructs the upper hull and a lower hull for separated using the cross product of the segments. 

The intersection function works with a **Line** struct (composed of the double slope and the double y-intercept) for finding the cutting point for every pair of segments and a function that tells if a point is inside a polygon for knowing which points intersect. Finally a convex hull is created from all the found points. The cost is O(n²).

## Other information <a name="other"></a>
Comments can be written with a # when starting the line.
```
# This is a comment
#
```
The calculator has the ability to detect some errors in input format:
``` bash
error: undefined polygon identifier
error: invalid command
error: wrong color format
```
All doubles are printed with a precision of 3 decimals.
