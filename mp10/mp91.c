#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mp5.h"
#include "mp10.h"
#define MY_INFINITY 1000000000
#define INITIAL_SIZE 10

// Function to find leaf nodes: It checks if the given location falls within the range of the node, and if so, adds the node to the vertex set.
void find_leaf_node(locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum) {
    if (in_range(loc, p->node[nnum].x, p->node[nnum].y_left) || in_range(loc, p->node[nnum].x, p->node[nnum].y_right)) {
        if (vs->count == vs->id_array_size) {
            vs->id_array_size *= 2;
            vs->id = realloc(vs->id, vs->id_array_size * sizeof(int32_t));
            if (vs->id == NULL) {
                return;
            }
        }
        int i = vs->count - 1;
        while (i >= 0 && vs->id[i] > p->node[nnum].id) {
            vs->id[i + 1] = vs->id[i];
            i--;
        }
        vs->id[i + 1] = p->node[nnum].id;
        vs->count++;
    }
}


// Function to find internal nodes: It iterates through the four child nodes of the current node and recursively calls the find_nodes function if the child node falls within the given location and range.
void find_internal_node(locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum) {
    for (int i = 0; i < 4; i++) {
        int child = 4 * nnum + i + 1;
        double child_x = (i % 2 == 0) ? p->node[nnum].x : p->node[nnum].x;
        double child_y = (i < 2) ? p->node[nnum].y_left : p->node[nnum].y_right;
        if (child_x >= loc->x - loc->range || child_x <= loc->x + loc->range) {
            if (in_range(loc, child_x, child_y)) {
                find_nodes(loc, vs, p, child); // Recursively call the find_nodes function
            } else if (child_y >= loc->y - loc->range || child_y <= loc->y + loc->range) {
                find_nodes(loc, vs, p, child); // Recursively call the find_nodes function
            }
        }
    }
}

// Function to find nodes: It checks if the given node number is valid and calls the appropriate function based on the node type.
void find_nodes(locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum) {
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    if (nnum < 0 || nnum >= p->n_nodes) { // If the node number is out of bounds, return.
        return;
    }
    if (vs->id == NULL) {
        vs->id_array_size = INITIAL_SIZE;
        vs->id = malloc(vs->id_array_size * sizeof(int32_t));
        if (vs->id == NULL) {
            return;
        }
    }
    if (4 * nnum + 1 >= p->n_nodes) { // If the node is a leaf node, call the find_leaf_node function.
        find_leaf_node(loc, vs, p, nnum);
    } else { // Otherwise, call the find_internal_node function.
        find_internal_node(loc, vs, p, nnum);
    }
}

void trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    int32_t in_range_ids[vs->count];
    int32_t in_range_count = 0;

    // Go through vertices
    for (int32_t i = 0; i < vs->count; i++) {
        // If in range, add to the temporary array
        if (in_range(loc, g->vertex[vs->id[i]].x, g->vertex[vs->id[i]].y)) {
            in_range_ids[in_range_count] = vs->id[i];
            in_range_count++;
        }
    }

    // Copy the in range vertices back to the original vertex set
    for (int32_t i = 0; i < in_range_count; i++) {
        vs->id[i] = in_range_ids[i];
    }

    // Update the count of the vertex set
    vs->count = in_range_count;

}
/**
 * brief: Swap the positions of two elements in the heap and update the heap identifiers in the graph.
 * 
 *  g Pointer to the graph structure.
 *  h Pointer to the heap structure.
 *  hid_1 Index of the first element in the heap.
 *  hid_2 Index of the second element in the heap.
 */
void HeapSwapping(graph_t* g, heap_t* h, int32_t hid_1, int32_t hid_2) {
    // Get the identifiers of the two vertices in the graph
    int32_t gid_1 = h->elt[hid_1];
    int32_t gid_2 = h->elt[hid_2];

    // Update the heap identifiers in the graph vertices
    g->vertex[gid_1].heap_id = hid_2;
    g->vertex[gid_2].heap_id = hid_1;

    // Swap the positions of the two elements in the heap
    int32_t temp = h->elt[hid_1];
    h->elt[hid_1] = h->elt[hid_2];
    h->elt[hid_2] = temp;
}

/**
 * brief Perform an upward heap operation to maintain the heap property after inserting an element.
 * 
 * graph Pointer to the graph structure.
 * heap Pointer to the heap structure.
 * currentNode Index of the current node in the heap.
 */
void UpHeap(graph_t* graph, heap_t* heap, int32_t currentNode) {
    while (currentNode > 0) {
        // Calculate the index of the parent node
        int32_t parentNode = (currentNode - 1) / 2;

        // Get distances from source for the parent and current nodes
        int32_t distanceParent = graph->vertex[heap->elt[parentNode]].from_src;
        int32_t distanceNode = graph->vertex[heap->elt[currentNode]].from_src;

        // Check if the parent node has a greater distance from the source
        if (distanceParent > distanceNode) {
            // Swap the current node with its parent in the heap
            HeapSwapping(graph, heap, parentNode, currentNode);
            currentNode = parentNode;
        } else {
            // Stop the loop if the heap property is satisfied
            break;
        }
    }
}

/**
 * brief Perform a downward heap operation to maintain the heap property after extracting the minimum element.
 * 
 * graph Pointer to the graph structure.
 * heap Pointer to the heap structure.
 * currentNode Index of the current node in the heap.
 */
