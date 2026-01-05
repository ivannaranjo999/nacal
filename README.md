# nacal
'nacal' is a lightweight TUI (Text User Interface) calendar written in C and ncurses.
It allows you to navigate through a calendar and attach notes to individual days, all from the terminal.

The interface is keyboard-driven and designed to work in a UTF-8 terminal.

![Add anything to any day's note](./images/example1.png)

![Even ASCII art!](./images/example2.png)

## Features
- Terminal-based calendar (TUI)
- Navigate days and months
- View persistent notes for each day
- Lightweight and fast

## Requirements
- C compiler (GCC or compatible)
- ncurses

## Compilation

```
gcc -Wall -Wextra nacal.c -lncurses -o nacal
```

```
./nacal
```

## Basic controls
- 'hjkl' keys to navigate though the same month
- 'np' keys to change months
- 's' key to select specific date
- 't' key to select today
- 'q' key to quit

## License

This project is provided as-is for learning and personal use.
