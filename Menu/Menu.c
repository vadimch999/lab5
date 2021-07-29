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

Info* createInfo(char* name, int x, int y) {
    Info* info = (Info*) malloc(sizeof(Info));

    info->name = name;
    info->x = x;
    info->y = y;

    return info;
}

void menu() {
    Graph* graph = NULL;
    int option = 1;
    while (option) {
        printf(textMenu);
        printf("Choose option: ");
        option = getInt(0, 8);
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
//                readFromFile(&root);
                break;
            }
            case 8: {
//                writeToFile(root);
                break;
            }
            default: {
                deleteGraph(graph);
                option = 0;
                break;
            }
        }
    }
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


    printf("Enter coordinates\n");
    printf("\tx: ");
    int x = getInt(0, 0);
    printf("\ty: ");
    int y = getInt(0, 0);

    Info* info = createInfo(name, x, y);

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

    if ( addEdge(graph, out, in) )
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
        gNode = graph->adjList[i].next;

        if (!gNode) {
            printf("}\n");
            continue;
        }

        printf("\n");

        while (gNode) {
            printf("\tname: %s; weight: %f;\n", gNode->node->info->name, gNode->weight);
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

    if ( !removeVert(graph, name) ) {
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

    if ( removeEdg(graph, from, to) ) {
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
        removeList(node->next);
        free(node->info->name);
        free(node->info);
    }

    free(graph->adjList);
    free(graph);
}

void success() {
    printf("\n%sSuccess%s\n\n", GRN, WHT);
}