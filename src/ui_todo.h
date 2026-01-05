#ifndef TODO_H
#define TODO_H

void PrintRecurringTodo(int *lastY, FILE *fp, struct tm *today);
void PrintTodo(FILE *fp, FILE *fpRecurring, char* dayFile, struct tm *today);

#endif

