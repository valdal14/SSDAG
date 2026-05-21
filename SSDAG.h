#ifndef SSDAG_H
#define SSDAG_H

#include <stddef.h>
#include <stdint.h>

#define MATRIX_SIZE 64
#define MIN_VAL 0
#define MAX_VAL 63
// Types
#define VALUE_ERROR 0
// Error Messages
#define ERRNO0 fprintf(stderr, "Unsupported type\n");
#define ERRNO1 fprintf(stderr, "Invalid source or target. Value range must be 0 to 63\n");

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

#endif
