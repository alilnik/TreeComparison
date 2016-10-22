//
//  tests.cpp
//  TreeComp
//
//  Created by Victor Drobny on 19.10.16.
//  Copyright © 2016 Victor Drobny. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "octree.hpp"
#include "kd_tree.h"

using std::vector;

bool test_octree_insert()
{
    int * objects;
    point * possitions;
    
    int size = 10000;
    
    clock_t time;
    
    printf("Octree test: \n");
    
    objects = new int[size];
    possitions = new point[size];
    
    int * ints;
    
    ints = new int[size];
    
    double gap = (RAND_MAX - 1) * 1.0 / size;
    double current = 0;
    
    for (int i = 0; i < size; i++)
    {
        ints[i] = (int) current;
        current += gap;
        
        if (ints[i] >= RAND_MAX)
            printf("Rand Err\n");
    }
    
    std::random_shuffle(ints, ints + size);
    
    octree_node<int> octree(bound(0, 0, 0, 1, 1, 1), 10, 100);
    
    
    time = clock();
    for (int i = 0; i < size; i++)
    {
        int o = rand();
        point p = point((double) ints[i] / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        octree.put(p, o);
        objects[i] = o;
        possitions[i] = p;
    }
    
    double insert_time = clock() - time;
    printf("Inserting %i elements: %lf msec\n", size, insert_time / CLOCKS_PER_SEC * 1000);
    
    if (octree.size() != size)
        printf("Size\n");
    
    
    time = clock();
    for (int i = 0; i < size; i++)
    {
        int tree_int = octree.at(possitions[i]);
        int obj_int = objects[i];
        if (tree_int != obj_int)
        {
            printf("Error\n");
            return false;
        }
    }
    double search_time = clock() - time;
    printf("Searching %i elements: %lf msec\n", size, search_time / CLOCKS_PER_SEC * 1000);
    
    time = clock();
    auto res = octree.get_neighbors(0.001);
    double neighbor_search_time = clock() - time;
    printf("Searching neighbors %i elements: %lf msec\n", size, neighbor_search_time / CLOCKS_PER_SEC * 1000);
    
    delete[] ints;
    delete[] objects;
    delete[] possitions;
    
    printf("=================");
    
    return true;
}

bool test_kd_tree_insert(){

    pair<point, int> * objects;
    int size = 10000;
    clock_t time;
    printf("KD TREE TEST: \n");
    objects = new pair<point, int>[size];
    int * ints;
    ints = new int[size];
    double gap = (RAND_MAX - 1) * 1.0 / size;
    double current = 0;

    for (int i = 0; i < size; i++)
    {
        ints[i] = (int) current;
        current += gap;

        if (ints[i] >= RAND_MAX)
            printf("Rand Err\n");
    }

    std::random_shuffle(ints, ints + size);
    kd_tree_node<int> kd(bound(0, 0, 0, 1, 1, 1), 10, 10);
    point* median = new point(0.5, 0, 0);
    kd.median = *median;

    time = clock();
    for (int i = 0; i < size; i++)
    {
        int o = rand();
        point p = point((double) ints[i] / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        kd.put(p, o);


    }

    double insert_time = clock() - time;
    printf("Inserting %i elements: %lf msec\n", size, insert_time / CLOCKS_PER_SEC * 1000);



}
