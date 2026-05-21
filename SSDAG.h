#ifndef SSDAG_H
#define SSDAG_H

#include <stddef.h>
#include <stdint.h>

#define MATRIX_SIZE 64
#define MIN_VAL 0
#define MAX_VAL 63
// Types
#define VALUE_ERROR 0
#define CYCLE_ERROR 1
// Error Messages
#define ERRNO0 fprintf(stderr, "Unsupported type\n");
#define ERRNO1 fprintf(stderr, "Invalid source or target. Value range must be 0 to 63\n");
#define ERRNO2 fprintf(stderr, "Cycle detected. Invalid DAG edge.\n");

typedef struct
{
    uint64_t matrix[MATRIX_SIZE];
} StateMatrix;

/**
 * @brief Adds the edge dependency between vertexes
 * @param StateMatrix pointer
 * @param int source
 * @param int target
 * @return void
 */
void add_dependency(StateMatrix *sm, int source, int target);

/**
 * @brief Takes the current_state of the system, look up the specific adjacency mask 
 * for the trigger_node, and return the new global state by applying a single bitwise 
 * OR operation between the current state and the trigger node's mask.
 * @param StateMatrix pointer
 * @param uint64_t current_state
 * @param int trigger_node
 * @return uint64_t
 */
uint64_t get_blast_radius(StateMatrix *sm, uint64_t current_state, int trigger_node);

/**
 * @brief Resolves dependency cascade between vertexes
 * @param StateMatrix pointer
 * @param uint64_t initial_state
 * @return uint64_t
 */
uint64_t resolve_cascade(StateMatrix *sm, uint64_t initial_state);

#endif
