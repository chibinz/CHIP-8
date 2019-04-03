//graphics.c -- a demo for terminalGraphics.h
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#ifndef FORM_WIDTH
#define FORM_WIDTH 120
#endif
#ifndef FORM_LENGTH
#define FORM_LENGTH 30
#endif
#define CHARS (FORM_WIDTH * FORM_LENGTH)
#define DEPTH 10
#define T 100
#define D 8
#define O graph[0]
#define P(X) printf("%d", (X))

char depth[DEPTH] = {' ', '.', ':', '-', '~', '+', '=', '%', '#', '@'};
int graph[FORM_WIDTH][FORM_LENGTH];

struct point
{
  int x;
  int y;
};

struct rectangle
{
  struct point p1;
  struct point p2;
};

typedef struct point PT;
typedef struct rectangle REC;

void drawPoint(PT p, int d, int *graph)
{
  graph += p.x + p.y * (FORM_WIDTH);
  *graph += d;
}

void drawRec(REC r, int *graph)
{
  int width = (r.p2.x - r.p1.x);
  int length = (r.p2.y - r.p1.y);

  graph += r.p1.x + r.p1.y * (FORM_WIDTH);

  for(int i = 0; i <= length; i++, graph += (FORM_WIDTH - width - 1))
    for(int j = 0; j <= width; j++, graph++)
      *graph = 6;
}

void movePoint(PT *p, char ch)
{
  switch(ch)
  {
    case 'w': p->y--;  break;
    case 's': p->y++;  break;
    case 'a': p->x--;  break;
    case 'd': p->x++;  break;
  }
}

int checkHit(PT p, int *graph)
{
   graph += p.x + p.y * FORM_WIDTH;
   return *graph;
}

int checkBoundary(PT *p)
{
  if(p->x <= 0)
    p->x = 0;
  if(p->x >= FORM_WIDTH - 1)
    p->x = FORM_WIDTH - 1;
  if(p->y <= 0)
    p->y = 0;
  if(p->y >= FORM_LENGTH -2)
    p->y = FORM_LENGTH - 2;

  return 0;
}

void printGraph(int *graph)
{
  char ar[CHARS];

  for(int i = 0; i < CHARS; i++, graph++)
    for(int j = 0; j < DEPTH; j++)
      if(*graph == j)
        ar[i] = depth[j];

  ar[CHARS - 1] = '\0';
  printf("%s\n", ar);
}

void initGraph(int *graph)
{
  for(int i = 0; i < CHARS; i++, graph++)
    *graph = 0;
}

void drawBorder(int *graph)
{
  REC top = {{0, 0}, {FORM_WIDTH - 1, 0}};
  REC bottom = {{0, FORM_LENGTH - 1}, {FORM_WIDTH - 2, FORM_LENGTH - 1}};
  REC left = {{0, 0}, {0, FORM_LENGTH - 1}};
  REC right = {{FORM_WIDTH - 1, 0}, {FORM_WIDTH - 1, FORM_LENGTH - 2}};

  drawRec(top, graph);
  drawRec(bottom, graph);
  drawRec(left, graph);
  drawRec(right, graph);
}
