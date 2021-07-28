#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"


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
GraphNode* removeGNode(Node* node, Node* toDel);
GraphNode* findGNode(Node* node, Node* toFind);

Graph* createGraph();
Node* addVert(Graph* graph, Info* info);
Node* findVert(Graph* graph, char* name);
GraphNode* addEdge(Graph* graph, char* outcome, char* income);
double getWeight(Node* first, Node* second);
void throwError(char* error);

#endif
