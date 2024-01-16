#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mp10.h"
/*becky_mark: This recursive function traverses a pyramid tree and marks each vertex in the graph with the index of its ancestor in the pyramid tree. The index is adjusted by subtracting 21.
*/
/*mark_vertex_minimap: This function marks each vertex in the graph with the index of its ancestor in the pyramid tree. If the number of vertices in the graph is less than 64, the index is simply the index of the vertex in the vertex array. Otherwise, the index is the index of the vertex’s ancestor in the pyramid tree minus 21.
*/
/*build_vertex_set_minimap: This function builds a minimap for a vertex set. The minimap is a 64-bit integer with a 1 in the ith bit if the ith vertex in the graph is in the vertex set, and a 0 otherwise. The function loops through the vertices in the vertex set, and for each vertex, it sets the ith bit in the minimap.
*/
/*build_path_minimap: This function builds a minimap for a path. The minimap is a 64-bit integer with a 1 in the ith bit if the ith vertex in the graph is in the path, and a 0 otherwise. The function loops through the vertices in the path, and for each vertex, it sets the ith bit in the minimap.
*/
/*merge_vertex_sets: This function merges two vertex sets into a third vertex set. The function loops through the vertices in the first vertex set, and for each vertex, it checks if the vertex is in the second vertex set. If it is, the vertex is added to the third vertex set. The function returns 1 if the third vertex set is not empty, and 0 otherwise.
*/
void becky_mark(graph_t* g,pyr_tree_t* p,int32_t ancestor) {
    int32_t children_index=ancestor*4+1;
    if(children_index>=p->n_nodes){
        g->vertex[p->node[ancestor].id].mm_bit=ancestor-21;
        return;
    }
    else{
        for(int32_t i=0;i<4;i++){
            if(children_index+i<p->n_nodes){
            becky_mark(g,p,children_index+i);
            }
        }
    }
}
int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    //first check the number of vertices is less than 64 or not
    ///
    if(p->n_nodes<=64){
        for(int32_t i=0;i<64;i++){
            g->vertex[p->node[i].id].mm_bit=i;
        }
    }
    // Otherwise, for each graph vertex, identify the pyramid tree node corresponding to each vertex,
    // then find the ancestor node with index from 21 to 84 (these are the nodes at the fourth level of the pyramid tree).
    // Finally, set the vertex’ mm_bit to the ancestor’s node index minus 21.
    else{
        for(int32_t i=21;i<=84;i++){
            becky_mark(g,p,i);
        }
    }
    return 1;
}


void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    //use the loop to left shift the minimap
    //when we find the vertex, we use the or operation to add the minimap
    for(int32_t i=0;i<vs->count;i++){
        vs->minimap|=1ULL<<g->vertex[vs->id[i]].mm_bit;
    }
}


void 
build_path_minimap (graph_t* g, path_t* p)
{
    //use the loop to left shift the minimap
    //when we find the vertex, we use the or operation to add the minimap
    for(int32_t i=0;i<p->n_vertices;i++){
        p->minimap|=1ULL<<g->vertex[p->id[i]].mm_bit;
    }
}


int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    //first we initailize the count to 0    
    vint->count=0;
    //loop to iterate every element in the two vertex sets
    for(int32_t i=0;i<v1->count;i++){
        for(int32_t j=0;j<v2->count;j++){
            if(v1->id[i]==v2->id[j]){
                vint->id[vint->count++]=v1->id[i];
            }
        }
    }
    if(vint->count==0){
        return 0;
    }
    else{
        return 1;
    }
}

