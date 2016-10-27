//
//  main.cpp
//  TreeComp
//
//  Created by Victor Drobny on 18.10.16.
//  Copyright © 2016 Victor Drobny. All rights reserved.
//

#include <iostream>

#include "spatial_tree.hpp"
#include "octree.hpp"
#include "kd_tree.h"


void test_trees();
bool test_kd_tree_insert();
bool test_kd();

int main(int argc, const char * argv[]) {



    kd_tree_node<double> kd(bound(0, 0, 0, 1, 1, 1), 3, 3);

    kd.put(point(0.1, 0.2, 0.3), 0.001);
    kd.put(point(0.2, 0.2, 0.5), 0.543);
    kd.put(point(0.3, 0.1, 0.12), 0.923);
    kd.put(point(0.4, 0.3, 0.45), 0.823);
    kd.put(point(0.5, 0.4, 0.87), 0.111);
    kd.put(point(0.6, 0.2, 0.123), 0.112);
    kd.put(point(0.7, 0.5, 0.11), 0.113);
    kd.put(point(0.8, 0.2, 0.661), 0.114);
    kd.put(point(0.5, 0.4, 0.871), 0.111);
    kd.put(point(0.6, 0.2, 0.1231), 0.112);
    kd.put(point(0.7, 0.5, 0.111), 0.113);
    kd.put(point(0.8, 0.2, 0.661), 0.114);

    test_kd();
    //test_kd_tree_insert();
    //test_trees();
    //test_octree_insert();

    //test_kd_tree_insert();
    
    return 0;
}
