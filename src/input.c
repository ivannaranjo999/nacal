#include "common.h"
#include "input.h"
#include "calendar.h"

int SelectDate(struct tm *selectTm){
  int selectValue, selectDaysInMonth;
  int returnValue = 0;
  size_t selectLen;
  char selectBuf[8];

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
  for (size_t i = 0; i < selectLen; i++)
    if (!isdigit((unsigned char)selectBuf[i])) returnValue = 1;

  selectValue = atoi(selectBuf) - 1900;
  if (selectValue < 0 || selectValue > 1100) returnValue = 1;
  selectTm->tm_year = selectValue;

  mvprintw(2,0,"Select month:");
  refresh();
  move(2, 14);
  clrtoeol();
  echo();
  curs_set(1);
  getnstr(selectBuf, 2);
  noecho();
  curs_set(0);

  selectValue = atoi(selectBuf) - 1;
  if(selectValue < 0 || selectValue > 11) returnValue = 1;
  selectTm->tm_mon = selectValue;

  mvprintw(4,0,"Select day:");
  refresh();
  move(4, 12);
  clrtoeol();
  echo();
  curs_set(1);
  getnstr(selectBuf, 2);
  noecho();
  curs_set(0);

  selectValue = atoi(selectBuf);
  selectTm->tm_mday = selectValue;
  selectDaysInMonth = GetDaysInMonth(selectTm);
  if(selectValue < 0 || selectValue > selectDaysInMonth) returnValue = 1;

  mktime(selectTm);

  if (returnValue == 0)
    mvprintw(6,0,"Selected %d/%d/%d. Press any key to continue",
      selectTm->tm_year+1900,selectTm->tm_mon+1,selectTm->tm_mday);
  else
    mvprintw(6,0,"Introduced date is not allowed. Press any key to continue");

  refresh();
  getch();
  return returnValue;
}

