#ifndef CENTROID_H
#define CENTROID_H


// Centroid in 3D space.
struct Centroid {
    double x; // x coordinate.
    double y; // y coordinate.


    /*
    Centroid constructor.
    
    Parameters:
      - x (double): x coordinate.
      - y (double): y coordinate.
    */
    Centroid(double x, double y);

    /*
    Overload of the << operator to print a point.
    */
    friend std::ostream& operator<<(std::ostream& os, const Centroid& p);
};

#endif // CENTROID_H