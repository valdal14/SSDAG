#include <stdio.h>
#include <stdlib.h>
#include "SSDAG.h" 

/**
 * @brief Handles the error based on the given type
 * @param int error_type: Defined in the SSDAG.h 
 * @return void
 */
void handle_error(int error_type)
{
    switch(error_type)
    {
        case 0:
        {
            ERRNO1;
            exit(1);
        }
        default:
        {
            ERRNO0;
            exit(1);
        }
    }
}

/**
 * @brief Adds the edge dependency between vertexes
 * @param StateMatrix pointer
 * @param int source
 * @param int target
 * @return void
 */
void add_dependency(StateMatrix *sm, int source, int target)
{
    if((source < MIN_VAL || source > MAX_VAL) || (target < MIN_VAL || target > MAX_VAL)) handle_error(VALUE_ERROR);
    sm->matrix[source] |= (1ULL << target);
    printf("invisible stitching phase: source %d -> mask %llu\n", source, sm->matrix[source]);
}

int main(void)
{
    StateMatrix dag = {0};
    add_dependency(&dag, 0, 14);
    return 0;
}
