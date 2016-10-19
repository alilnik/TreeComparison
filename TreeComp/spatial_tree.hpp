//
//  spatial_tree.hpp
//  TreeComp
//
//  Created by Victor Drobny on 18.10.16.
//  Copyright © 2016 Victor Drobny. All rights reserved.
//

#ifndef spatial_tree_hpp
#define spatial_tree_hpp

#include <vector>
#include <utility>
#include "utils.hpp"
#include <stdio.h>

using std::vector;
using std::pair;

template <typename T>
class spatial_tree_node
{
protected:
    bound bnd;
private:
    virtual spatial_tree_node<T> ** get_children() = 0;
    virtual vector<T> * get_objects() = 0;
    
public:
    
    spatial_tree_node() {}
    virtual ~spatial_tree_node() {}
    bound get_bound();
    virtual bool is_leaf() = 0;
    virtual void put(point p, T obj) = 0;
    
    
    vector<pair<T *, T *> > get_neighbors();
};

template <typename T>
bound spatial_tree_node<T>::get_bound()
{
    return bnd;
}

template <typename T>
vector<pair<T *, T *> > spatial_tree_node<T>::get_neighbors()
{
    return vector<pair<T *, T *> >();
}

#endif /* spatial_tree_hpp */
