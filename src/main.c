#include "common.h"

#include "config.h"
#include "calendar.h"
#include "ui_calendar.h"
#include "ui_todo.h"
#include "input.h"

int MAXX;
int MAXY;

void HelpPrint(char *filename){
  printf("Usage: %s path/to/folder\n",filename);
}

void OpenDayFile(FILE **fp, char *filename, const char *path, 
  const struct tm *t){

  char newFile[PATH_MAX];

  sprintf(newFile, "%s%d-%d-%d.txt",path,t->tm_year+1900,t->tm_mon+1,
    t->tm_mday);

  /* If its a new file, close previous and open new */
  if (strcmp(newFile,filename) != 0){
    if (*fp) {
      fclose(*fp);
      *fp = NULL;
    }

    strcpy(filename,newFile);
    *fp = fopen(filename,"r");
  }
}

int main(int argc, char *argv[]) {
  time_t currentDate = time(NULL);
  struct tm t = *localtime(&currentDate);
  struct tm selectTm = t;
  struct tm originalTm = t;

  int dim = GetDaysInMonth(&t);
  int toClear = 0;
  int ch, lastY;
  char dayFile[PATH_MAX] = "";
  char recurringFile[PATH_MAX] = "";
  char pathToNotes[PATH_MAX/2] = "";
  size_t len = 0;
  FILE *fpDay = NULL;

  if (argc == 2) {
    len = strlen(argv[1]);

    if (len + 2 > PATH_MAX/2){
      return 1;
    }

    strcpy(pathToNotes,argv[1]);

    if (len > 0 && pathToNotes[len-1] != '/'){
      pathToNotes[len] = '/';
      pathToNotes[len+1] = '\0';
    }

  }
  else {
    HelpPrint(argv[0]);
    return 2;
  }

  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  timeout(150);
  keypad(stdscr, TRUE);
  curs_set(0);
  getmaxyx(stdscr,MAXY,MAXX);
  if (MAXX > 100) MAXX = 100;

  sprintf(recurringFile, "%srecurring.txt",pathToNotes);
  FILE* fpRecurring = fopen(recurringFile,"r");

  while(1){
    mvprintw(0, 0, "%d", t.tm_year + 1900);
    lastY = PrintMonth(&t, 1, 0);

    OpenDayFile(&fpDay,dayFile,pathToNotes,&t);

    if (fpDay || fpRecurring ){
      toClear=1;
      PrintTodo(fpDay,fpRecurring,dayFile,&t);
    } else if (!fpDay && !fpRecurring){
      mvprintw(4,32,"Create %d-%d-%d.txt",
        t.tm_year+1900,t.tm_mon+1,t.tm_mday);
      mvprintw(5,32,"to open 'todo' window");
      toClear = 1;
    }

    lastY++;
    mvprintw(lastY++,0,"Instructions:");
    mvprintw(lastY++,2,"* Move with 'hjkl'");
    mvprintw(lastY++,2,"* Change month with 'pn'");
    mvprintw(lastY++,2,"* Select date with 's'");
    mvprintw(lastY++,2,"* Select today with 't'");
    mvprintw(lastY++,2,"* Exit with 'q'");

    wnoutrefresh(stdscr);
    doupdate();
    ch = getch();
    if (ch == 'q') break;

    switch (ch) {
      case KEY_LEFT_CODE: case 'h': if(t.tm_mday>1){t.tm_mday--; mktime(&t);} break;
      case KEY_RIGHT_CODE: case 'l': if(t.tm_mday<dim){t.tm_mday++; mktime(&t);} break;
      case KEY_UP_CODE: case 'k': if(t.tm_mday>7){t.tm_mday-=7; mktime(&t);} break;
      case KEY_DOWN_CODE: case 'j': if(t.tm_mday+7<=dim){t.tm_mday+=7; mktime(&t);} break;
      case 'p': t.tm_mday = 1 ; t.tm_mon--; if(t.tm_mon<0){t.tm_mon=11; t.tm_year--;} mktime(&t); dim = GetDaysInMonth(&t); toClear=1; break;
      case 'n': t.tm_mday = 1 ; t.tm_mon++; if(t.tm_mon>=12){t.tm_mon=0; t.tm_year++;} mktime(&t); dim = GetDaysInMonth(&t); toClear=1; break;
      case 's': timeout(-1); SelectDate(&selectTm);t=selectTm; dim = GetDaysInMonth(&t); toClear=1; timeout(50); break;
      case 't': t=originalTm; dim = GetDaysInMonth(&t); toClear=1; break;
      default: mvprintw(lastY++,2,"* Detected key press!%d",ch); break;
    
    }

    if (toClear){ erase(); toClear=0; }
  }

  if (fpRecurring) fclose(fpRecurring);
  if (fpDay) fclose(fpDay);

  endwin();
  return 0;
}
