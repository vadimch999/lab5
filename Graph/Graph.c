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

bool bfs(Graph* graph, Node* node, Node* nodeToFind) {
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
            if ( !strcmp(temp->node->info->name, nodeToFind->info->name) ) {
                free(visited);
                removeQueue(queue);
                return true;
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
    return false;
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
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
    double* dist = (double*) malloc(sizeof(double) * V); // The output array.  dist[i] will hold the shortest
    // distance from src to i

    bool* sptSet = (bool*) malloc(sizeof(bool) * V); // sptSet[i] will be true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = DBL_MAX;   //  -1 means infinity since weight is positive
        sptSet[i] = false;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;
    GraphNode* distNode;

    // Find shortest path for all vertices
    for (int count = 0; count < V; count++) {

        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(V, dist, sptSet);


        // Mark the picked vertex as processed
        sptSet[u] = true;


        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++) {
            distNode = findGNode(graph->adjList + u, graph->adjList + v);
            if (src == v)
                dist[v] = 0;
            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            else if (!sptSet[v] && distNode && dist[u] != DBL_MAX
                && dist[u] + distNode->weight < dist[v])
                dist[v] = dist[u] + distNode->weight;
        }
    }

    // print the constructed distance array
//    for (int i = 0; i < V; i++)
//        printf("%d: %f\n", i, dist[i]);

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

//// Returns the maximum flow from s to t in the given graph
//int fordFulkerson(Graph* graph, int s, int t)
//{
//    int u, v;
//    int graph[V][V]
//    // Create a residual graph and fill the residual graph
//    // with given capacities in the original graph as
//    // residual capacities in residual graph
//    int rGraph[V][V]; // Residual graph where rGraph[i][j]
//    // indicates residual capacity of edge
//    // from i to j (if there is an edge. If
//    // rGraph[i][j] is 0, then there is not)
//    for (u = 0; u < V; u++)
//        for (v = 0; v < V; v++)
//            rGraph[u][v] = graph[u][v];
//
//    int parent[V]; // This array is filled by BFS and to
//    // store path
//
//    int max_flow = 0; // There is no flow initially
//
//    // Augment the flow while tere is path from source to
//    // sink
//    while (bfs(rGraph, s, t, parent)) {
//        // Find minimum residual capacity of the edges along
//        // the path filled by BFS. Or we can say find the
//        // maximum flow through the path found.
//        double path_flow = DBL_MAX;
//        for (v = t; v != s; v = parent[v]) {
//            u = parent[v];
//            path_flow = path_flow < rGraph[u][v] ? path_flow : rGraph[u][v];
//        }
//
//        // update residual capacities of the edges and
//        // reverse edges along the path
//        for (v = t; v != s; v = parent[v]) {
//            u = parent[v];
//            rGraph[u][v] -= path_flow;
//            rGraph[v][u] += path_flow;
//        }
//
//        // Add path flow to overall flow
//        max_flow += path_flow;
//    }
//
//    // Return the overall flow
//    return max_flow;
//}