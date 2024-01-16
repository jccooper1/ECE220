#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mp5.h"
#include "mp10.h"


//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//
/*The function first allocates memory for various vertex sets and checks for successful allocation. It then fills the vertex sets for the request and checks if they are empty. If they are, it cleans up and returns.
Next, it builds minimaps of the vertex sets for the request and allocates memory for "id" arrays for matching. If the allocation fails, it cleans up and returns.
The function then initializes the 'match_src_vs' and 'match_dst_vs' structures and traverses through the "available" linked list. For each node in the list, it allocates vertex sets, fills them, and checks if they are non-empty. If they are, it builds minimaps of the vertex sets.
If the minimaps match and the vertex sets intersect and a valid path is found, it cleans up and overwrites the vertex sets and path of the request and the current node with the matching ones. It then builds minimaps of the matching vertex sets and path, and links the partners into "shared".
If the vertex sets for the current node are empty, it cleans up. Finally, it links the request into "available" and cleans up the "id" arrays for matching.
The cleanup function is a utility function that frees memory allocated for various data structures. The conditions_met function checks if the minimaps match and the vertex sets intersect and a valid path is found.
*/
static request_t* available = NULL;
static request_t* shared = NULL;
bool conditions_met(request_t* r, request_t* becky, vertex_set_t* match_src_vs, vertex_set_t* match_dst_vs, graph_t* g, heap_t* h) {
    return (r->src_vs->minimap & becky->src_vs->minimap)
        && (r->dst_vs->minimap & becky->dst_vs->minimap)
        && merge_vertex_sets(r->src_vs, becky->src_vs, match_src_vs)
        && merge_vertex_sets(r->dst_vs, becky->dst_vs, match_dst_vs)
        && dijkstra(g, h, match_src_vs, match_dst_vs, r->path);
}
void cleanup (vertex_set_t* source_vertex_set, vertex_set_t* destination_vertex_set, vertex_set_t* matching_source_vertex_set, vertex_set_t* matching_destination_vertex_set, 
        vertex_set_t* available_source_vertex_set, vertex_set_t* available_destination_vertex_set, path_t* path, int32_t* source_id_array, int32_t* destination_id_array) {
    if (source_id_array) free(source_id_array);
    if (destination_id_array) free(destination_id_array);
    if (source_vertex_set) free_vertex_set(source_vertex_set);
    if (destination_vertex_set) free_vertex_set(destination_vertex_set);
    if (matching_source_vertex_set) free_vertex_set(matching_source_vertex_set);
    if (matching_destination_vertex_set) free_vertex_set(matching_destination_vertex_set);
    if (available_source_vertex_set) free_vertex_set(available_source_vertex_set);
    if (available_destination_vertex_set) free_vertex_set(available_destination_vertex_set);
    if (path) free_path(path);
}

