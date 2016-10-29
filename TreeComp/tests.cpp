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
#include "r_tree.h"
#include "kd_tree.h"

using std::vector;

vector<pair<point, point> > * manual_search(point * pos, int size, double distance)
{
    vector<pair<point, point> > * result = new vector<pair<point, point> >();
    
    double sd = distance * distance;
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (pos[i].distance(pos[j]) <= distance)
                result->push_back(make_pair(pos[i], pos[j]));
    
    return result;
}

template <typename T>
bool test_tree_search(vector<pair<pair<point, T>, pair<point, T> > > * result, double distance)
{
    double sd = distance * distance;
    
    int incorrect = 0;
    for (int i = 0; i < result->size(); i++)
    {
        point p1 = result->at(i).first.first;
        point p2 = result->at(i).second.first;
        if (p1.distance(p2) > distance)
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
        if (p1.distance(p2) > distance)
            incorrect++;
    }
    return incorrect == 0;
}



bool test_tree(spatial_tree_node<int> * tree, char * tree_name)
{
    int * objects;
    point * possitions;
    
    int size = 2500;
    double dist = 0.01;
    clock_t time;
    
    printf("%s test: \n", tree_name);
    
    objects = new int[size];
    possitions = new point[size];
    
    int * ints;
    
    ints = new int[size];
    
    srand(19024);
    
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
        int o = i;
        point p = point((double) ints[i] / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
        tree->put(p, o);
        objects[i] = o;
        possitions[i] = p;
    }
    
    double insert_time = clock() - time;
    printf("Inserting %i elements: %lf msec\n", size, insert_time / CLOCKS_PER_SEC * 1000);
    

    if (dynamic_cast<const r_tree_node<int>*>(tree) != 0)
    {
        printf("Yes. Size %i\n", ((r_tree_node<int> *)tree)->get_size());
        r_tree_node<int> * r_tree = dynamic_cast<r_tree_node<int>*>(tree);
        int misses = 0;
        time = clock();
        for (int i = 0; i < size; i++)
        {
            int * search = r_tree->at(possitions[i]);
            if (search == nullptr)
            {
                printf(" %i,", i);
                misses++;
                continue;
            }
            int tree_int = *search;
            int obj_int = objects[i];
            if (tree_int != obj_int)
                misses++;
        }
        double search_time = clock() - time;
        printf("Searching %i elements with %i misses: %lf msec\n", size, misses, search_time / CLOCKS_PER_SEC * 1000);
    }
     

    time = clock();
    auto res = tree->get_neighbors(dist);
    double neighbor_search_time = clock() - time;
    printf("Searching neighbors %i within %lf: %lf msec\n", size, dist, neighbor_search_time / CLOCKS_PER_SEC * 1000);
    
    time = clock();
    auto res2 = manual_search(possitions, size, dist);
    double manual_search_time = clock() - time;
    printf("Bruteforce searching neighbors %i within %lf: %lf msec\n", size, dist, manual_search_time / CLOCKS_PER_SEC * 1000);
    
    printf("Found %i from %i pairs.\n", res->size(), res2->size());
    
    if (res->size() != res2->size())
        printf("Different sizes! %s: %lu, brutforce: %lu\n", tree_name, res->size(), res2->size());
    
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
    delete res2;
    
    printf("=================\n");
    
    return true;
}


bool test_octree()
{
    
    octree_node<int> octree(bound(0, 0, 0, 1, 1, 1), 100, 100);
    
    return test_tree(&octree, "Octree");
}

bool test_kd()
{

    kd_tree_node<int> kd(bound(0, 0, 0, 1, 1, 1), 100, 100);

    return test_tree(&kd, "KD-tree");
}


bool test_kd_tree_insert(){

    pair<point, int> * objects;
    int size = 25000;
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

    return true;

}


