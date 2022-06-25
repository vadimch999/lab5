#include "Menu.h"

char* getStr() {
    char* ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0)
            scanf("%*c");
        else {
            len += (int) strlen(buf);
            ptr = (char *)realloc(ptr, len + 1);
            strcat(ptr, buf);
        }
    } while (n > 0);
    return ptr;
}

void menu() {
    // Graph* graph = readFromFile(LOAD_PATH);
    int option = 1;
    Graph* graph = createGraph();
    // if (graph)
    //    successMessage("Graph is loaded");

    while (option) {
        printf(textMenu);
        printf("Choose option: ");
        option = getInt(0, 12);
        switch (option) {
            case 1: {
                createNewGraph(&graph);
                break;
            }
            case 2: {
                addNewVertex(graph);
                break;
            }
            case 3: {
                addNewEdge(graph);
                break;
            }
            case 4: {
                displayGraph(graph);
                break;
            }
            case 5: {
                removeVertex(graph);
                break;
            }
            case 6: {
                removeEdge(graph);
                break;
            }
            case 7: {
                startBFS(graph);
                break;
            }
            case 8: {
                dijkstraAlg(graph);
                break;
            }
            case 9: {
                applyMST(graph);
                break;
            }
          /*  case 10: {
                randomGraph(&graph);
                break;
            } */
         /*   case 11: {
                timing();
                break;
            } */
            default: {
            /*    writeToFile(graph, LOAD_PATH);
                successMessage("Graph is saved"); */
                deleteGraph(graph);
                option = 0;
                break;
            }
        }
    }
}

void applyMST(Graph *graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    success();
    find_minimum_spanning_tree(graph);
    printf("1111");
    displayGraph(find_minimum_spanning_tree(graph));
}

int getInt(int min, int max) {
    int result = 0;
    if (min == 0 && max == 0) {
        scanf("%d", &result);
    } else {
        int flag = 1;
        do {
            scanf("%d", &result);
            if (result > max || result < min)
                printf("Error! Enter number from %d to %d: ", min, max);
            else
                flag = 0;
        } while(flag);
    }
    getchar();
    return result;
}

void createNewGraph(Graph** graph) {
    if (*graph) {
        printf("The existed graph will be erased. Continue?\n");
        printf("0. Yes\n1. No\n");
        printf("Choose option: ");
        int option = getInt(0, 1);
        if (option)
            return;
        else {
            deleteGraph(*graph);
            *graph = NULL;
        }
    }

    *graph = createGraph();
    success();
}

void addNewVertex(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    printf("Enter name of the vertex: ");
    char* name = getStr();

    if (findVert(graph, name)) {
        throwError("Vertex with this name is already exists!");
        return;
    }


    //printf("Enter coordinates\n");
    // printf("\tx: ");
    // int x = getInt(0, 0);
    // printf("\ty: ");
    // int y = getInt(0, 0);

    Info* info = createInfo(name, 1, 1);

    if ( addVert(graph, info) )
        success();
}

void addNewEdge(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    printf("Enter name of the vertex for OUT node: ");
    char* out = getStr();
    printf("Enter name of the vertex for IN node: ");
    char* in = getStr();

    if ( addEdge(graph, out, in) && addEdge(graph, in, out))
        success();

    free(out);
    free(in);
}

void displayGraph(Graph* graph) {
    if (!graph || !graph->count) {
        throwError("Graph is empty!");
        return;
    }

    GraphNode* gNode;

    printf("\n");
    for (int i = 0; i < graph->count; i++) {
        printf("Name: %s : { ", graph->adjList[i].info->name);
        gNode = graph->adjList[i].list;

        if (!gNode) {
            printf("}\n");
            continue;
        }

        printf("\n");

        while (gNode) {
            printf("\tname: %s;\n", gNode->node->info->name);
            gNode = gNode->next;
        }

        printf("}\n");
    }

    printf("\n");
}

void removeVertex(Graph* graph) {
    if (!graph || !graph->count) {
        throwError("Graph is empty!");
        return;
    }

    printf("Enter the name of the vertex: ");
    char* name = getStr();

    if ( !(graph, name) ) {
        free(name);
        return;
    }

    free(name);
    success();
}

