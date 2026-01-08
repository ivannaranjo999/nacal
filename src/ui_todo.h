#ifndef TODO_H
#define TODO_H

void PrintRecurringTodo(int *lastY, FILE *fp, struct tm *today);
void PrintTodo(FILE *fp, FILE *fpRecurring, char* dayFile, struct tm *today);

#define VERTICAL_LINE "│"
#define LVERTICAL_DLINE "╞"
#define RVERTICAL_DLINE "╡"
#define HORIZONTAL_LINE "─"
#define HORIZONTAL_DLINE "═"
#define TL_CORNER "┌"
#define TR_CORNER "┐"
#define BL_CORNER "└"
#define BR_CORNER "┘"

#endif

