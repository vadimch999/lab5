#include "Graph.h"

Node* createNode(Info* info, double weigh) {
    Node* List = (Node*) malloc(sizeof(Node));
    List->info = info;
    List->weight = weigh;
    return List;
}

Node* addNode(Node* node, Info* info, double weight) {
    if (!node)
        return createNode(info, weight);

    while(node->next)
        node = node->next;

    node->next = createNode(info, weight);

    return node->next;
}

Node* removeNode(Node* node, Node* toDel) {
    if (!node) {
        throwError("List is empty!");
        return NULL;
    }
    Node* prev = NULL;
    char flag = 0;

    while (node) {
        if (node == toDel) {
            flag = 1;
            break;
        }

        prev = node;
        node = node->next;
    }

    if (!flag) {
        throwError("There is no such node in list!");
        return NULL;
    }

    if (prev)
        prev->next = node->next;

    //  Очищение памяти происходит из Graph -> nodes

    return prev;
}

Graph* createGraph() {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->count = 0;
    graph->nodes = NULL;
    graph->adjList = NULL;
    return graph;
}

Node* addVert(Graph* graph, Info* info) {
    if (!graph) {
        throwError("Graph is not created!");
        return NULL;
    }

    graph->count++;

    if (graph->count == 1) {
        graph->nodes = (Info**) malloc(sizeof(Info*));
        graph->adjList = (Node**) malloc(sizeof(Node*));
    } else {
        graph->nodes = (Info**) realloc(graph->nodes, sizeof(Info*) * graph->count);
        graph->adjList = (Node**) realloc(graph->adjList, sizeof(Node*) * graph->count);
    }





}





double getWeight(Info* first, Info* second) {
    int xDist = first->x - second->x;
    int yDist = first->y - second->y;

    //  Добавить с CMake в конец файла строчку: target_link_libraries(lab5 m)
    return sqrt(xDist * xDist + yDist * yDist);
}

void throwError(char* error) {
    printf("\n%s%s%s\n\n", RED, error, WHT);
}