#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mp5.h"
#include "mp9.h"
#define MY_INFINITY   1000000000
//this functions identify whether the nodes are in the range by first judging the round position, which distinguishes the node is 
// in the right part or the left part of the tree, and then judge whether the node is in the range by comparing the x and y value
//then use the inrange function to help to judge whether the node is in the range. at last, starting to recurse.
void find_nodes(locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    
    // Do not modify the following line nor add anything above in the function.
    record_fn_call();
    // Local variables for frequently accessed values
    int32_t x = p->node[nnum].x;
    int32_t y_left = p->node[nnum].y_left;
    int32_t y_right = p->node[nnum].y_right;
    int32_t loc_x = loc->x;
    int32_t loc_y = loc->y;
    int32_t loc_range = loc->range;
    if(4*nnum+1 >= p->n_nodes){
        // If it is a leaf node, then check if it is in the range
        if(in_range(loc, x, y_left) ){
            // If it is in the range, then add it to the vertex set
            vs->id[vs->count++] = p->node[nnum].id;
        }
    }
        else{
            // Check the left part first
            if(x >= loc_x - loc_range){
                if(in_range(loc, x, y_left)){
                    find_nodes(loc, vs, p, 4*nnum+1);
                    find_nodes(loc, vs, p, 4*nnum+3);
                }
                else{
                    if(loc_y >= y_left - loc_range ){
                        find_nodes(loc, vs, p, 4*nnum+3);
                    }
                    if(loc_y <= y_left + loc_range ){
                        find_nodes(loc, vs, p, 4*nnum+1);
                    }
                }
            }
            // This is for the right part 
            if(x <= loc_x + loc_range){
                if(in_range(loc, x, y_right)){
                    find_nodes(loc, vs, p, 4*nnum+2);
                    find_nodes(loc, vs, p, 4*nnum+4);
                }
                else{
                    if(loc_y <= y_right + loc_range){
                        find_nodes(loc, vs, p, 4*nnum+2);
                    }
                    if(loc_y >= y_right - loc_range ){
                        find_nodes(loc, vs, p, 4*nnum+4);
                    }
                }
            }
        }
    }
//in this function we tried to move those useless nodes inside the tree
//we will check every element in the vertex set, if it is not in the range, we will move it to the end of the vertex set and decrease the count
//if it is in the range, we will move to the next element
void trim_nodes(graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    int32_t i = 0;
    int32_t j=0;
    while (i < vs->count) {
        // Get the x and y coordinates of the vertex
        int32_t ax = g->vertex[vs->id[i]].x;
        int32_t ay = g->vertex[vs->id[i]].y;
        // If the vertex is not in range of the locale, remove it from the vertex set
        if (!in_range(loc, ax, ay)) {
            // Move every element forward one step
            for(j=i; j<vs->count-1; j++){
                vs->id[j] = vs->id[j+1];
            // Decrease the count of vertices in the set
            }
            vs->count--;
        } else {
            // Move to the next vertex
            i++;
        }
    }
}
//for the preparation job, we need to initialize the heap, and then we need to initialize the path
//to save the efforts in advance
void init(heap_t* h, graph_t* g) {
    // Initialize the number of elements in the heap
    h->n_elts = g->n_vertices;
    // Initialize the distances and the heap elements
    for (int32_t i = 0; i < g->n_vertices; i++) {
        // Initialize the heap element
        h->elt[i] = i;
        g->vertex[i].heap_id = i;
        // Initialize the distance to infinity
        g->vertex[i].pred = MY_INFINITY;
        g->vertex[i].from_src = MY_INFINITY;
    }
}
void up_to_down(heap_t*h,graph_t*g,int32_t i){
    int32_t smallest = i;
    int32_t left = 2 * i + 1;
    int32_t right = 2 * i + 2;

    // If the left child is smaller than the root
    if (left < h->n_elts && g->vertex[h->elt[left]].from_src < g->vertex[h->elt[smallest]].from_src) {
        smallest = left;
    }
    // If the right child is smaller than the smallest so far
    if (right < h->n_elts && g->vertex[h->elt[right]].from_src < g->vertex[h->elt[smallest]].from_src) {
        smallest = right;
    }
    // If the smallest is not the root
    if (smallest != i) {
        // Swap the root with the smallest
  
        g->vertex[h->elt[i]].heap_id = smallest;
        g->vertex[h->elt[smallest]].heap_id = i;
        int32_t becky = h->elt[i];
        h->elt[i] = h->elt[smallest];
        h->elt[smallest] = becky;
        // Recursively heapify the affected sub-tree
        up_to_down(h, g, smallest);
    }
}
void down_to_up(heap_t*h,graph_t*g,int32_t i){
    // Get the parent of the node
    int32_t parent = (i - 1) / 2;
    // While the node is not the root and its key is less than its parent's key
    if(i > 0 && g->vertex[h->elt[i]].from_src < g->vertex[h->elt[parent]].from_src) {
        // Swap the node with its parent
    
        g->vertex[h->elt[i]].heap_id = parent;
        g->vertex[h->elt[parent]].heap_id = i;
            int32_t becky = h->elt[i];
        h->elt[i] = h->elt[parent];
        h->elt[parent] = becky;
        down_to_up(h, g, parent);
    }
}

