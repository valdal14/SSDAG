#include <stdio.h>
#include "SSDAG.h"

int main(void)
{
    // Zero-initialize the matrix on the stack
    StateMatrix dag = {0};

    // Node 0 triggers Node 1 and Node 2
    add_dependency(&dag, 0, 1);
    add_dependency(&dag, 0, 2);

    // Node 2 triggers Node 5
    add_dependency(&dag, 2, 5);
    
    // Cycle Prevention automatically blocks illegal wiring:
    // add_dependency(&dag, 5, 0); // Exits with CYCLE_ERROR
    
    return 0;
}
