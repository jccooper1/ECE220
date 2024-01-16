#include <stdint.h>
#include <stdio.h>
#include "mp5.h"
#include "mp9.h"
int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
	src_vs->count = 0;
	dst_vs->count = 0;
	path->n_vertices = 0;
	path->tot_dist = 0;
	// Find the vertices in the graph that are within range of the starting
	// and ending locales for the two requests
	find_nodes(&r1->from, src_vs, p, 0);
	find_nodes(&r1->to, dst_vs, p, 0);
	// Remove any vertices not in range of the starting and ending locales
	// from the vertex sets
	trim_nodes(g, src_vs, &r2->from);
	trim_nodes(g, dst_vs, &r2->to);
	// Find a shortest path between any vertex in the starting set and any
	// vertex in the ending set
	if (!dijkstra(g, h, src_vs, dst_vs, path) || src_vs->count ==0 || dst_vs->count == 0 )
		return 0;

	return 1; 
}

//----------------------------------------------------------------------------------------------------------------------------------------
//this is the head file of mp9
/*struct vertex_t {
    int32_t  x;			// x position
    int32_t  y;			// y position
    int32_t  n_neighbors;	// number of neighbors
    // arrays of size n_neighbors
    int32_t* neighbor;		// neighbor indices in graph's vertex array
    int32_t* distance;		// Euclidean distance to neighbors

    // for use with Dijkstra's single-source shortest-path algorithm
    int32_t heap_id;		// heap array index for this vertex 
    int32_t from_src;		// distance from source
    int32_t pred;		// predecessor graph node (array index)
};

struct graph_t {
    int32_t   n_vertices;	// number of vertices in the graph
    vertex_t* vertex;		// array of vertices
};

struct pyr_node_t {
    int32_t x;			// x position of node / x-splitter
    int32_t y_left;		// y position of node / left y-splitter
    int32_t y_right;		// y position of node / right y-splitter
    int32_t id;			// graph vertex array index (for leaf nodes)
};

struct pyr_tree_t {
    int32_t     n_nodes;	// number of nodes in tree
    pyr_node_t* node;		// array of nodes
};

struct locale_t {
    int32_t x;			// x position of location
    int32_t y;			// y position of location
    int32_t range;		// maximum distance to chosen node 
};

struct request_t {
    locale_t from;		// source / starting position
    locale_t to;		// destination / ending position
};

#define MAX_IN_VERTEX_SET 100	// maximum nodes considered
struct vertex_set_t {
    int32_t count;		  // number of graph vertices found
    int32_t id[MAX_IN_VERTEX_SET];// array of graph vertex array indices
    				  //    (valid ids from 0 to count-1)
};

struct heap_t {
    int32_t  n_elts;		// number of things in the heap
    int32_t* elt;		// array of size equal to vertices in graph
};

#define MAX_PATH_LENGTH 500	// max length of a path
struct path_t {
    int32_t n_vertices;		// number of actual vertices in path
    int32_t tot_dist;		// sum of distances along edges in path
    int32_t id[MAX_PATH_LENGTH];// graph vertex array indices for path
};
*/