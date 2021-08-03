// Thanks to this: https://stackoverflow.com/a/44237055/4215651

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE // Enable POSIX extensions in standard library headers
#endif
#include <unistd.h>
#include <time.h>
#endif

// Use an enum for platform-independent interface:
typedef enum TextColor
{
  TC_BLACK = 0,
  TC_BLUE = 1,
  TC_GREEN = 2,
  TC_CYAN = 3,
  TC_RED = 4,
  TC_MAGENTA = 5,
  TC_BROWN = 6,
  TC_LIGHTGRAY = 7,
  TC_DARKGRAY = 8,
  TC_LIGHTBLUE = 9,
  TC_LIGHTGREEN = 10,
  TC_LIGHTCYAN = 11,
  TC_LIGHTRED = 12,
  TC_LIGHTMAGENTA = 13,
  TC_YELLOW = 14,
  TC_WHITE = 15
} TextColor;
TextColor __defaultColor = TC_WHITE;

void clearScreen();

void sleepFor(int);
void microSleepFor(int);

void setTextColor(FILE *stream, TextColor color);
void setDefaultColor(TextColor color);
void setColor(TextColor color);
void resetColor();
TextColor getColor(char *);

void clearScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void sleepFor(int seconds)
{
  // Empty the buffer, then sleep
  fflush(stdout);

#ifdef _WIN32
  Sleep(seconds * 1000);
#else
  struct timespec stop = {0};
  stop.tv_sec = seconds;
  nanosleep(&stop, (struct timespec *)(NULL));
#endif
}

void microSleepFor(int milliseconds)
{
  // Empty the buffer, then sleep
  fflush(stdout);

#ifdef _WIN32
  Sleep(milliseconds);
#else
  struct timespec stop = {0};
  stop.tv_nsec = milliseconds * 1000000;
  nanosleep(&stop, (struct timespec *)(NULL));
#endif
}

#ifdef _WIN32

void setTextColor(FILE *stream, TextColor color)
{
  int outFd = fileno(stream);
  HANDLE out = (HANDLE)_get_osfhandle(outFd);
  DWORD outType = GetFileType(out);
  DWORD mode;

  if (outType == FILE_TYPE_CHAR && GetConsoleMode(out, &mode))
  {
    // We're directly outputting to a win32 console if the file type
    // is FILE_TYPE_CHAR and GetConsoleMode() returns success
    SetConsoleTextAttribute(out, color);
  }
}

#else

static const char *ansiColorSequences[] =
    {
        "\e[0;30m",
        "\e[0;34m",
        "\e[0;32m",
        "\e[0;36m",
        "\e[0;31m",
        "\e[0;35m",
        "\e[0;33m",
        "\e[0;39m",
        "\e[0;90m",
        "\e[0;94m",
        "\e[0;92m",
        "\e[0;96m",
        "\e[0;91m",
        "\e[0;95m",
        "\e[0;93m",
        "\e[0;97m"};

static const char *ansiColorTerms[] =
    {
        "xterm",
        "rxvt",
        "vt100",
        "linux",
        "screen",
        0
        // ...
};

// Get current terminal and check whether it's in our list of terminals
// supporting ANSI colors
static int isAnsiColorTerm(void)
{
  char *term = getenv("TERM");
  for (const char **ansiTerm = &ansiColorTerms[0]; *ansiTerm; ++ansiTerm)
  {
    int match = 1;
    const char *t = term;
    const char *a = *ansiTerm;
    while (*a && *t)
    {
      if (*a++ != *t++)
      {
        match = 0;
        break;
      }
    }
    if (match)
    {
      return 1;
    }
  }
  return 0;
}

void setTextColor(FILE *stream, TextColor color)
{
  int outFd = fileno(stream);
  if (isatty(outFd) && isAnsiColorTerm())
  {
    // We're directly outputting to a terminal supporting ANSI colors,
    // so send the apppropriate sequence
    fputs(ansiColorSequences[color], stream);
  }
}

#endif

void setDefaultColor(TextColor color)
{
  __defaultColor = color;
}

void setColor(TextColor color)
{
  setTextColor(stdout, color);
}

void resetColor()
{
  setTextColor(stdout, __defaultColor);
}

TextColor getColor(char *color)
{
  char *colors[] = {
      "black",
      "blue",
      "green",
      "cyan",
      "red",
      "magenta",
      "brown",
      "lightgray",
      "darkgray",
      "lightblue",
      "lightgreen",
      "lightcyan",
      "lightred",
      "lightmagenta",
      "yellow",
      "white"};

  if (strcmp(color, "reset") == 0)
  {
    return __defaultColor;
  }

  for (int i = 0; i < 16; i++)
  {
    if (strcmp(color, colors[i]) == 0)
    {
      return (TextColor)(i);
    }
  }

  // If a color cannot be found
  return getColor("reset");
}

#endif