void removeEdge(Graph* graph) {
    if (!graph || !graph->count) {
        throwError("Graph is empty!");
        return;
    }

    printf("Enter the name of the FROM vertex: ");
    char* from = getStr();

    printf("Enter the name of the TO vertex: ");
    char* to = getStr();

    if ( removeEdg(graph, from, to) && removeEdg(graph, to, from)) {
        free(from);
        free(to);
        success();
        return;
    }

    free(from);
    free(to);
}



void deleteGraph(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }

    Node* node;

    for (int i = 0; i < graph->count; i++) {
        node = &(graph->adjList[i]);
        removeList(node->list);
        free(node->info->name);
        free(node->info);
    }

    free(graph->adjList);
    free(graph);
}

/* Graph* readFromFile(char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp)
        return NULL;

    Graph* graph = createGraph();

    int count;
    fread(&count, sizeof(int), 1, fp);

    int length;
    char* name;
    Info* info;
    int x, y;

    for (int i = 0; i < count; i++) {
        fread(&length, sizeof(int), 1, fp);
        name = (char*) malloc(sizeof(char) * length);
        fread(name, sizeof(char), length, fp);
        fread(&x, sizeof(int), 1, fp);
        fread(&y, sizeof(int), 1, fp);

        info = createInfo(name, x, y);
        if ( !addVert(graph, info) ) {
            throwError("Unexpected error!");
            exit(1);
        }
    }

    GraphNode* gNode;
    int index;
    int edgeCount;
    for (int i = 0; i < count; i++) {
        fread(&edgeCount, sizeof(int), 1, fp);
        while (edgeCount--) {
            fread(&index, sizeof(int), 1, fp);
            gNode = addEdge(
                    graph,
                    graph->adjList[i].info->name,
                    graph->adjList[index].info->name
                    );
            if (!gNode) {
                throwError("Unexpected error!");
                exit(1);
            }
        }
    }
    return graph;
}

 void readGraph(Graph** graph) {
    if (*graph) {
        printf("The existed graph will be erased. Continue?\n");
        printf("0. Yes\n1. No\n");
        printf("Choose option: ");
        int option = getInt(0, 1);
        if (option)
            return;
        else {
            deleteGraph(*graph);
            *graph = NULL;
        }
    }

    printf("Enter file path: ");
    char* path = getStr();

    *graph = readFromFile(path);

    free(path);

    if (*graph)
        success();
    else
        throwError("Cannot open that file!");
}

Graph* writeToFile(Graph* graph, char* path) {
    FILE* fp = fopen(path, "wb");
    if (!fp) {
        throwError("Cannot open that file!");
        return NULL;
    }

    int count = graph->count;
    fwrite(&(count), sizeof(int), 1, fp);

    int length;

    for (int i = 0; i < count; i++) {
        length = (int) strlen(graph->adjList[i].info->name) + 1;
        fwrite( &length, sizeof(int), 1, fp);
        fwrite( graph->adjList[i].info->name,
                sizeof(char), length, fp);
        fwrite( &(graph->adjList[i].info->x),
                sizeof(int), 1, fp);
        fwrite( &(graph->adjList[i].info->y),
                sizeof(int), 1, fp);
    }

    int edgeCount;
    int index;
    GraphNode* gNode;

    for (int i = 0; i < count; i++) {
        edgeCount = countEdges(graph->adjList + i);
        if (!edgeCount)
            continue;

        fwrite(&edgeCount, sizeof(int), 1, fp);
        gNode = graph->adjList[i].list;
        for (int j = 0; j < edgeCount; j++) {
            index = indexOfNode(graph, gNode->node);
            fwrite(&index, sizeof(int), 1, fp);
            gNode = gNode->next;
        }
    }

    fclose(fp);
    return graph;
}

void writeGraph(Graph* graph) {
    if (!graph || !graph->count) {
        throwError("Graph is empty!");
        return;
    }

    printf("Enter file path: ");
    char* path = getStr();

    if ( writeToFile(graph, path) )
        success();

    free(path);
} */

void startBFS(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    printf("Enter name of the vertex to start finding from: ");
    char* name = getStr();
    printf("Enter name of the vertex to find: ");
    char* toFind = getStr();

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

    bool result = bfs(graph, node, nodeToFind, NULL);

    if (result == true)
        printf("\n%s%s is reachable from %s%s\n\n", GRN, toFind, name, WHT);
    else
        printf("\n%s%s is NOT reachable from %s%s\n\n", RED, toFind, name, WHT);



    free(name);
    free(toFind);
}

