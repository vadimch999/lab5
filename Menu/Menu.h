#ifndef LAB5_MENU_H
#define LAB5_MENU_H

#include "../Graph/Graph.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define textMenu    "1. New empty graph\n"          \
                    "2. Add vertex\n"            \
                    "3. Add edge\n"          \
                    "4. Display graph\n"            \
                    "5. Remove vertex\n"      \
                    "6. Remove edge\n" \
                    "7. ...\n"       \
                    "8. ...\n"        \
                    "0. exit\n"                 \


//  Основная функция меню.
void menu();

//  Возвращает новую строку из пользовательского ввода.
char* getStr();

//  Создаёт структуру информации.
Info* createInfo(char* name, int x, int y);

void createNewGraph(Graph** graph);
void addNewVertex(Graph* graph);
void addNewEdge(Graph* graph);
void displayGraph(Graph* graph);
void removeVertex(Graph* graph);
void removeEdge(Graph* graph);

void deleteGraph(Graph* graph);

int getInt(int min, int max);
void success();

#endif
