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
#include <utility>

using std::pair;
using std::make_pair;

class point {
private:
    double const eps = 10e-15;
    
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
    
    double distance(point p)
    {
        return (p + *this * (-1)).length();
    }
    
    double sqdist(point p)
    {
        double dx = x - p.x;
        double dy = y - p.y;
        double dz = z - p.z;
        
        return dx * dx + dy * dy + dz * dz;
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
    
    double sqdist(pair<double, double> p1, pair<double, double> p2)
    {
        double dx = p1.first - p2.first;
        double dy = p1.second - p2.second;
        return dx * dx + dy * dy;
    }
    
    double distance(bound b)
    {
        bool is_overlap_x;
        bool is_overlap_y;
        bool is_overlap_z;
        
        double bx[2][2] = {{far(), top()}, {b.far(), b.top()}};
        double by[2][2] = {{left(), right()}, {b.left(), b.right()}};
        double bz[2][2] = {{bottom(), top()}, {b.bottom(), b.top()}};
        
        
        is_overlap_x = (bx[0][0] < bx[1][1] && bx[0][0] > bx[1][0]) || (bx[1][0] < bx[0][1] && bx[1][0] > bx[0][0]);
        is_overlap_y = (by[0][0] < by[1][1] && by[0][0] > by[1][0]) || (by[1][0] < by[0][1] && by[1][0] > by[0][0]);
        is_overlap_z = (bz[0][0] < bz[1][1] && bz[0][0] > bz[1][0]) || (bz[1][0] < bz[0][1] && bz[1][0] > bz[0][0]);
        
        if (is_overlap_x && is_overlap_y && is_overlap_z)
            return 0;
        if (!is_overlap_x && is_overlap_y && is_overlap_z)
        {
            double dist = fmin(fabs(bx[0][0] - bx[1][1]), fabs(bx[0][1] - bx[1][0]));
            return dist * dist;
        }
        if (is_overlap_x && !is_overlap_y && is_overlap_z)
        {
            double dist = fmin(fabs(by[0][0] - by[1][1]), fabs(by[0][1] - by[1][0]));
            return dist * dist;
        }
        if (is_overlap_x && is_overlap_y && !is_overlap_z)
        {
            double dist = fmin(fabs(bz[0][0] - bz[1][1]), fabs(bz[0][1] - bz[1][0]));
            return dist * dist;
        }
        if (!is_overlap_x && !is_overlap_y && is_overlap_z)
        {
            pair<double, double> b[2][4];
            
            for (int k = 0; k < 2; k++)
                for (int i = 0; i < 2; i++)
                    for (int j = 0; j < 2; j++)
                        b[k][2 * i + j] = make_pair(bx[k][i], by[k][j]);
            
            double min_dist = sqdist(b[0][0], b[1][0]);
            double tmp;
            
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (min_dist > (tmp = sqdist(b[0][i], b[1][j])))
                        min_dist = tmp;
            return min_dist;
        }
        if (!is_overlap_x && is_overlap_y && !is_overlap_z)
        {
            pair<double, double> b[2][4];
            
            for (int k = 0; k < 2; k++)
                for (int i = 0; i < 2; i++)
                    for (int j = 0; j < 2; j++)
                        b[k][2 * i + j] = make_pair(bx[k][i], bz[k][j]);
            
            double min_dist = sqdist(b[0][0], b[1][0]);
            double tmp;
            
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (min_dist > (tmp = sqdist(b[0][i], b[1][j])))
                        min_dist = tmp;
            return min_dist;
        }
        if (is_overlap_x && !is_overlap_y && !is_overlap_z)
        {
            pair<double, double> b[2][4];
            
            for (int k = 0; k < 2; k++)
                for (int i = 0; i < 2; i++)
                    for (int j = 0; j < 2; j++)
                        b[k][2 * i + j] = make_pair(bz[k][i], by[k][j]);
            
            double min_dist = sqdist(b[0][0], b[1][0]);
            double tmp;
            
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (min_dist > (tmp = sqdist(b[0][i], b[1][j])))
                        min_dist = tmp;
            return min_dist;
        }
        if (!is_overlap_x && !is_overlap_y && !is_overlap_z)
        {
            point b[2][8];
            
            for (int k = 0; k < 2; k++)
            {
                for (int i = 0; i < 2; i++)
                    for (int j = 0; j < 2; j++)
                        for (int l = 0; l < 2; l++)
                            b[k][4 * i + 2 * j + l] = point(bx[k][i], by[k][j], bz[k][l]);
            }
            
            double min_dist = b[0][0].distance(b[1][0]);
            double tmp;
            
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    if (min_dist > (tmp = b[0][i].sqdist(b[1][j])))
                        min_dist = tmp;
            return min_dist;
        }
        return 0;
    }
    
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
