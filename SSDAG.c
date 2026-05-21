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
            ERRNO1;
            break;
        case 1: 
            ERRNO2;
            break;
        default:
            ERRNO0;
            break;
    }

    exit(1);
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

/**
 * @brief Adds the edge dependency between vertexes 
 * only after detecting a potential cycle that breaks
 * the DAG nature of a Graph.
 * @param StateMatrix pointer
 * @param int source
 * @param int target
 * @return void
 */
void add_dependency(StateMatrix *sm, int source, int target)
{
    if((source < MIN_VAL || source > MAX_VAL) || (target < MIN_VAL || target > MAX_VAL)) handle_error(VALUE_ERROR);
    uint64_t simulated_radius = resolve_cascade(sm, (1ULL << target));

    if (simulated_radius & (1ULL << source)) handle_error(CYCLE_ERROR);
    
    sm->matrix[source] |= (1ULL << target);
    printf("invisible stitching phase: source %d -> mask %llu\n", source, sm->matrix[source]);
}
