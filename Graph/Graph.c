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
    GraphNode* gNode = node->list;

    if (!gNode) {
        node->list = createGNode(toAdd, weight);
        return node->list;
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

    GraphNode* gNode = node->list;

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

    if (gNode == node->list)
        node->list = gNode->next;

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


    if (!node->list)
        return NULL;

    GraphNode* gNode = node->list;
    Node* tmp;

    do {
        if (!strcmp(gNode->node->info->name, toFind->info->name))
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
    node.list = NULL;

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

int findIndexOfVert(Graph* graph, char* name) {
    if(!graph)
        return -1;

    for (int i = 0; i < graph->count; i++)
        if(!strcmp(graph->adjList[i].info->name, name))
            return i;

    return -1;
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

    removeList(toDel->list);
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

Info* createInfo(char* name, int x, int y) {
    Info* info = (Info*) malloc(sizeof(Info));

    info->name = name;
    info->x = x;
    info->y = y;

    return info;
}

double getWeight(Node* first, Node* second) {
   return 1;
}

int countEdges(Node* node) {
    if (!node) {
        throwError("Node does`t exist!");
        return -1;
    }
    GraphNode* gNode = node->list;
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

bool bfs(Graph* graph, Node* node, Node* nodeToFind, int* parent) {
    int vertex = indexOfNode(graph, node);

    Queue* queue = createQueue(graph->count);
    int* visited = (int*) calloc(graph->count, sizeof(int));

    visited[vertex] = 1;
    if (parent)
        parent[vertex] = -1;
    enqueue(queue, vertex);
    GraphNode* temp;

    while (!isEmpty(queue)) {
//        printQueue(queue);
        int currentVertex = dequeue(queue);
        temp = graph->adjList[currentVertex].list;

        while (temp) {
            int adjVertex = indexOfNode(graph, temp->node);
            if (visited[adjVertex] == 0) {
                if (parent)
                    parent[adjVertex] = currentVertex;

                if ( !strcmp(temp->node->info->name, nodeToFind->info->name) ) {
                    free(visited);
                    removeQueue(queue);
                    return true;
                }

                visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
    free(visited);
    removeQueue(queue);
    return false;
}

double dijkstra(Graph* graph, char* name, char* destName)
{
    Node* node = findVert(graph, name);
    if (!node) {
        throwError("There is no such node with start name!");
        return -1;
    }
    Node* destNode = findVert(graph, destName);
    if (!destNode) {
        throwError("There is no such node with destination name");
        return -1;
    }

    int destIndex = indexOfNode(graph, destNode);

    int src = indexOfNode(graph, node);
    int V = graph->count;
    double* dist = (double*) malloc(sizeof(double) * V);

    bool* sptSet = (bool*) malloc(sizeof(bool) * V);

    for (int i = 0; i < V; i++) {
        dist[i] = DBL_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;
    GraphNode* distNode;

    for (int count = 0; count < V; count++) {
        int u = minDistance(V, dist, sptSet);


        sptSet[u] = true;


        for (int v = 0; v < V; v++) {
            distNode = findGNode(graph->adjList + u, graph->adjList + v);
            if (src == v)
                dist[v] = 0;

            else if (!sptSet[v] && distNode && dist[u] != DBL_MAX
                && dist[u] + distNode->weight < dist[v])
                dist[v] = dist[u] + distNode->weight;
        }
    }

    double distance = dist[destIndex];

    free(sptSet);
    free(dist);
    return distance;
}

int minDistance(int V, double* dist, bool* sptSet)
{
    // Initialize min value
    double min = DBL_MAX;
    int min_index;
    bool flag = true;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] >= 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
            flag = false;
        }

    if (flag == true) {
        for (int v = 0; v < V; v++)
            if (sptSet[v] == false && dist[v] < 0) {
                min_index = v;
                break;
            }
    }

    return min_index;
}

double** cloneGraph(Graph* graph) {
    double** rGraph = (double**) malloc(sizeof(double*) * graph->count);
    for (int i = 0; i < graph->count; i++) {
        rGraph[i] = (double*) malloc(sizeof(double) * graph->count);
        for (int j = 0; j < graph->count; j++)
            rGraph[i][j] = 0;
    }


    Node* node;
    GraphNode* gNode;
    int index;

    for (int i = 0; i < graph->count; i++) {
        node = graph->adjList + i;
        gNode = node->list;
        while(gNode) {
            index = indexOfNode(graph, gNode->node);
            rGraph[i][index] = gNode->weight;
            gNode = gNode->next;
        }
    }

    return rGraph;
}

Graph* getRandomGraph(int count) {
    Graph* graph = createGraph();
    char* name;
    Info* info;
    for (int i = 0; i < count; i++) {
        name = (char*) malloc(sizeof(char) * 2);
        name[0] = (char) ( '0' + i );
        name[1] = '\0';

        info = createInfo(name, i, i);
        addVert(graph, info);
    }

    srand(time(NULL));

    Node* node;
    for (int i = 0; i < count; i++) {
        node = graph->adjList + i;
        for (int j = 0; j < count; j++) {
            //   Вероятность 25%
            if (rand() % 4 == 0)
                addGNode(node, graph->adjList + j);
        }
    }

    return graph;
}

double** fordFulkerson(Graph* graph, int s, int t)
{

    int u, v;

    // Create a residual graph and fill the residual graph
    // with given capacities in the original graph as
    // residual capacities in residual graph
    double** rGraph = cloneGraph(graph); // Residual graph where rGraph[i][j]
    // indicates residual capacity of edge
    // from i to j (if there is an edge. If
    // rGraph[i][j] is 0, then there is not)

    int* parent = (int*) malloc(sizeof(int) * graph->count); // This array is filled by BFS and to
    // store path

    double max_flow = 0; // There is no flow initially

    // Augment the flow while tere is path from source to
    // sink
    while (bfsForFordFalk(rGraph, s, t, graph->count, parent)) {
        // Find minimum residual capacity of the edges along
        // the path filled by BFS. Or we can say find the
        // maximum flow through the path found.

        double path_flow = DBL_MAX;

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = path_flow < rGraph[u][v] ? path_flow : rGraph[u][v];
        }

        // update residual capacities of the edges and
        // reverse edges along the path
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    free(parent);

    // Return the overall flow

    printf("\nMax flow: %lf\n\n", max_flow);

    return rGraph;
}

bool bfsForFordFalk(double** rGraph, int s, int t, int size, int* parent)
{
    int vertex = s;
    int toFind = t;

    Queue* queue = createQueue(size);
    bool* visited = (bool*) malloc(sizeof(bool) * size);
    memset(visited, false, size);

    visited[vertex] = 1;
    parent[vertex] = -1;
    enqueue(queue, vertex);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);

        for (int adjVertex = 0; adjVertex < size; ++adjVertex) {
            if (visited[adjVertex] == 0 && rGraph[currentVertex][adjVertex] > 0) {
                parent[adjVertex] = currentVertex;

                if ( adjVertex == toFind ) {
                    free(visited);
                    removeQueue(queue);
                    return true;
                }

                visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
        }
    }
    free(visited);
    removeQueue(queue);
    return false;
}

Graph* find_minimum_spanning_tree(Graph *graph) {
    GraphNode *current_graph_node;
    int index;
    bool *used_vertices = (bool *) malloc(graph->count * sizeof(bool));
    int *parent_vertices = (int *) malloc(graph->count * sizeof(int));

    for (int i = 0; i < graph->count; i++) {
        used_vertices[i] = false;
        parent_vertices[i] = -1;
    }

    used_vertices[0] = true;
    parent_vertices[0] = 0;

    for (int i = 0; i < graph->count; i++) {
        current_graph_node = graph->adjList[i].list;

        while (current_graph_node != NULL) {
            index = indexOfNode(graph, current_graph_node->node);

            if (used_vertices[index] == false) {
                used_vertices[index] = true;
                parent_vertices[index] = i;
            }

            current_graph_node = current_graph_node->next;
        }
    }
    Graph *spinning_tree = createGraph();
    for (int i = 0; i < graph->count; i++) {
        Info *info = graph->adjList[i].info;
        addVert(spinning_tree, info);
    }

    for (int i = 0; i < graph->count; i++)
        addEdge(spinning_tree, spinning_tree->adjList[parent_vertices[i]].info->name, spinning_tree->adjList[i].info->name);

    free(used_vertices);
    free(parent_vertices);

    return spinning_tree;
}