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

int main(int argc, const char * argv[]) {



    kd_tree_node<double> kd(bound(0, 0, 0, 1, 1, 1), 3, 3);

    kd.put(point(0.1, 0.2, 0.3), 0.123);
    kd.put(point(0.5, 0.2, 0.3), 0.543);
    kd.put(point(0.9, 0.2, 0.3), 0.923);
    kd.put(point(0.8, 0.2, 0.3), 0.823);
    
    test_trees();
    //test_octree_insert();

    //test_kd_tree_insert();
    
    return 0;
}
