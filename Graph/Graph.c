#include "Graph.h"

GraphNode* createGNode(Node* node, double weigh) {
    GraphNode* gNode = (GraphNode*) malloc(sizeof(GraphNode));
    gNode->node = node;
    gNode->weight = weigh;
    gNode->next = NULL;
    return gNode;
}

GraphNode* addGNode(Node* node, Node* toAdd) {
    if (!node || !toAdd) {
        throwError("Error with adding nodes!");
        return NULL;
    }

    double weight = getWeight(node, toAdd);
    GraphNode* gNode = node->next;

    if (!gNode) {
        node->next = createGNode(toAdd, weight);
        return node->next;
    }

    while (gNode->next) {
        gNode = gNode->next;
    }

    gNode->next = createGNode(toAdd, weight);

    return gNode->next;
}

Node* removeGNode(Node* node, Node* toDel) {
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
        return NULL;
    }

    if (gNode == node->next)
        node->next = gNode->next;

    if (prev)
        prev->next = gNode->next;

    free(gNode);

    return node;
}



void removeList(GraphNode* node) {
    if (node)
        removeList(node->next);
    else
        return;

    free(node);
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
    } while (gNode);

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
        graph->adjList = (Node*) malloc(sizeof(Node));
    else
        graph->adjList = (Node*) realloc(graph->adjList, sizeof(Node) * graph->count);

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

GraphNode* addEdge(Graph* graph, char* out, char* in) {
    Node* nodeOut = findVert(graph, out);
    if (!nodeOut) {
        throwError("There is no node with OUT name!");
        return NULL;
    }

    Node* nodeIn = findVert(graph, in);
    if (!nodeIn) {
        throwError("There is no node with IN name!");
        return NULL;
    }

    GraphNode* gNode = findGNode(nodeOut, nodeIn);


    if (gNode) {
        throwError("This edge is already exists!");
        return NULL;
    }

    gNode = addGNode(nodeOut, nodeIn);


    return gNode;
}

Node* removeVert(Graph* graph, char* name) {
    Node* toDel = findVert(graph, name);
    if (!toDel) {
        throwError("There is no such vertex!");
        return NULL;
    }

    Node* refNode = NULL;
    int index = -1;


    for (int i = 0; i < graph->count; i++) {
        refNode = &(graph->adjList[i]);

        if (refNode == toDel) {
            index = i;
            continue;
        }

        removeGNode(refNode, toDel);
    }

    removeList(toDel->next);
    free(toDel->info->name);
    free(toDel->info);

    if (index != graph->count - 1)
        graph->adjList[index] = graph->adjList[graph->count - 1];

    graph->count--;
    graph->adjList = (Node*) realloc(graph->adjList, sizeof(Node) * graph->count);

    return index != graph->count ? &(graph->adjList[index]) : &(graph->adjList[index-1]);
}

Node* removeEdg(Graph* graph, char* from, char* to) {
    Node* fromNode = findVert(graph, from);
    if (!fromNode) {
        throwError("There is no such FROM vertex!");
        return NULL;
    }

    Node* toNode = findVert(graph, to);
    if (!toNode) {
        throwError("There is no such TO vertex!");
        return NULL;
    }

    Node* gNode = removeGNode(fromNode, toNode);

    if (!gNode) {
        throwError("There is no such edge!");
        return NULL;
    }

    return fromNode;
}

double getWeight(Node* first, Node* second) {
    int xDist = first->info->x - second->info->x;
    int yDist = first->info->y - second->info->y;

    //  Добавить с CMake в конец файла строчку: target_link_libraries(lab5 m)
    return sqrt(xDist * xDist + yDist * yDist);
}

int countEdges(Node* node) {
    if (!node) {
        throwError("Node does`t exist!");
        return -1;
    }
    GraphNode* gNode = node->next;
    int count = 0;
    while (gNode) {
        count++;
        gNode = gNode->next;
    }
    return count;
}

int indexOfNode(Graph* graph, Node* node) {
    int index = (int) (node - graph->adjList);
    return index;
}

void bfs(Graph* graph, char* name, char* toFind) {
    Node* node = findVert(graph, name);
    if (!node) {
        throwError("Vertex doesn't exist!");
        return;
    }

    Node* nodeToFind = findVert(graph, toFind);
    if (!nodeToFind) {
        throwError("Vertex to find doesn't exist!");
        return;
    }

    if (!strcmp(name, toFind)) {
        throwError("These are the same node!");
        return;
    }

    int vertex = indexOfNode(graph, node);

    Queue* queue = createQueue(graph->count);
    int* visited = (int*) calloc(graph->count, sizeof(int));

    visited[vertex] = 1;
    enqueue(queue, vertex);
    GraphNode* temp;

    while (!isEmpty(queue)) {
//        printQueue(queue);
        int currentVertex = dequeue(queue);
        temp = graph->adjList[currentVertex].next;

        while (temp) {
            int adjVertex = indexOfNode(graph, temp->node);
            if ( !strcmp(temp->node->info->name, toFind) ) {
                printf("\n%s%s is reachable from %s%s\n\n", GRN, toFind, name, WHT);
                free(visited);
                removeQueue(queue);
                return;
            }

            if (visited[adjVertex] == 0) {
                visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
    free(visited);
    removeQueue(queue);
    printf("\n%s%s is NOT reachable from %s%s\n\n", RED, toFind, name, WHT);
}