void compare_trees() {
    int *objects;
    point *possitions;
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    double distances[] = {10e-4, 10e-6};
    int number_of_tries = 5;

    clock_t time;

    // Insert name here!
    char *names[] = {"Octree(10, 100)   ", "Octree(100, 100)  ", "Octree(1000, 100) ", "Octree(10, 1000)  ",
                     "Octree(100, 1000) ", "Octree(1000, 1000)", "Kd-tree(10, 100)  ", "Kd-tree(100, 100) ",
                     "Kd-tree(1000, 100)", "Kd-tree(10, 1000)  ", "Kd-tree(100, 1000) ", "Kd-tree(1000, 1000)",
                     "Kd-tree with single axis split(10, 100)  ", "Kd-tree with single axis split(100, 100) ",
                     "Kd-tree with single axis split(1000, 100)", "Kd-tree with single axis split(10, 1000)  ",
                     "Kd-tree with single axis split(100, 1000) ", "Kd-tree with single axis split(1000, 1000)",
                     "Kd-tree with center split(10, 100)  ", "Kd-tree with center split(100, 100) ",
                     "Kd-tree with center split(1000, 100)", "Kd-tree with center split(10, 1000)  ",
                     "Kd-tree with center split(100, 1000) ", "Kd-tree with center split(1000, 1000)",
                     "Kd-tree with SAH split(10, 100)  ", "Kd-tree with SAH split(100, 100) ",
                     "Kd-tree with SAH split(1000, 100)", "Kd-tree with SAH split(10, 1000)  ",
                     "Kd-tree with SAH split(100, 1000) ", "Kd-tree with SAH split(1000, 1000)"
            //                , "R-tree(5)         ", "R-tree(25)        ", "R-tree(125)       "
    };
    int size = sizeof(names) / sizeof(names[0]);
    int dist_x_size = size * (sizeof(distances) / sizeof(double));
    double *insertion_time = new double[size];
    double searching_time[1000] = {0.0};


    for (int q = 0; q < sizeof(sizes) / sizeof(sizes[0]); q++) {
        for (int k = 0; k < number_of_tries; k++) {
            objects = new int[sizes[q]];
            possitions = new point[sizes[q]];

            // Generating data
            double *rand_double = new double[sizes[q]];
            double gap = (RAND_MAX - 1) * 1.0 / sizes[q];
            double current = 0;

            for (int i = 0; i < sizes[q]; i++) {
                rand_double[i] = current;
                current += gap;
            }

            std::random_shuffle(rand_double, rand_double + sizes[q]);


            // Insert your tree here
            spatial_tree_node<int> *tree[] = {
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 100),
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 100),
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 100),
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 1000),
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 1000),
                    new octree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 1000),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 100, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 100, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 100, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 1000, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 1000, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 1000, 0),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 100, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 100, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 100, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 1000, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 1000, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 1000, 1),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 100, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 100, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 100, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 1000, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 1000, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 1000, 2),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 100, 3),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 100, 3),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 100, 3),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 10, 1000, 3),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 100, 1000, 3),
                    new kd_tree_node<int>(bound(0, 0, 0, 1, 1, 1), 1000, 1000, 3)

                    //new r_tree<int>(5),
                    //new r_tree<int>(25),
                    //new r_tree<int>(125)
            };


            for (int i = 0; i < sizes[q]; i++) {
                int o = i;
                point p = point(rand_double[i] / RAND_MAX, (double) rand() / RAND_MAX, (double) rand() / RAND_MAX);
                objects[i] = o;
                possitions[i] = p;
            }

            for (int i = 0; i < size; i++) {
                time = clock();
                for (int j = 0; j < sizes[q]; j++) {
                    int o = objects[j];
                    point p = possitions[j];
                    tree[i]->put(p, o);
                }

                double insert_time = clock() - time;
                insertion_time[i] += insert_time / CLOCKS_PER_SEC;

            }


            for (int i = 0; i < size; i++) {

                for (int j = 0; j < sizeof(distances) / sizeof(double); j++) {
                    time = clock();
                    spatial_tree_node<int> *t = tree[i];
                    auto result = t->get_neighbors(distances[j]);
                    double search_time = clock() - time;
                    searching_time[size * i + j] += search_time / CLOCKS_PER_SEC;
                    delete result;
                }
            }

            for (int i = 0; i < size; i++) {
                spatial_tree_node<int> *t = tree[i];
                delete t;
            }
            delete[] rand_double;
            delete[] objects;
            delete[] possitions;

            printf("\n");

            for (int i = 0; i < size; i++) {
                printf("Test of %s on %10i elements\n", names[i], sizes[q]);
                printf("Inserting in %lf msec\n", insertion_time[i] * 1000 / number_of_tries);
                printf("Searching within the distance:\n");
                for (int j = 0; j < sizeof(distances) / sizeof(double); j++)
                    printf("  %lf in %lf msec\n", distances[j], searching_time[size * i + j] * 1000 / number_of_tries);
                printf("====================\n");
            }

        }


        delete[] insertion_time;
        //delete[] searching_time;
    }
}


bool test_r_tree()
{

    r_tree_node<int> r_tree(bound(0, 0, 0, 1, 1, 1), 5, nullptr);

    return test_tree(&r_tree, "R tree");
}


void test_trees()
{
    //test_octree();
    //test_r_tree();

    compare_trees();
}