void dijkstraAlg(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    printf("Enter node name to start from: ");
    char* name = getStr();
    printf("Enter destination node name: ");
    char* destName = getStr();
    double distance = dijkstra(graph, name, destName);
    free(name);
    free(destName);

    if (distance < 0)
        return;
    else if (distance == DBL_MAX) {
        throwError("Infinity");
    } else {
        printf("\nThe shortest path distance is %f\n\n", distance);
    }
}

/* void randomGraph(Graph** graph) {
    if (*graph) {
        printf("The existed graph will be erased. Continue?\n");
        printf("0. Yes\n1. No\n");
        printf("Choose option: ");
        int option = getInt(0, 1);
        if (option)
            return;
        else {
            deleteGraph(*graph);
            *graph = NULL;
        }
    }

    printf("Enter count (max %d): ", RANDOM_MAX_COUNT);
    int count = getInt(0, RANDOM_MAX_COUNT);
    *graph = getRandomGraph(count);

    if (*graph)
        success();
} */

void timing() {
    int i, count;
    Node* root = NULL;
    clock_t begin, end;

    printf("Enter how many vertices do you want to test (max %d): ", TIMING_MAX_COUNT);
    count = getInt(0, TIMING_MAX_COUNT);

    Graph* graph = createGraph();
    char* name;
    for (int i = 0; i < count; i++) {

    }

    Info** infoArr = (Info**) malloc(sizeof(Info*) * count);

    //  Выделение памяти под строки.
    printf("Memory allocation...\n");
    for (i = 0; i < count; i++) {
        name = (char*) malloc(sizeof(char) * 3);
        name[0] = (char) ( i % 127 + 1 );
        name[1] = (char) ( i / 127 > 0 ? i % 127 + 1 : 1);
        name[2] = '\0';

        infoArr[i] = createInfo(name, i, i);
    }
    printf("Memory was successfully allocated\n\n");

    // Таймирование функции вставки вершины

    printf("Vertex adding...\n");
    begin = clock();

    for (i = 0; i < count; i++)
        addVert(graph, infoArr[i]);

    end = clock();

    printf("Time of vertex adding: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC );

    // Таймирование функции добавления ребра из каждой вершины в каждую
    printf("Edge adding...\n");
    begin = clock();

    for (i = 0; i < count; i++)
        for (int j = 0; j < count; j++)
            addEdge(graph, infoArr[i]->name, infoArr[j]->name);

    end = clock();

    printf("Time of edge adding: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC );

    // Таймирование функции поиска элемента в дереве.
    printf("Vertex finding...\n");
    begin = clock();

    for (i = 0; i < count; i++)
        findVert(graph, infoArr[i]->name);

    end = clock();

    printf("Time of vertex finding: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);

    // Таймирование BFS.
    printf("Breadth first search testing...\n");
    begin = clock();

    for (i = 0; i < count; i++)
        for (int j = 0; j < count; j++)
            bfs(graph, graph->adjList + i, graph->adjList + j, NULL);

    end = clock();

    printf("Time of BFS: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);

    // Таймирование Dijkstra.
    printf("Dijkstra testing...\n");
    begin = clock();

    for (i = 0; i < count; i++)
        for (int j = 0; j < count; j++)
            dijkstra(graph, infoArr[i]->name, infoArr[j]->name);

    end = clock();

    printf("Time of Dijkstra: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);

    // Таймирование функции удаления графа.
    printf("Graph deleting...\n");
    begin = clock();
    deleteGraph(graph);
    end = clock();

    printf("Time of element deleting: %f * 10^-3\n\n", (double)(end - begin) * 1000 / CLOCKS_PER_SEC);
    free(infoArr);
}

/* void flowNetwork(Graph* graph) {
    if (!graph) {
        throwError("Graph is empty!");
        return;
    }
    printf("Enter name of the vertex to start finding from: ");
    char* name = getStr();
    printf("Enter name of the vertex to find: ");
    char* toFind = getStr();

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

    double** result = fordFulkerson(graph, indexOfNode(graph, node), indexOfNode(graph, nodeToFind));

    for (int i = 0; i < graph->count; ++i) {
        printf("\"%.6s\"  ", (graph->adjList[i]).info->name) ;
    }
    printf("\n");

    for (int i = 0; i < graph->count; ++i) {
        for (int j = 0; j < graph->count; ++j) {
            printf("%lf ", result[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < graph->count; ++i) {
        free(result[i]);
    }
    free(result);
} */