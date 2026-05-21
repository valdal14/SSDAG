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

/**
 * @brief Takes the current_state of the system, look up the specific adjacency mask
 * for the trigger_node, and return the new global state by applying a single bitwise
 * OR operation between the current state and the trigger node's mask.
 * @param StateMatrix pointer
 * @param uint64_t current_state
 * @param int trigger_node
 * @return uint64_t
 */
uint64_t get_blast_radius(StateMatrix *sm, uint64_t current_state, int trigger_node)
{
    uint64_t state = sm->matrix[trigger_node];
    state = state | current_state;
    printf("invisible stitching phase: current_state %llu -> new state %llu\n", current_state, state);
    return state;
}

/**
 * @brief Resolves dependency cascade between vertexes
 * @param StateMatrix pointer
 * @param uint64_t initial_state
 * @return uint64_t
 */
uint64_t resolve_cascade(StateMatrix *sm, uint64_t initial_state)
{
    uint64_t current_state = initial_state;
    // Starts at 0 so the loop runs at least once
    uint64_t previous_state = 0; 

    while (current_state != previous_state)
    {
        // Lock in the current state before we check the next
        previous_state = current_state;

        for (int i = 0; i < 64; i++)
        {
            if (current_state & (1ULL << i))
            {
                current_state |= sm->matrix[i]; 
            }
        }
    }

    return current_state;
}

int main(void)
{
    StateMatrix dag = {0};
    // Node 0 is currently the only thing failing. 1 in binary is ...0001, which corresponds to bit 0
    uint64_t global_state = 1;
    
    add_dependency(&dag, 0, 1);
    add_dependency(&dag, 0, 2);
    add_dependency(&dag, 2, 5);

    uint64_t state = get_blast_radius(&dag, global_state, 0);
    uint64_t new_state = resolve_cascade(&dag, global_state);
    printf("invisible stitching phase: current_state %llu -> new state %llu\n", global_state, new_state);
    return 0;
}