int32_t dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest, path_t* path) {					
	init(h,g);												
    //then we need to set the source points
    for (int32_t i = 0; i < src->count; i++) {
        g->vertex[src->id[i]].from_src = 0;
    }
    for(int32_t i=h->n_elts-1;i>=0;i--){
        up_to_down(h, g, i);
    }
    for(h->n_elts=h->n_elts-1;h->n_elts>0;h->n_elts--){
      
        g->vertex[h->elt[0]].heap_id = h->n_elts;
        g->vertex[h->elt[h->n_elts]].heap_id = 0;
          int32_t becky= h->elt[0];
        h->elt[0] = h->elt[h->n_elts];
        h->elt[h->n_elts] = becky;
        up_to_down(h, g, 0);
        //find the index of our positons at this moment
        int32_t pol=h->elt[h->n_elts];
        //then we need to find the neighbors of the current node
        for(int32_t i=0;i<g->vertex[pol].n_neighbors;i++){
            //then we need to find the distance between the current node and its neighbors
            int32_t distance=g->vertex[pol].from_src+g->vertex[pol].distance[i];
            //then we need to find the index of the neighbors
            int32_t neighbor=g->vertex[pol].neighbor[i];
            int32_t neighbor_distacne=g->vertex[neighbor].from_src;
            //then we need to compare the distance between the current node and its neighbors
            if(distance<neighbor_distacne){
                g->vertex[neighbor].from_src=distance;
                g->vertex[neighbor].pred=pol;
                down_to_up(h, g, g->vertex[neighbor].heap_id);
            }   
        }
    }
	path->tot_dist = MY_INFINITY;
    int32_t small_nodes;
    for(int32_t c=0; c<dest->count ;c++)
        if(g->vertex[dest->id[c]].from_src<path->tot_dist){
            small_nodes=dest->id[c];
            path->tot_dist=g->vertex[dest->id[c]].from_src;
    }
    path->n_vertices=1;
    int32_t inter_nodes=small_nodes;
    //now find all the nodes in the path
    for (; g->vertex[inter_nodes].pred != MY_INFINITY; path->n_vertices++) {
    inter_nodes = g->vertex[inter_nodes].pred; // Trace through the path
    }
    if(path->n_vertices==0||path->n_vertices>MAX_PATH_LENGTH||path->n_vertices==1){
        return 0;
    }   
    //now we need to find the path  
    for(int32_t i=path->n_vertices-1;i>=0;i--){
        path->id[i]=small_nodes;
        small_nodes=g->vertex[small_nodes].pred;
    }
    return 1;
}