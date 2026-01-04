/* Code by Iván Naranjo Ortega */

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<ncurses.h>
#include<string.h>
#include<locale.h>
#include<ctype.h>

#define ANSI_COLOR "\x1b[42m\x1b[30m"
#define ANSI_COLOR_RESET "\x1b[0m"

int MAXX;
int MAXY;

static const char *MONTHS[12] ={
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};

static const int DAYSINMONTHS[12] ={
  31, // J
  28, // F
  31, // M
  30, // A
  31, // M
  30, // J
  31, // J
  31, // A
  30, // S
  31, // O
  30, // N
  31  // D
};

int CalculateFirstDay(struct tm *today){
  int daysSinceFirst = today->tm_mday - 1;
  int firstWeekDay = today->tm_wday - (daysSinceFirst % 7);

  /* Change format to Monday = 0 */
  firstWeekDay--;

  /* Wrap around if negative value */
  if (firstWeekDay < 0) firstWeekDay += 7;
  

  return firstWeekDay;
}

int GetDaysInMonth(struct tm *today){
    int dim = DAYSINMONTHS[today->tm_mon];
    if(today->tm_mon == 1){ 
        int year = today->tm_year + 1900;
        if ((year%4==0 && year%100!=0) || (year%400==0)) dim = 29;
    }
    return dim;
}


int PrintMonth(struct tm *today, int startY, int startX) {
  int y = startY;
  int x = startX;
  int selectedDay = today->tm_mday;

  // Print month name centered
  mvprintw(y++, x, "%s", MONTHS[today->tm_mon]);

  // Print day header
  mvprintw(y++, x, "%2s %2s %2s %2s %2s %2s %2s",
        "M","T","W","T","F","S","S");

  int firstWeekDay = CalculateFirstDay(today);
  int lastPrintedDay = 0;
  int dim = GetDaysInMonth(today);

  // Print first week
  for (int i = 0; i < 7; i++) {
    lastPrintedDay = i - firstWeekDay + 1;
    if (i < firstWeekDay) {
      mvprintw(y, x + i*3, "  ");
    } else {
      if (lastPrintedDay == selectedDay)
        attron(A_REVERSE);
      mvprintw(y, x + i*3, "%2d", lastPrintedDay);
      if (lastPrintedDay == selectedDay)
        attroff(A_REVERSE);
    }
  }
  y++;

  // Print remaining weeks
  while (lastPrintedDay < dim) {
    for (int i = 0; i < 7 && lastPrintedDay < dim; i++) {
      lastPrintedDay++;
      if (lastPrintedDay == selectedDay) attron(A_REVERSE);
      mvprintw(y, x + i*3, "%2d", lastPrintedDay);
      if (lastPrintedDay == selectedDay) attroff(A_REVERSE);
    }
    y++;
  }

  return y;
}

void PrintTodo(FILE *fp, char* dayFile){
  const int x = 32;
  const int todoSize = MAXX-x-6;
  int lastY = 2;
  int i;
  char buff[todoSize];

  /* Print top line */
  mvprintw(lastY,x,"+");
  for (i = 1; i<todoSize+2 ; ++i){
    mvprintw(lastY,x+i,"-");
  }
  mvprintw(lastY,x+i,"+");
  mvprintw(lastY,x+3,"File-\"%s\"",dayFile);
  lastY++;

  /* Print content */
  while(fgets(buff,sizeof(buff),fp) != NULL) {
    buff[strcspn(buff,"\n")] = '\0';
    mvprintw(lastY,x,"| %s ",buff);
    mvprintw(lastY++,x+todoSize+2," |");
  }

  /* Print bottom line */
  mvprintw(lastY,x,"+");
  for (i = 1; i<todoSize+2 ; ++i){
    mvprintw(lastY,x+i,"-");
  }
  mvprintw(lastY,x+i,"+");
  lastY++;
  fclose(fp);
}

