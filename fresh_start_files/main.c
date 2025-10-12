#include "raylib.h"

#define SCREEN_WIDTH    900
#define SCREEN_HEIGHT   600
#define SCREEN_FPS      60

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fresh Start Files");
  SetTargetFPS(SCREEN_FPS);

  while (!WindowShouldClose())
  {
    BeginDrawing();

    ClearBackground(BLACK);

    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}