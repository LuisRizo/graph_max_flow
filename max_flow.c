#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

/* 
 * Student Name: Luis Rizo
 * Class: Algorithms CS220 Fall 2018
 * Professor: Peter Brass
 * Function Signature:
 *  void maximum_flow(int n, int s, int t, int *cap, int *flow); 
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
    return Stack;
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
    int res;
    if (Stack->size > 0) {
        res = *(Stack->elements + (Stack->size - 1));
        Stack->size--;
        return res;
    } else {
        printf("[Stack Error] - Stack is empty to remove an element\n");
        exit(1);
    }
}

int peek(StackClass *Stack) {
    if (Stack->size > 0) {
        return *(Stack->elements + (Stack->size - 1));
    } else {
        printf("[Stack Error] - Stack is empty. Cannot peek\n");
        exit(1);
    }
}

// End Stack Definition

void maximum_flow(int n, int s, int t, int *cap, int *flow) {
    const int null = -1;
    StackClass *Stack = Stack_new(n * (n - 1));
    int * current_res_cap  = (int *) malloc( n * n * sizeof(int));
    int * current_flow     = (int *) malloc( n * n * sizeof(int));
    int * current_path     = (int *) malloc( n     * sizeof(int));
    int * visited          = (int *) malloc( n     * sizeof(int));
    int i, j, num_path, num_visited, current_vertex, cap_bottle_neck, tmp;

    // Make current flow matrix empty
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            *(current_flow + i * n + j) = 0;

    // Construct residual capacity graph
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            *(current_res_cap + i * n + j) = *(cap + i * n + j);
    // End condition is inside while loop
    while (1) {
        // Creating a path
        push(Stack, s);
        current_vertex = s;
        for (i = 0; i < n; i++) visited[i] = null;
        for (i = 0; i < n; i++) current_path[i] = null;
        num_visited = 0;
        num_path = 0;
        while (Stack->size != 0) {
            current_vertex = pop(Stack);
            // Prevent re-visiting.
            for (i = 0; i < num_visited; i++) {
                if (visited[i] == current_vertex) {
                    break; // Early end of loop.
                }
            }
            if (i != num_visited) { // Check if loop was ended using break;
                continue;
            }
            visited[num_visited++] = current_vertex;
            tmp = Stack->size; // Flag used to know if current_vertex has neighbors with residual capacity
            for (j = 0; j < n; j++) {
                if (*(current_res_cap + current_vertex * n + j) != 0) { // Only add neighbors that have residual capacity
                    if (j == t) {
                        // Early break, found path from s -> t
                        // Add current vertex to path
                        current_path[num_path++] = current_vertex;
                        // Add t to path
                        current_path[num_path++] = j;
                        break;
                    }
                    // Prevent re-visiting.
                    for (i = 0; i < num_visited; i++) {
                        if (visited[i] == j) {
                            break; // Early end of loop.
                        }
                    }
                    if (i != num_visited) { // Check if loop was ended using break;
                        continue;
                    } else {
                        // If valid neighbor, add to stack
                        push(Stack, j);
                    }
                }
            }
            if (j == t) {
                printf("Early exit %d\n", current_vertex);
                break;
            }
            if (tmp != Stack->size) { // If current vertex has neighbor with residual capacity, add current vertex to path
                current_path[num_path++] = current_vertex;
            } else {
                // If the path with the current vertex could not reach the target t, reset path until one that does.
                // Find last vertex in path that has an unvisited neighbor, which is the vector neighbor to the last item in the Stack.
                for (i = num_path - 1; i >= 0; i--) {
                    if (*(current_res_cap + current_path[i] * n + peek(Stack)) != 0) {
                        break;
                    } else {
                        // Clear element
                        current_path[i] = null;
                    }
                }
                num_path = i + 1;
            }
        }
        // No path was found
        if (Stack->size == 0) {
            break;
        }
        // Path found, now find capacity bottle neck
        cap_bottle_neck = -1;
        for (i = 0; i < num_path - 1; i++) {
            if (cap_bottle_neck == -1 || *(current_res_cap + current_path[i] * n + current_path[i+1]) < cap_bottle_neck)
                cap_bottle_neck = *(current_res_cap + current_path[i] * n + current_path[i+1]);
        }
        // Found smallest capacity, now add it to all the edges in the path
        for (i = 0; i < num_path - 1; i++) {
            // Update flow
            *(current_flow + current_path[i] * n + current_path[i+1]) = *(current_flow + current_path[i] * n + current_path[i+1]) + cap_bottle_neck;
            // Update residual capacity
            *(current_res_cap + current_path[i] * n + current_path[i+1]) = *(current_res_cap + current_path[i] * n + current_path[i+1]) - cap_bottle_neck;
        }
    }
    // Copy current_flow into flow
    for (i = 0; i < n; i ++)
        for (j = 0; j < n; j++)
            *(flow + i * n + j) = *(current_flow + i * n + j);
}

int test_stack() {
    int i, size = 1000;
    StackClass *Stack = Stack_new(size);
    printf("Initialized Stack\n");
    push(Stack, 50);
    assert(Stack->size == 1);
    printf("Inserted element Stack\n");

    // Assert pop in one element stack
    assert(pop(Stack) == 50);
    printf("Test 1 passed\n");
    assert(Stack->size == 0);
    printf("Test 2 passed\n");

    // Fill in Stack
    for(i = 0; i < size; i++) {
        push(Stack, i * i);
    }

    // Ensure peek only shows the last element;
    for(i = 0; i < size; i++) {
        assert(peek(Stack) == (size - 1) * (size - 1));
    }
    printf("Test 2 passed\n");

    // Ensure assert works for all the items
    for(i = 0; i < size; i++) {
        assert(pop(Stack) == (size - i - 1) * (size - i - 1));
    }
    printf("Test 3 passed\n");
    printf("All tests passed\n");
    return 0;
}

int main() {
    test_stack();
    int max = 1000;
  int cap[max][max], flow[max][max];
   int i,j, flowsum;
   for(i=0; i< max; i++)
     for( j =0; j< max; j++ )
       cap[i][j] = 0;
     
   for(i=0; i<499; i++)
     for( j=i+1; j<500; j++) 
       cap[i][j] = 2;
   for(i=1; i<500; i++)
     cap[i][500 + (i/2)] =4;
   for(i=500; i < 750; i++ )
   { cap[i][i-250]=3;
     cap[i][750] = 1;
     cap[i][751] = 1;
     cap[i][752] = 5;
   }
   cap[751][753] = 5;
   cap[752][753] = 5;
   cap[753][750] = 20;
   for( i=754; i< max - 1; i++)
   {  cap[753][i]=1;
      cap[i][500]=3;
      cap[i][498]=5;
      cap[i][1] = 100;
   }
   cap[900][max - 1] = 1;
   cap[910][max - 1] = 1;
   cap[920][max - 1] = 1;
   cap[930][max - 1] = 1;
   cap[940][max - 1] = 1;
   cap[950][max - 1] = 1;
   cap[960][max - 1] = 1;
   cap[970][max - 1] = 1;
   cap[980][max - 1] = 1;
   cap[990][max - 1] = 1;

    // Smaller version of graph
    // cap[0][1] = 20;
    // cap[0][2] = 5;
    // cap[2][3] = 5;
    // cap[3][4] = 5;
    // cap[4][5] = 5;
    // cap[5][8] = 4;
    // cap[1][6] = 3;
    // cap[1][7] = 12;
    // cap[6][8] = 3;
    // cap[7][8] = 10;

   printf("prepared capacity matrix, now executing maxflow code\n");
   maximum_flow(max,0,max - 1,&(cap[0][0]),&(flow[0][0]));
   for(i=0; i<=max - 1; i++)
     for(j=0; j<=max - 1; j++)
     {  if( flow[i][j] > cap[i][j] )
        {  printf("Capacity violated\n"); exit(0);}
     }    
   for(i=0; i<=max - 1; i++)
     for(j=0; j<=max - 1; j++)
     {  
         if( flow[i][j] < 0 )
        {  printf("Flow[%d][%d] = %d value negative\n", i, j, flow[i][j]); exit(0);}
     }    
   for(i=1; i<max - 1; i++)
   {  int inflow, outflow;
      inflow = 0;
      for( j=0; j <= max - 1; j++ )
         inflow += flow[j][i];
      outflow = 0;
      for( j=0; j <= max - 1; j++ )
         outflow += flow[i][j];
      if( inflow != outflow )
        {  printf("Flow conservation fails at vertex %d\n",i); exit(0);}
   }
   printf("Flow is a valid flow\n");
   flowsum = 0;
   for(i=0; i<=max - 1; i++)
     flowsum += flow[0][i];
   printf("Outflow of  0 is %d, should be 10\n", flowsum);
   flowsum = 0;
   for(i=0; i<=max - 1; i++)
     flowsum += flow[i][max - 1];
   printf("Inflow of 999 is %d, should be 10\n", flowsum);
       
   printf("End Test\n");

   return 0;
}