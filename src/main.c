#include "common.h"

#include "config.h"
#include "calendar.h"
#include "ui_calendar.h"
#include "ui_todo.h"
#include "input.h"

int MAXX;
int MAXY;

int main(int argc, char *argv[]) {
  time_t currentDate = time(NULL);
  struct tm t = *localtime(&currentDate);
  struct tm selectTm = t;
  struct tm originalTm = t;

  int dim = GetDaysInMonth(&t);
  int toClear = 0;
  int ch, lastY;
  char dayFile[256];
  char recurringFile[256];
  char pathToNotes[128]="";

  if (argc == 2) strcpy(pathToNotes,argv[1]);
  else if (argc > 2) return 0;

  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  getmaxyx(stdscr,MAXY,MAXX);
  if (MAXX > 100) MAXX = 100;

  while(1){
    mvprintw(0, 0, "%d", t.tm_year + 1900);
    lastY = PrintMonth(&t, 1, 0);

    sprintf(dayFile, "%s%d-%d-%d.txt",pathToNotes,t.tm_year+1900,t.tm_mon+1,t.tm_mday);
    FILE* fp = fopen(dayFile,"r");
    sprintf(recurringFile, "%srecurring.txt",pathToNotes);
    FILE* fpRecurring = fopen(recurringFile,"r");

    if (fp != NULL || fpRecurring != NULL){
      toClear=1;
      PrintTodo(fp,fpRecurring,dayFile,&t);
    }

    lastY++;
    mvprintw(lastY++,0,"Instructions:");
    mvprintw(lastY++,2,"* Move with 'hjkl'");
    mvprintw(lastY++,2,"* Change month with 'pn'");
    mvprintw(lastY++,2,"* Select date with 's'");
    mvprintw(lastY++,2,"* Select today with 't'");
    mvprintw(lastY++,2,"* Exit with 'q'");

    refresh();
    ch = getch();
    if (ch == 'q') break;

    switch (ch) {
      case 'h': if(t.tm_mday>1){t.tm_mday--; mktime(&t);} break;
      case 'l': if(t.tm_mday<dim){t.tm_mday++; mktime(&t);} break;
      case 'k': if(t.tm_mday>7){t.tm_mday-=7; mktime(&t);} break;
      case 'j': if(t.tm_mday+7<=dim){t.tm_mday+=7; mktime(&t);} break;
      case 'p': t.tm_mon--; if(t.tm_mon<0){t.tm_mon=11; t.tm_year--;} mktime(&t); toClear=1; break;
      case 'n': t.tm_mon++; if(t.tm_mon>=12){t.tm_mon=0; t.tm_year++;} mktime(&t); toClear=1; break;
      case 's': if(SelectDate(&selectTm)==0){t=selectTm; toClear=1;} break;
      case 't': t=originalTm; toClear=1; break;
    }

    if (toClear){ clear(); toClear=0; }
  }

  endwin();
  return 0;
}
