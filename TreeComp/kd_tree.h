//
// Created by Ilya Dmitrenok on 19.10.16.
//

#ifndef kd_tree_hpp
#define kd_tree_hpp

#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric> 
#include "spatial_tree.hpp"

const int N = 32;
const int ct = 1;
const int ci = 1;

template <typename T>
class kd_tree_node: public spatial_tree_node<T>
{

private:
    bool is_leaf_node;
    kd_tree_node<T>* children[2];
    vector<pair<point, T> > * objects;
    int max_amount_of_objects;
    //vector<point> * positions;
    void split();
    int get_child_id(point p);
    virtual int get_children_size();
    virtual spatial_tree_node<T> ** get_children();
    virtual vector<pair<point, T> > * get_objects();
    /**
     * Added this functions and vars for kd tree purpose
     */
    double surface;
    int max_depth;
    point get_median();



public:
    kd_tree_node();
    kd_tree_node(bound bnd, int max_depth, int max_amount_of_objects);
    ~kd_tree_node();
    virtual bool is_leaf();
    virtual void put(point p, T obj);
    T at(point p);
    int size();
    point median;

    //little comparator for points

    static bool point_compare (const pair<point, T> x, const pair<point, T> y) { return (x.first.x<y.first.x); }

    void split_sah();

    double k_part_surface(int k, int axis);

    point split_with_sah();
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
    for (int i = 0; i < 2; i++)
        children[i] = nullptr;
    objects = new vector<pair<point, T> >();
    is_leaf_node = true;
    this->surface=bnd.half_surface_area();
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
            objects->push_back(make_pair(p, obj));

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
    bound bnd = this->bnd;
    if (!this->is_leaf_node)
        return;
    point median = split_with_sah();
    point p[2] = {
        point(bnd.flb.x, bnd.flb.y, bnd.flb.z),
        point(median.x, bnd.flb.y, bnd.flb.z),
    };
    point e[2] = {
        point(median.x, bnd.flb.y, bnd.flb.z),
        point(bnd.nrt.x, bnd.nrt.y, bnd.nrt.z)

    };

    for (int i = 0; i < 2; i++)
    {
        children[i] = new kd_tree_node<T>(bound(p[i], e[i]), max_depth - 1, max_amount_of_objects);
    }

    for (int i = 0; i < objects->size(); i++)
    {
        int idx = get_child_id(objects->at(i).first);
        children[idx]->put(objects->at(i).first, objects->at(i).second);
    }
    delete objects;
    objects = nullptr;
    this->is_leaf_node = false;

}

template <typename T>
void kd_tree_node<T>::split_sah()
{
    bound bnd = this->bnd;
    if (!this->is_leaf_node) return;

}


template <typename T>
spatial_tree_node<T> ** kd_tree_node<T>::get_children()
{
    return (spatial_tree_node<T> **) children;
}

template <typename T>
vector<pair<point, T> > * kd_tree_node<T>::get_objects()
{
    return this->objects;
}

template <typename T>
int kd_tree_node<T>::get_children_size()
{
    if (!is_leaf())
        return 2;
    return 0;
}



template <typename T>
int kd_tree_node<T>::get_child_id(point p)
{

    if (p.x > this->median.x) return 1;
    else return  0;
}

template <typename T>
bool kd_tree_node<T>::is_leaf()
{
    return is_leaf_node;
}


/**
 *
 * @param axis - on what axis are we separating: x=0,y=1,z=2
 * @param num - what parttion are we on (1;N)
 * @return sah rating
 */
