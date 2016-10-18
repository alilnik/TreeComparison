//
//  utils.hpp
//  TreeComp
//
//  Created by Victor Drobny on 18.10.16.
//  Copyright © 2016 Victor Drobny. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <cmath>

class point {
private:
    double const eps = 10e-5;
    
public:
    double x, y, z;
    
    point(): x(0), y(0), z(0) {}
    point(double x, double y, double z): x(x), y(y), z(z) {}

    
    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    
    void normalize()
    {
        double len = length();
        x /= len;
        y /= len;
        z /= len;
    }
    
    double operator* (point p)
    {
        return x * p.x + y * p.y + z * p.y;
    }
    
    point operator* (double a)
    {
        return point(x * a, y * a, z * a);
    }
    
    point operator+ (point b)
    {
        return point(x + b.x, y + b.y, z + b.x);
    }
    
    void operator=(point b)
    {
        x = b.x;
        y = b.y;
        z = b.z;
    }
    
    bool equals(point b)
    {
        return fabs(x - b.x) < eps && fabs(y - b.y) < eps && fabs(z - b.z) < eps;
    }
};

struct bound
{
    // Bounds
    // far left bottom, near right top
    point flb, nrt;
    
    bound()
    {
        flb = point(0, 0, 0);
        nrt = point(1, 1, 1);
    }
    
    bound(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        flb = point(x1, y1, z1);
        nrt = point(x2, y2, z2);
    }
    
    bound(point p1, point p2) : flb(p1), nrt(p2) {}
    
    point center()
    {
        return (flb + nrt) * 0.5;
    }
    
    double near()
    {
        return nrt.x;
    }
    
    double far()
    {
        return flb.x;
    }
    
    double left()
    {
        return flb.y;
    }
    
    double right()
    {
        return nrt.y;
    }
    
    double top()
    {
        return nrt.z;
    }
    
    double bottom()
    {
        return flb.z;
    }
};



#endif /* utils_hpp */
