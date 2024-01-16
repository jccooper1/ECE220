#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"
/*new_vertex_set: This function allocates memory for a new vertex_set_t structure and its id array. The id array is initially allocated space for 64 integers. If the allocation for either the structure or the id array fails, any allocated memory is freed and the function returns NULL. If the allocation is successful, the count and minimap fields of the structure are initialized to 0, the id field is set to point to the allocated id array, and the id_array_size field is set to 64. The function then returns a pointer to the newly allocated structure.
*/
/*free_vertex_set: This function frees the memory allocated for a vertex_set_t structure and its id array. If the id field of the structure is not NULL, the memory for the id array is freed. Then, the memory for the structure itself is freed.
*/
/*new_path: This function allocates memory for a new path_t structure and its id array. The id array is initially allocated space for 64 integers. If the allocation for either the structure or the id array fails, any allocated memory is freed and the function returns NULL. If the allocation is successful, the n_vertices and minimap fields of the structure are initialized to 0, the id field is set to point to the allocated id array, and the id_array_size field is set to 64. The function then returns a pointer to the newly allocated structure.
*/
/*free_path: This function frees the memory allocated for a path_t structure and its id array. If the id field of the structure is not NULL, the memory for the id array is freed. Then, the memory for the structure itself is freed.
*/
/*Allocation calls should return
pointers to the new objects, or NULL
on failure*/

vertex_set_t*
new_vertex_set ()
{
    //allocate space for  the new vertex set and id
    vertex_set_t* vsb=malloc(sizeof(vertex_set_t));
    int32_t* idb=malloc(sizeof(int32_t)*64);
    if(vsb==NULL){
        return NULL;
    }
    if(idb==NULL){
        free(idb);
        return NULL;
    }
    vsb->count=0;
    vsb->minimap=0;
    vsb->id=idb;
    vsb->id_array_size=64;       
    return vsb;
}


void free_vertex_set (vertex_set_t* vs)
{
    if(vs != NULL) {
        if(vs->id != NULL) {
            free(vs->id); // free id if it's not NULL
        }
        free(vs); // free the vertex set
    }
}

path_t*
new_path ()
{
//allocate space for the new path and id  
//and then check the space is enough or not
//if it's empty, free space and return NULL 
    path_t* pathb=malloc(sizeof(path_t));
    int32_t* idpathb=malloc(sizeof(int32_t)*64);
    if(pathb==NULL){
        return NULL;
    }
    if(idpathb==NULL){
        free(idpathb);
        return NULL;
    }
    pathb->n_vertices=0;
    pathb->minimap=0;
    pathb->id=idpathb;
    return pathb;
}


void
free_path (path_t* path)
{
    if(path!=NULL){
        if(path->id!=NULL){
            free(path->id);
        }
        free(path);
    }
}

