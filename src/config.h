#ifndef CONFIG_H
#define CONFIG_H

#define ANSI_COLOR "\x1b[42m\x1b[30m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define PATH_MAX 256
#define KEY_UP_CODE 259
#define KEY_DOWN_CODE 258
#define KEY_LEFT_CODE 260
#define KEY_RIGHT_CODE 261
#define TIMEOUT 150

extern int MAXX;
extern int MAXY;

extern const char *MONTHS[12];
extern const int DAYSINMONTHS[12];

#endif

