//
//  tests.cpp
//  TreeComp
//
//  Created by Victor Drobny on 19.10.16.
//  Copyright © 2016 Victor Drobny. All rights reserved.
//

#include <stdio.h>
#include "octree.hpp"
#include <vector>
#include <cstdlib>

using std::vector;

bool test_octree_insert()
{
    int * objects;
    point * possitions;
    
    int size = 20000000;
    
    objects = new int[size];
    possitions = new point[size];
    
    octree_node<int> octree(bound(0, 0, 0, 1, 1, 1), 10, 100);
    
    for (int i = 0; i < size; i++)
    {
        int o = rand();
        point p = point((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        octree.put(p, o);
        objects[i] = o;
        possitions[i] = p;
    }
    
    if (octree.size() != size)
        printf("Size");
    
    for (int i = 0; i < size; i++)
    {
        int tree_int = octree.at(possitions[i]);
        int obj_int = objects[i];
        if (tree_int != obj_int)
        {
            printf("Error");
            return false;
        }
    }
    
    return true;
}
