#include "ui_calendar.h"
#include "common.h"
#include "calendar.h"
#include "config.h"

const char *MONTHS[12] ={
  "January","February","March","April","May","June",
  "July","August","September","October","November","December"
};

int PrintMonth(struct tm *today, int startY, int startX) {
  int y = startY;
  int x = startX;
  int selectedDay = today->tm_mday;

  mvprintw(y++, x, "%s", MONTHS[today->tm_mon]);
  mvprintw(y++, x, "%2s %2s %2s %2s %2s %2s %2s",
        "M","T","W","T","F","S","S");

  int firstWeekDay = CalculateFirstDay(today);
  int lastPrintedDay = 0;
  int dim = GetDaysInMonth(today);

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

