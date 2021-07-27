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

//  Узел списка
typedef struct Node {
    Info* info;
    double weight;
    struct Node* next;
} Node;

typedef struct Graph {
    int count;
    Info** nodes;
    Node** adjList;
} Graph;



Node* createNode(Info* info, double weigh);
Node* addNode(Node* node, Info* info, double weight);
Node* removeNode(Node* node, Node* toDel);
double getWeight(Info* first, Info* second);
void throwError(char* error);

#endif
