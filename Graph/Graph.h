#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H

#define RED "\e[0;31m"
#define GRN "\x1B[92m"
#define WHT "\033[0m"




#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Info {
    char* name;
    int x;
    int y;
} Info;

//  Узел
typedef struct Node {
    Info* info;
    struct GraphNode* next;
} Node;

typedef struct GraphNode {
    Node* node;
    double weight;
    struct GraphNode* next;
} GraphNode;

typedef struct Graph {
    int count;
    Node* adjList;
} Graph;



GraphNode* createGNode(Node* node, double weigh);
GraphNode* addGNode(Node* node, Node* toAdd);
Node* removeGNode(Node* node, Node* toDel);
GraphNode* findGNode(Node* node, Node* toFind);

Graph* createGraph();
Node* addVert(Graph* graph, Info* info);
Node* findVert(Graph* graph, char* name);
GraphNode* addEdge(Graph* graph, char* out, char* in);
double getWeight(Node* first, Node* second);
void removeList(GraphNode* node);
void throwError(char* error);
Node* removeVert(Graph* graph, char* name);
Node* removeEdg(Graph* graph, char* from, char* to);

#endif