void DownHeap(graph_t* graph, heap_t* heap, int32_t currentNode) {
    while (currentNode * 2 + 1 < heap->n_elts) {
        // Initialize the smallest node as the current node
        int32_t smallestNode = currentNode;

        // Get distances from source for the current node and its left child
        int32_t distanceCurrent = graph->vertex[heap->elt[currentNode]].from_src;
        int32_t distanceLeft = graph->vertex[heap->elt[currentNode * 2 + 1]].from_src;

        // Update smallestNode if the left child has a smaller distance
        if (distanceLeft < distanceCurrent) {
            smallestNode = currentNode * 2 + 1;
        }

        // Check if the right child exists
        if (currentNode * 2 + 2 < heap->n_elts) {
            // Get distance from source for the right child
            int32_t distanceRight = graph->vertex[heap->elt[currentNode * 2 + 2]].from_src;

            // Update smallestNode if the right child has a smaller distance than the left child
            if (distanceRight < distanceCurrent && distanceRight < distanceLeft) {
                smallestNode = currentNode * 2 + 2;
            }
        }

        // Check if a swap is needed to maintain the heap property
        if (smallestNode != currentNode) {
            // Swap the current node with the smallest child in the heap
            HeapSwapping(graph, heap, smallestNode, currentNode);
            currentNode = smallestNode;
        } else {
            // Stop the loop if the heap property is satisfied
            break;
        }
    }
}


/**
 * Perform Dijkstra's algorithm to find the shortest path from a source set of vertices to a destination set.
 * 
 * graph Pointer to the graph structure.
 * heap Pointer to the heap structure.
 * sourceSet Pointer to the source set of vertices.
 * destinationSet Pointer to the destination set of vertices.
 * path Pointer to the structure to store the resulting path.
 * Returns 1 if a valid path is found, 0 otherwise.
 */
void execute_dijkstra(graph_t* graph, heap_t* heap) {
    while (heap->n_elts > 0) {
        --heap->n_elts;
        HeapSwapping(graph, heap, 0, heap->n_elts);
        DownHeap(graph, heap, 0);

        // Process the current node
        int32_t currentNode = heap->elt[heap->n_elts];
        vertex_t* currentVertex = &graph->vertex[currentNode];

        // Update distances and perform heap operations for neighbors
        for (int i = 0; i < currentVertex->n_neighbors; i++) {
            int32_t neighborNode = currentVertex->neighbor[i];
            vertex_t* neighborVertex = &graph->vertex[neighborNode];
            int32_t distance = currentVertex->from_src + currentVertex->distance[i];

            // Relaxation step
            if (distance < neighborVertex->from_src) {
                neighborVertex->from_src = distance;
                neighborVertex->pred = currentNode;
                UpHeap(graph, heap, neighborVertex->heap_id);
            }
        }
    }
}

int32_t dijkstra(graph_t* graph, heap_t* heap, vertex_set_t* sourceSet, vertex_set_t* destinationSet, path_t* path) {
    // Initialize distances and heap
    // Set the number of elements in the heap to the number of vertices in the graph
    heap->n_elts = graph->n_vertices;

    // Iterate through each vertex in the graph and initialize relevant information
    for (int32_t i = 0; i < graph->n_vertices; i++) {
        // Initialize the predecessor, distance from the source, and heap identifier of the vertex
        graph->vertex[i].pred = MY_INFINITY;
        graph->vertex[i].from_src = MY_INFINITY;
        graph->vertex[i].heap_id = i;

        // Set the vertex identifier in the heap
        heap->elt[i] = i;
    }

    // Set distances from source to 0 for vertices in the source set
    for (int i = 0; i < sourceSet->count; i++) {
        graph->vertex[sourceSet->id[i]].from_src = 0;
    }

    // Heapify the heap
    for (int i = heap->n_elts - 1; i >= 0; i--) {
        DownHeap(graph, heap, i);
    }
    // Execute Dijkstra's algorithm
    execute_dijkstra(graph, heap);

    // Find the best destination and build the path
    path->tot_dist = MY_INFINITY;
    int32_t bestDestination = -1;

    // Iterate through the destination set
    for (int i = 0; i < destinationSet->count; i++) {
        int32_t currentDestination = destinationSet->id[i];
        int32_t currentDistance = graph->vertex[currentDestination].from_src;

        // Update the best destination if a shorter path is found
        if (currentDistance < path->tot_dist) {
            bestDestination = currentDestination;
            path->tot_dist = currentDistance;
        }
    }

    // If no valid destination is found, return 0
    if (bestDestination == -1) return 0;

    // Reconstruct the path
    path->n_vertices = 0;
    int32_t traceNode = bestDestination;

    // Count the number of vertices in the path
    while (graph->vertex[traceNode].pred != MY_INFINITY) {
        path->n_vertices++;
        traceNode = graph->vertex[traceNode].pred;
    }

    path->n_vertices++;
    traceNode = bestDestination;


    // Allocate memory for the path
    int32_t path_length = path->n_vertices;

    path->id = malloc(path_length * sizeof(int32_t));
    if (path->id == NULL) {
        return 0;
    }

    // Populate the path array in reverse order
    for (int i = path->n_vertices - 1; i >= 0; i--) {
        path->id[i] = bestDestination;
        bestDestination = graph->vertex[bestDestination].pred;
    }
    if(path->n_vertices==0||path->n_vertices==1){
        return 0;
    }
    return 1;
    
}