int32_t handle_request(graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r) {
    // Allocate memory for matching source and destination vertex sets
    vertex_set_t* match_src_vs = malloc(sizeof(vertex_set_t));
    vertex_set_t* match_dst_vs = malloc(sizeof(vertex_set_t));

    // Initialize source and destination vertex sets and path for the request
    r->src_vs = new_vertex_set();
    r->dst_vs = new_vertex_set();
    r->path = new_path();

    // Check if memory allocation was successful, if not, cleanup and return 0
    if (r->src_vs == NULL || r->dst_vs == NULL || match_src_vs == NULL || match_dst_vs == NULL || r->path == NULL) {
        cleanup(r->src_vs, r->dst_vs, NULL, NULL, match_src_vs, match_dst_vs, r->path, NULL, NULL);
        return 0;
    }

    // Find nodes for the source and destination of the request
    find_nodes(&r->from, r->src_vs, p, 0);
    find_nodes(&r->to, r->dst_vs, p, 0);

    // If either source or destination vertex set is empty, cleanup and return 0
    if (r->src_vs->count==0 || r->dst_vs->count==0) {
        cleanup(r->src_vs, r->dst_vs, NULL, NULL, match_src_vs, match_dst_vs, r->path, NULL, NULL);
        return 0;
    }

    // Mark minimap for the graph and build minimaps for the source and destination vertex sets
    mark_vertex_minimap(g, p);
    build_vertex_set_minimap(g, r->src_vs);
    build_vertex_set_minimap(g, r->dst_vs);

    // Allocate memory for the id arrays of the matching vertex sets
    match_dst_vs->id = malloc(sizeof(int32_t) * r->dst_vs->count);
    match_src_vs->id = malloc(sizeof(int32_t) * r->src_vs->count);

    // If memory allocation for the id arrays failed, cleanup and return 0
    if (match_src_vs->id==NULL || match_dst_vs->id==NULL) {
        cleanup(r->src_vs, r->dst_vs, NULL, NULL, match_src_vs, match_dst_vs, r->path, match_src_vs->id, match_dst_vs->id);
        return 0;
    }

    // Initialize the matching vertex sets
    match_src_vs->count = 0;
    match_src_vs->minimap = 0;
    match_dst_vs->count = 0;
    match_dst_vs->minimap = 0;
    match_dst_vs->id_array_size = r->dst_vs->count;
    match_src_vs->id_array_size = r->src_vs->count;

    // Start processing the available requests
    request_t* becky = available;
    request_t* prev;

    // Loop through all available requests
    while (becky!=NULL) {
        // Initialize source and destination vertex sets for the current request
        becky->src_vs = new_vertex_set();
        becky->dst_vs = new_vertex_set();

        // If memory allocation failed, cleanup and return 0
        if (becky->src_vs==NULL|| becky->dst_vs==NULL) {
            cleanup(r->src_vs, r->dst_vs, becky->src_vs, becky->dst_vs, match_src_vs, match_dst_vs, r->path, match_src_vs->id, match_dst_vs->id);
            return 0;
        }

        // Find nodes for the source and destination of the current request
        find_nodes(&becky->from, becky->src_vs, p, 0);
        find_nodes(&becky->to, becky->dst_vs, p, 0);

        // If both source and destination vertex sets are non-empty
        if (becky->src_vs->count!=0 && becky->dst_vs->count!=0) {
            // Build minimaps for the source and destination vertex sets
            build_vertex_set_minimap(g, becky->src_vs);
            build_vertex_set_minimap(g, becky->dst_vs);

            // If conditions are met for the request and the current request
            if (conditions_met(r, becky, match_src_vs, match_dst_vs, g, h)) {
                // If the current request is the first in the available list, update the first request
                if (becky == available){
                    available = becky->next;
                } else {
                    // Otherwise, update the next request of the previous request
                    prev->next = becky->next;	
                }

                // Cleanup the source and destination vertex sets of the request and the current request
                cleanup(r->src_vs, r->dst_vs, becky->src_vs, becky->dst_vs,NULL, NULL, NULL, NULL, NULL);

                // Update the source and destination vertex sets and path of the request and the current request
                r->src_vs = match_src_vs;
                becky->src_vs = match_src_vs;
                r->dst_vs = match_dst_vs;
                becky->dst_vs = match_dst_vs;
                becky->path = r->path;
                // Build minimaps for the matching vertex sets and path
                build_vertex_set_minimap(g, match_src_vs);
                build_vertex_set_minimap(g, match_dst_vs);
                build_path_minimap(g, r->path);
                // Update the partner and next request of the current request and the request
                becky->partner = NULL;
                becky->next = NULL;
                r->partner = becky;
                r->next = shared;
                shared = r;
                return 1;
            }
        } else {
            // If either source or destination vertex set is empty, cleanup
            cleanup(NULL, NULL, becky->src_vs, becky->dst_vs, NULL, NULL, NULL, NULL, NULL);
        }

        // Move to the next request
        prev = becky;
        becky = becky->next;
    }

    // If no matching request was found, add the request to the available list
    r->next = available;
    available = r;

    // Cleanup the id arrays of the matching vertex sets
    cleanup(NULL, NULL, NULL, NULL, NULL, NULL, NULL, match_src_vs->id, match_dst_vs->id);
    return 1;
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }

    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


