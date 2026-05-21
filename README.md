# Spatial-State Directed Acyclic Graph (SSDAG)

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-blue?style=for-the-badge)
![Version](https://img.shields.io/badge/version-1.1.0-blue?style=for-the-badge)

SSDAG is a highly specialized, domain-specific graph engine written in pure C. It replaces traditional struct-and-pointer-based Adjacency Lists with a **Bitwise Adjacency Matrix**, enabling instantaneous $O(1)$ state propagation and blast-radius calculations for systems with up to 64 interdependent nodes.

## 🚀 Why SSDAG?

Traditional graph traversals (like BFS/DFS) take $O(V + E)$ time and require scattered heap allocations, which can introduce unacceptable latency in high-throughput monitoring systems, alerting engines, or hardware state machines.

SSDAG solves this by mapping the graph directly to CPU registers using 64-bit integers.

### Key Features
* **Zero Dynamic Memory:** No `malloc` or `free`. The entire engine operates on the stack with a static 512-byte footprint.
* **$O(1)$ State Propagation:** Trigger downstream dependencies in a single CPU clock cycle using bitwise `OR` operations.
* **Equilibrium Resolution:** Multi-hop cascades are resolved mathematically without infinite loop vulnerabilities.
* **Cycle Prevention:** Native predictive cycle detection blocks illegal graph edges from being added at runtime.

---

## 🛠️ Build Instructions

Ensure you have a standard C compiler (e.g., `gcc` or `clang`) and `make` installed.

SSDAG is a lightweight, drop-in utility. Simply copy `SSDAG.h` and `SSDAG.c` into your project directory.

1. Clone the repository:
   `git clone git@github.com:valdal14/CD`
2. Navigate to the directory:
   `cd SSDAG`
3. Compile the binary:
   `make`
4. Execute the engine:
   `./ssdag_engine`

---

## 💻 Usage

1. Initialization and Wiring
Instantiate the StateMatrix and map your system's dependencies. The nodes are represented as integers from 0 to 63.

```c
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
```

2. State Propagation
Represent your current system state as a 64-bit integer bitmask. Use the engine to calculate cascades.

```c
// System State: Node 0 is triggered (Bit 0 is 1)
    uint64_t global_state = 1; 

    // Calculate a single hop ($O(1)$)
    uint64_t one_hop_state = get_blast_radius(&dag, global_state, 0);

    // Calculate the total multi-hop cascade to equilibrium 
    uint64_t final_blast_radius = resolve_cascade(&dag, global_state);

    printf("Final System State Mask: %llu\n", final_blast_radius);
    // Output: 39 (Nodes 0, 1, 2, and 5 are active)

    return 0;
}
```

---

## 🏗️ Architecture Under the Hood
Instead of linked lists, SSDAG uses a uint64_t matrix[64].

matrix[n] holds a 64-bit integer representing all outgoing edges for node n.

To propagate state, SSDAG utilizes hardware-level bitwise OR assignment (|=) and AND masking (&) to stack active nodes, finding the graph's equilibrium in microseconds.

---

## 📝 License
Distributed under the MIT License. See LICENSE for more information.
