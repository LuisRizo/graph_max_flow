#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/* 
 * Student Name: Luis Rizo
 * Class: Algorithms CS220 Fall 2018
 * Professor: Peter Brass
 * Function Signature:
 *  void maximum flow(int n, int s, int t, int *cap, int *flow); 
 * Variables:
 * - n: the number of vertices of the graph,
 * - s: the start vertex,
 * - t: the target vertex
 * - cap: the matrix of edge capacities.
 * - flow: the matrix used to return the maximum flow
 * Instructions:
 * Write code that finds a maximum flow in a directed graph, using the Ford-Fulkerson
 *  algorithm. The graph is given as adjacency matrix with cap[i][j] being the capacity
 *  of the directed edge from i to j. The function received another argument, the matrix
 *  flow[][], which is used to return the flow values in the maximum flow from s to t
 */

// Definition of a Stack
typedef struct{
    int size;
    int cap;
    int * elements;
} StackClass;

StackClass* Stack_new(int n) {
    StackClass * Stack = malloc(sizeof(StackClass));
    Stack->elements = (int *) malloc (n * sizeof(int));
    Stack->size     = 0;
    Stack->cap      = n;
}

void push(StackClass *Stack, int element) {
    if (Stack->size == Stack->cap) {
        printf("[Stack Error] - Stack is full to insert %d\n", element);
        exit(1);
    } else {
        *(Stack->elements + Stack->size) = element;
        Stack->size++;
    }
}

int pop(StackClass *Stack) {
    int res, i;
    if (Stack->size > 0) {
        res = *(Stack->elements);
        Stack->size--;
        for (i = Stack->size - 1; i >= 0; i--) *(Stack->elements + i) = *(Stack->elements + i + 1);
        return res;
    } else {
        printf("[Stack Error] - Stack is empty to remove an element\n");
        exit(1);
    }
}

int peek(StackClass *Stack) {
    if (Stack->size > 0) {
        return *(Stack->elements);
    } else {
        printf("[Stack Error] - Stack is empty. Cannot peek\n");
        exit(1);
    }
}

// End Stack Definition

void maximum_flow(int n, int s, int t, int *cap, int *flow) {
    int * current_flow = (int *) malloc( n * n * sizeof(int));
    int * current_cap  = (int *) malloc( n * n * sizeof(int));
    int * visited      = (int *) malloc( n     * sizeof(int));
}

int test_stack() {
    int i;
    StackClass *Stack = Stack_new(1000);
    push(Stack, 50);

    // Assert pop in one element stack
    assert(pop(Stack) == 50);
    printf("Test 1\n");

    // Fill in Stack
    for(i = 0; i < 1000; i++) {
        push(Stack, i * i);
    }

    // Ensure peek only shows the first element;
    for(i = 0; i < 1000; i++) {
        assert(peek(Stack) == 0);
    }
    printf("Test 2 passed\n");

    // Ensure assert works for all the items
    for(i = 0; i < 1000; i++) {
        assert(pop(Stack) == i * i);
    }
    printf("Test 3 passed\n");
    printf("All tests passed\n");
    return 0;
}

int main() {
    test_stack();

    return 0;
}