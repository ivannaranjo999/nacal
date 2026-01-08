#include "common.h"
#include "ui_todo.h"
#include "config.h"

int isNumber(const char *s) {
  if (*s == '\0') return 0;
  for (; *s; s++) {
    if(!isdigit((unsigned char)*s)) return 0;
  }
  return 1;
}

void PrintRecurringTodo(int *lastY, FILE *fp, struct tm *today) {
  const int x = 32;
  const size_t todoSize = MAXX-x-6;
  char buff[todoSize];
  char cronDay[16], cronMonth[16], cronWeekday[16];
  int cronDayV = -1;
  int cronMonthV = -1;
  int cronWeekdayV = -1;

  while(fgets(buff,sizeof(buff),fp) != NULL) {
    /* Parse special conditions */
    if (buff[0] == '$' && 
      isspace((unsigned char)buff[1]) && 
      sscanf(buff, "$%15s %15s %15s", cronDay, cronMonth, cronWeekday) == 3)  {
      cronDayV = -1;
      cronMonthV = -1;
      cronWeekdayV = -1;

      if (isNumber(cronDay)) {
        /* Create condition for next line print */
        cronDayV = atoi(cronDay);
        if (cronDayV < 1 || cronDayV > 31) cronDayV = -1;
      }
      if (isNumber(cronMonth)) {
        /* Create condition for next line print */
        cronMonthV = atoi(cronMonth);
        if (cronMonthV < 0 || cronMonthV > 11) cronMonthV = -1;
        else cronMonthV--;
      }
      if (isNumber(cronWeekday)) {
        /* Create condition for next line print */
        cronWeekdayV = atoi(cronWeekday);
        if (cronWeekdayV < 0 || cronWeekdayV > 6) cronWeekdayV = -1;
      }

    } else {
    /* Apply conditions to print */
    if ((today->tm_mday == cronDayV     || cronDayV     == -1) && 
        (today->tm_mon  == cronMonthV   || cronMonthV   == -1) &&
        (today->tm_wday == cronWeekdayV || cronWeekdayV == -1)) {
      buff[strcspn(buff,"\n")] = '\0';
      mvprintw(*lastY,x,"%s %s ",VERTICAL_LINE,buff);
      mvprintw(*lastY,x+todoSize+2,"%s",VERTICAL_LINE);
      *lastY = *lastY + 1;
      }
    }
  }
}

void PrintTodo(FILE *fp, FILE *fpRecurring, char* dayFile, struct tm *today){
  const int x = 32;
  const size_t todoSize = MAXX-x-6;
  const size_t fileHeaderSize = todoSize/2;
  size_t i;
  int lastY = 2;
  char buff[todoSize];
  char headerName[fileHeaderSize];

  memset (headerName,0,sizeof(headerName));
  if (strlen(dayFile) > fileHeaderSize) {
    for (int j = 0; j < 3; ++j){
      headerName[j]='.';
    }
    memcpy(&headerName[3], dayFile + strlen(dayFile) - fileHeaderSize+3, sizeof(headerName));
  } else {
    memcpy(headerName, dayFile, sizeof(headerName));
  }

  mvprintw(lastY,x,"%s",TL_CORNER);
  for (i = 1; i<todoSize+2 ; ++i){
    mvprintw(lastY,x+i,"%s",HORIZONTAL_LINE);
  }
  mvprintw(lastY,x+3,"File \"%s\"",headerName);
  mvprintw(lastY,x+i,"%s",TR_CORNER);
  lastY++;

  if (fpRecurring != NULL){
    PrintRecurringTodo(&lastY,fpRecurring, today);
    if (fp != NULL) {
    mvprintw(lastY,x,"%s",LVERTICAL_DLINE);
      for (i = 1; i<todoSize+2 ; ++i){
        mvprintw(lastY,x+i,"%s",HORIZONTAL_DLINE);
      }
    mvprintw(lastY,x+i,"%s",RVERTICAL_DLINE);
    lastY++;
    }
  }

  if (fp != NULL) { 
    while(fgets(buff,sizeof(buff),fp) != NULL) {
      buff[strcspn(buff,"\n")] = '\0';
      mvprintw(lastY,x,"%s %s ",VERTICAL_LINE,buff);
      mvprintw(lastY++,x+todoSize+2,"%s",VERTICAL_LINE);
    }
  }

  mvprintw(lastY,x,"%s",BL_CORNER);
  for (i = 1; i<todoSize+2 ; ++i){
    mvprintw(lastY,x+i,"%s",HORIZONTAL_LINE);
  }
  mvprintw(lastY,x+i,"%s",BR_CORNER);
  if (fp != NULL) fclose(fp);
  if (fpRecurring != NULL) fclose(fpRecurring);
}

