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

using std::vector;
using std::pair;
using std::make_pair;


vector<pair<point, point> > * manual_search(point * pos, int size, double distance)
{
    vector<pair<point, point> > * result = new vector<pair<point, point> >();
    
    double sd = distance * distance;
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (pos[i].sqdist(pos[j]) <= sd)
                result->push_back(make_pair(pos[i], pos[j]));
    
    return result;
}

template <typename T>
bool test_tree_search(vector<pair<pair<point, T>, pair<point, T> > > result, double distance)
{
    double sd = distance * distance;
    
    int incorrect = 0;
    for (int i = 0; i < result.size(); i++)
    {
        point p1 = result.at(i).first.first;
        point p2 = result.at(i).second.first;
        if (p1.sqdist(p2) > sd)
            incorrect++;
    }
    return incorrect == 0;
}

bool test_manual_search(vector<pair<point, point> > result, double distance)
{
    double sd = distance * distance;
    
    int incorrect = 0;
    for (int i = 0; i < result.size(); i++)
    {
        point p1 = result.at(i).first;
        point p2 = result.at(i).second;
        if (p1.sqdist(p2) > sd)
            incorrect++;
    }
    return incorrect == 0;
}



bool test_tree(spatial_tree_node<int> * tree, char * tree_name)
{
    int * objects;
    point * possitions;
    
    int size = 100000;
    
    clock_t time;
    
    printf("%s test: \n", tree_name);
    
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
    
    
    
    
    time = clock();
    for (int i = 0; i < size; i++)
    {
        int o = rand();
        point p = point((double) ints[i] / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        tree->put(p, o);
        objects[i] = o;
        possitions[i] = p;
    }
    
    double insert_time = clock() - time;
    printf("Inserting %i elements: %lf msec\n", size, insert_time / CLOCKS_PER_SEC * 1000);
    
    
    /*
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
    */
    double dist = 0.0001;
    
    time = clock();
    auto res = tree->get_neighbors(dist);
    double neighbor_search_time = clock() - time;
    printf("Searching neighbors %i within %lf elements: %lf msec\n", size, dist, neighbor_search_time / CLOCKS_PER_SEC * 1000);
    
    time = clock();
    auto res2 = manual_search(possitions, size, dist);
    double manual_search_time = clock() - time;
    printf("Bruteforce searching neighbors %i within %lf elements: %lf msec\n", size, dist, manual_search_time / CLOCKS_PER_SEC * 1000);
    
    if (res.size() != res2->size())
        printf("Different sizes! octree: %lu, brutforce: %lu\n", res.size(), res2->size());
    
    if (!test_tree_search(res, dist))
    {
        printf("Searching incorrect!\n");
    }
    
    if (!test_manual_search(*res2, dist))
    {
        printf("Manual incorrect!\n");
    }
    
    delete[] ints;
    delete[] objects;
    delete[] possitions;
    
    printf("=================");
    
    return true;
}

bool test_octree_insert()
{
    
    octree_node<int> octree(bound(0, 0, 0, 1, 1, 1), 100, 100);
    
    return test_tree(&octree, "Octree");
}
