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

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    spatial_tree_node<double> * tree;
    tree = new octree_node<double>(bound(0, 0, 0, 1, 1, 1), 10, 2);
    
    tree->put(point(0.1, 0.2, 0.3), 0.123);
    tree->put(point(0.9, 0.2, 0.3), 0.923);
    tree->put(point(0.8, 0.2, 0.3), 0.823);
    
    return 0;
}
