#include "common.h"
#include "calendar.h"
#include "config.h"

const int DAYSINMONTHS[12] ={
  31,
  28,
  31,
  30,
  31,
  30,
  31,
  31,
  30,
  31,
  30,
  31
};

int CalculateFirstDay(struct tm *today){
  int daysSinceFirst = today->tm_mday - 1;
  int firstWeekDay = today->tm_wday - (daysSinceFirst % 7);

  firstWeekDay--;

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