template <typename T>
point kd_tree_node<T>::split_with_sah()
{
    double objects_count[N], leftside_count[N-1], rightside_count[N-1];
    double sah, temp;
    point split_point;
    int axis = 0;

    bound bnd = this->bnd;


    for(pair<point, T> &i: *objects){
        objects_count[(int)fmod(i.first.x, N)];
    }

    for(int i = 0; i < N; i++){
        if(i == 0){
            leftside_count[i] = objects_count[i];
            rightside_count[i] = std::accumulate(std::begin(objects_count),std::end(objects_count), 0, std::plus<int>()) - objects_count[i];
        } else{
            leftside_count[i] = leftside_count[i-1]+objects_count[i];
            rightside_count[i] = rightside_count[i-1]-objects_count[i];
        }
    }

    sah = ct+ci*((k_part_surface(1, axis)*leftside_count[0]+k_part_surface(31, axis))/surface);
    split_point.set(bnd.flb.x+((bnd.nrt.x-bnd.flb.x)*1/N),bnd.flb.y,bnd.flb.z);
    for(int i = 1; i < N - 1; i++){
        temp = ct+ci*((k_part_surface(i+1, axis)*leftside_count[0]+k_part_surface(N-i-1, axis))/surface);
        if (temp<sah)
        {
            sah = temp;
            split_point.set(bnd.flb.x+((bnd.nrt.x-bnd.flb.x)*(i+1)/N),bnd.flb.y,bnd.flb.z);
        }
    }



    axis = 1;

    for(pair<point, T> &i: *objects){
        objects_count[(int)fmod(i.first.y, N)];
    }

    for(int i = 0; i < N; i++){
        if(i == 0){
            leftside_count[i] = objects_count[i];
            rightside_count[i] = std::accumulate(std::begin(objects_count),std::end(objects_count), 0, std::plus<int>()) - objects_count[i];
        } else{
            leftside_count[i] = leftside_count[i-1]+objects_count[i];
            rightside_count[i] = rightside_count[i-1]-objects_count[i];
        }
    }
    sah = ct+ci*((k_part_surface(1, axis)*leftside_count[0]+k_part_surface(31, axis))/surface);
    split_point.set(bnd.flb.x,bnd.flb.y+((bnd.nrt.y-bnd.flb.y)*1/N),bnd.flb.z);
    for(int i = 1; i < N - 1; i++){
        temp = ct+ci*((k_part_surface(i+1, axis)*leftside_count[0]+k_part_surface(N-i-1, axis))/surface);
        if (temp<sah){
            sah = temp;
            split_point.set(bnd.flb.x, bnd.flb.y+((bnd.nrt.y-bnd.flb.y)*(i+1)/N), bnd.flb.z);
        }
    }




    axis = 2;
    for(pair<point, T> &i: *objects){
        objects_count[(int)fmod(i.first.z, N)];
    }

    for(int i = 0; i < N; i++){
        if(i == 0){
            leftside_count[i] = objects_count[i];
            rightside_count[i] = std::accumulate(std::begin(objects_count),std::end(objects_count), 0, std::plus<int>()) - objects_count[i];
        } else{
            leftside_count[i] = leftside_count[i-1]+objects_count[i];
            rightside_count[i] = rightside_count[i-1]-objects_count[i];
        }
    }
    sah = ct+ci*((k_part_surface(1, axis)*leftside_count[0]+k_part_surface(31, axis))/surface);
    split_point.set(bnd.flb.x, bnd.flb.y, bnd.flb.z+((bnd.nrt.z-bnd.flb.z)*1/N));
    for(int i = 1; i < N - 1; i++){
        temp = ct+ci*((k_part_surface(i+1, axis)*leftside_count[0]+k_part_surface(N-i-1, axis))/surface);
        if (temp<sah)
        {
            sah = temp;
            split_point.set(bnd.flb.x, bnd.flb.y, bnd.flb.z+((bnd.nrt.z-bnd.flb.z)*(i+1)/N));
        }
    }

    return split_point;
}



/**
 * gets median of the box on X parameter. To be considered
 * @return
 */
template <typename T>
point kd_tree_node<T>::get_median() {
    vector<pair<point, T> > * sorted = this->objects;
    std::sort(sorted->begin(), sorted->end(), point_compare);
    this->median = sorted->at(sorted->size()/2).first;
    return sorted->at(sorted->size()/2).first;
}

template <typename T>
double kd_tree_node<T>::k_part_surface(int k, int axis){
    if(axis == 0){
        return this->surface-k/N*(this->bnd.height()*this->bnd.length())-k/N*(this->bnd.width()*this->bnd.length());
    } else if (axis == 1)
    {
        return this->surface-k/N*(this->bnd.height()*this->bnd.width())-k/N*(this->bnd.width()*this->bnd.length());
    }
    else if(axis == 2)
    {
        return this->surface-(k/N*(this->bnd.height()*this->bnd.width())+k/N*(this->bnd.height()*this->bnd.length()));
    } else return  -1;
}


#endif //SPATIAL_TREE_COMPARISON_KD_TREE_H
