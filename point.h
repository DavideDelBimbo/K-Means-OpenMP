#ifndef POINT_H
#define POINT_H

// Point in 3D space.
struct Point {
    double x; // x coordinate.
    double y; // y coordinate.
    int cluster; // Cluster to which the point belongs.
    

    /*
    Point constructor.
    
    Parameters:
      - x (double): x coordinate.
      - y (double): y coordinate.
    */
    Point(double x, double y);

    /*
    Overload of the << operator to print a point.
    */
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

#endif // POINT_H