int SelectDate(struct tm *selectTm){
  int selectValue, selectDaysInMonth;
  int returnValue = 0;
  size_t selectLen;
  char selectBuf[8];
  /* Choose year */
  clear();
  mvprintw(0,0,"Select year:");
  refresh();
  move(0, 13);
  clrtoeol();
  echo();
  curs_set(1);
  getnstr(selectBuf, 4);
  noecho();
  curs_set(0);

  selectLen = strlen(selectBuf);

  if (selectLen < 1 || selectLen > 4) returnValue = 1;

  for (size_t i = 0; i < selectLen; i++) {
    if (!isdigit((unsigned char)selectBuf[i])) returnValue = 1;
  }

  selectValue = atoi(selectBuf);
  selectValue -= 1900;
  if (selectValue < 0 || selectValue > 1100) returnValue = 1;
  mvprintw(1,0,"%d",selectValue+1900);
  selectTm->tm_year = selectValue;

  /* Choose month */
  mvprintw(2,0,"Select month:");
  refresh();
  move(2, 14);
  clrtoeol();
  echo();
  curs_set(1);
  getnstr(selectBuf, 2);
  noecho();
  curs_set(0);

  selectLen = strlen(selectBuf);

  if (selectLen < 1 || selectLen > 2) returnValue = 1;

  for (size_t i = 0; i < selectLen; i++) {
    if (!isdigit((unsigned char)selectBuf[i])) returnValue = 1;
  }

  selectValue = atoi(selectBuf);
  selectValue--;
  if(selectValue < 0 || selectValue > 11) returnValue = 1;
  mvprintw(3,0,"%d",selectValue+1);
  selectTm->tm_mon = selectValue;

  /* Choose day */
  mvprintw(4,0,"Select day:");
  refresh();
  move(4, 12);
  clrtoeol();
  echo();
  curs_set(1);
  getnstr(selectBuf, 2);
  noecho();
  curs_set(0);

  selectLen = strlen(selectBuf);

  if (selectLen < 1 || selectLen > 2) returnValue = 1;

  for (size_t i = 0; i < selectLen; i++) {
    if (!isdigit((unsigned char)selectBuf[i])) returnValue = 1;
  }

  selectValue = atoi(selectBuf);
  selectTm->tm_mday = selectValue;
  selectDaysInMonth = GetDaysInMonth(selectTm);
  if(selectValue < 0 || selectValue > selectDaysInMonth) returnValue = 1;
  mvprintw(5,0,"%d",selectValue);
  mktime(selectTm);

  /* Print result */
  if (returnValue == 0) {
    mvprintw(6,0,"Selected %d/%d/%d. Press any key to continue",
      selectTm->tm_year+1900,selectTm->tm_mon+1,selectTm->tm_mday);
  } else {
    mvprintw(6,0,"Introduced date is not allowed. Press any key to continue");
  }
  refresh();
  getch(); /* Blocking call */

  return returnValue;
}

int main() {
  time_t currentDate = time(NULL);
  struct tm t = *localtime(&currentDate);
  struct tm selectTm = *localtime(&currentDate);
  struct tm originalTm = *localtime(&currentDate);
  int dim = GetDaysInMonth(&t);
  int toClear = 0;
  int ch, lastY;
  char dayFile[64];

  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0); 
  getmaxyx(stdscr,MAXY,MAXX);
  
  if (MAXX > 100){
    MAXX = 100;
  }

  while(1){
    /* Print year */
    mvprintw(0, 0, "%d", t.tm_year + 1900);

    /* Print month */
    lastY = PrintMonth(&t, 1, 0);

    /* Open day file if existing */
    sprintf(dayFile, "notes/%d-%d-%d.txt",t.tm_year+1900,t.tm_mon+1,t.tm_mday);
    FILE* fp = fopen(dayFile,"r");
    if (fp == NULL){
      mvprintw(lastY+1,0,"%s missing ", dayFile);
    } else {
      toClear=1; 
      PrintTodo(fp,dayFile);
    }

    mvprintw(lastY+3,0,"Instructions:");
    mvprintw(lastY+4,2,"* Move with \'hjkl\'");
    mvprintw(lastY+5,2,"* Change month with \'pn\'");
    mvprintw(lastY+6,2,"* Select date with \'s\'");
    mvprintw(lastY+7,2,"* Select today with \'t\'");
    mvprintw(lastY+8,2,"* Exit with \'q\'");

    refresh();

    ch = getch(); /* Blocking call */
    if (ch == 'q') break;
    switch (ch) {
      case 'h':
        if(t.tm_mday>1){
          t.tm_mday--;
          mktime(&t);
        }
        break;
      case 'l':
        if(t.tm_mday<dim) {
          t.tm_mday++;
          mktime(&t);
        }
        break;
      case 'k':
        if(t.tm_mday>7) {
          t.tm_mday-=7;
          mktime(&t);
        }
        break;
      case 'j':
        if(t.tm_mday + 7 <= dim) {
          t.tm_mday+=7;
          mktime(&t);
        }
        break;
      case 'p':
        t.tm_mon--;
        if (t.tm_mon < 0) {
          t.tm_mon = 11;
          t.tm_year--;
        }
        mktime(&t);
        toClear=1;
        break;
      case 'n':
        t.tm_mon++;
        if (t.tm_mon >=12){
          t.tm_mon = 0;
          t.tm_year++;
        }
        mktime(&t);
        toClear=1;
        break;
      case 's':
        if(SelectDate(&selectTm) != 0) break;
        else {
          t = selectTm;
        }
        toClear = 1;
        break;
      case 't':
        t = originalTm;
        toClear = 1;
        break;
    }

    if (toClear == 1){
      clear();
      toClear=0;
    }
  }

  endwin();
  return 0;
}
