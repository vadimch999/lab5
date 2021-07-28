#include "Graph.h"

GraphNode* createGNode(Node* node, double weigh) {
    GraphNode* gNode = (GraphNode*) malloc(sizeof(GraphNode));
    gNode->node = node;
    gNode->weight = weigh;
    gNode->next = NULL;
    return gNode;
}

GraphNode* addGNode(Node* node, Node* toAdd) {
    double weight = getWeight(node, toAdd);
    if (!node || !toAdd) {
        throwError("Error with adding nodes!");
        return NULL;
    }

    GraphNode* gNode = node->next;
    //  Изменить всё с учётом GraphNode
    if (!gNode) {
        node->next = createGNode(node, weight);
    }


    while (!gNode->next) {
        gNode = gNode->next;
    }

    gNode->next = createGNode(node, weight);

    return gNode->next;
}

GraphNode* removeGNode(Node* node, Node* toDel) {
    if (!node) {
        throwError("List is empty!");
        return NULL;
    }

    GraphNode* gNode = node->next;

    GraphNode* prev = NULL;
    char flag = 0;

    while (gNode) {
        if (gNode->node == toDel) {
            flag = 1;
            break;
        }

        prev = gNode;
        gNode = gNode->next;
    }

    if (!flag) {
        throwError("There is no such node in list!");
        return NULL;
    }

    if (gNode == node->next)
        node->next = gNode->next;

    if (prev)
        prev->next = gNode->next;


    //  Очищение памяти происходит из Graph -> nodes

    return prev;
}

GraphNode* findGNode(Node* node, Node* toFind) {
    if(!node) {
        throwError("Graph is empty!");
        return NULL;
    }

    if (!node->next)
        return NULL;

    GraphNode* gNode = node->next;

    do {
        if (gNode->node == toFind)
            return gNode;
        gNode = gNode->next;
    } while (gNode->next);

    return NULL;
}

Graph* createGraph() {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->count = 0;
    graph->adjList = NULL;
    return graph;
}

Node* addVert(Graph* graph, Info* info) {
    if (!graph) {
        throwError("Graph is not created!");
        return NULL;
    }

    graph->count++;

    if (graph->count == 1)
        graph->adjList = (Node**) malloc(sizeof(Node*));
    else
        graph->adjList = (Node**) realloc(graph->adjList, sizeof(Node*) * graph->count);

    Node node;
    node.info = info;
    node.next = NULL;

    graph->adjList[graph->count - 1] = node;

    return &(graph->adjList[graph->count - 1]);
}

Node* findVert(Graph* graph, char* name) {
    if(!graph)
        return NULL;

    for (int i = 0; i < graph->count; i++)
        if(!strcmp(graph->adjList[i].info->name, name))
            return &(graph->adjList[i]);

    return NULL;
}

GraphNode* addEdge(Graph* graph, char* outcome, char* income) {
    Node* nodeOut = findVert(graph, outcome);
    if (!nodeOut) {
        throwError("There is no node with outcome name!");
        return NULL;
    }

    Node* nodeIn = findVert(graph, income);
    if (!nodeIn) {
        throwError("There is no node with income name!");
        return NULL;
    }

    GraphNode* gNode = findGNode(nodeOut, nodeIn);

    if (!gNode) {
        throwError("This edge is already exists!");
        return gNode;
    }

    gNode = addGNode(nodeOut, nodeIn);

    return gNode;
}

Node* removeVert() {

}


double getWeight(Node* first, Node* second) {
    int xDist = first->info->x - second->info->x;
    int yDist = first->info->y - second->info->y;

    //  Добавить с CMake в конец файла строчку: target_link_libraries(lab5 m)
    return sqrt(xDist * xDist + yDist * yDist);
}

void throwError(char* error) {
    printf("\n%s%s%s\n\n", RED, error, WHT);
}