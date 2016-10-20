//
// Created by Ilya Dmitrenok on 19.10.16.
//

#ifndef kd_tree_
#define kd_tree_hpp

#include <vector>
#include <algorithm>

#include "spatial_tree.hpp"

const int N = 32;
const int ct = 1;
const int cl = 1;

template <typename T>
class kd_tree_node: public spatial_tree_node<T>
{

private:
    bool is_leaf_node;
    kd_tree_node<T>* children[2];
    vector<T>* objects;
    int max_amount_of_objects;
    vector<point> * positions;
    void split();
    int get_child_id(point p);
    virtual spatial_tree_node<T> ** get_children();
    virtual vector<T> * get_objects();

    /**
     * Added this functions and vars for kd tree purpose
     */
    double volume;
    int calculate_sah(int axis, int num);
    int max_depth;
    double calulate_volume();
    point get_median();



public:
    kd_tree_node();
    kd_tree_node(bound bnd, int max_depth, int max_amount_of_objects);
    ~kd_tree_node();
    virtual bool is_leaf();
    virtual void put(point p, T obj);
    T at(point p);
    int size();
};

template <typename T>
kd_tree_node<T>::kd_tree_node()
{
    kd_tree_node<T>(bound(0, 0, 0, 1, 1, 1), 10, 1000);
}

template <typename T>
kd_tree_node<T>::kd_tree_node(bound bnd, int max_depth, int max_amount_of_objects)
{
    this->bnd = bnd;
    this->is_leaf_node = true;
    this->max_depth = max_depth;
    this->max_amount_of_objects = max_amount_of_objects;
    for (int i = 0; i < 8; i++)
        children[i] = nullptr;
    objects = new vector<T>();
    positions = new vector<point>();
    is_leaf_node = true;
}

template <typename T>
kd_tree_node<T>::~kd_tree_node()
{
    for (int i = 0; i < 2; i++)
        if (children[i] != nullptr)
        {
            delete children[i];
            children[i] = nullptr;
        }

    if (objects != nullptr)
    {
        delete objects;
        objects = nullptr;
    }
    if (positions != nullptr)
    {
        delete positions;
        positions = nullptr;
    }
}

/**
 * REVIEW!
 * @param p
 * @param obj
 */
template <typename T>
void kd_tree_node<T>::put(point p, T obj)
{
    if (is_leaf_node)
    {
        // Insert
        if (objects->size() < max_amount_of_objects || (max_depth == 0))
        {
            objects->push_back(obj);
            positions->push_back(p);
        } else {
            // Split
            split();
            put(p, obj);
        }
    } else {
        int idx = get_child_id(p);
        children[idx]->put(p, obj);
    }

}

template <typename T>
void kd_tree_node<T>::split()
{
    if (!this->is_leaf_node)
        return;
    point median = get_median();

    point p[2] = {
        point(bnd.flb.x, bnd.flb.y, bnd.flb.z),
        point(median.x, bnd.flb.y, bnd.flb.z),
    };
    point e[2] = {
        point(median.x, bnd.flb.y, bnd.flb.z),
        point(bnd.nrt.x, bnd.nrt.y, bnd.nrt.z)

    };

    /**
     * REVIEW!!!
     */
    for (int i = 0; i < 2; i++)
    {
        children[i] = new kd_tree_node<T>(bound(p[i], e[i]), max_depth - 1, max_amount_of_objects);
    }

    for (int i = 0; i < objects->size(); i++)
    {
        int idx = get_child_id(positions->at(i));
        children[idx]->put(positions->at(i), objects->at(i));
    }
    delete objects;
    delete positions;
    objects = nullptr;
    positions = nullptr;
    this->is_leaf_node = false;
}

/**
 *
 * @param axis - on what axis are we separating: x=0,y=1,z=2
 * @param num - what parttion are we on (1;N)
 * @return sah rating
 */
template <typename T>
int kd_tree_node<T>::calculate_sah(int axis, int num)
{
    int sah = 0;
    int split_coordinate, volume_l, volume_r, count_l, cout_r = 0;

    volume_l = 1/N*volume;
    volume_r = volume - volume_r;

    if (axis == 0){
        split_coordinate = 1/N*(bnd.flb.x+bnd.nrt.x);
        for(auto &i: *positions){
            if(i.x>=split_coordinate) cout_r++;
            else count_l++;
        }
    }
    else if(axis == 1){
        split_coordinate = 1/N*(bnd.flb.y+bnd.nrt.y);
        for(auto &i: *positions){
            if(i.y>=split_coordinate) cout_r++;
            else count_l++;
        }
    }
    else if(axis == 2){
        split_coordinate = 1/N*(bnd.flb.z+bnd.nrt.z);
        for(auto &i: *positions){
            if(i.z>=split_coordinate) cout_r++;
            else count_l++;
        }
    }
    else return -1;

    sah = ct + cl*((volume_l*count_l+volume_r*cout_r)/volume);

    return sah;
}

/**
 * calculates volume of the box
 * @return volume
 */
template <typename T>
double kd_tree_node<T>::calulate_volume() {
    return (bnd.nrt.x-bnd.flb.x)*(bnd.nrt.y-bnd.flb.y)*(bnd.nrt.z-bnd.flb.z);
}


//TODO: change coordiates
/**
 * gets median of the box on X parameter. To be considered
 * @return
 */
point kd_tree_node::get_median() {
    vector<point> * sorted = positions;
    std::sort(sorted->begin(), sorted->end(),point_compare);
    return sorted->[sorted->size()/2];
}

//little comparator for points
bool point_compare (point x, point y) { return (x.x<y.x); }


#endif //SPATIAL_TREE_COMPARISON_KD_TREE_H
