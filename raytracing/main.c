#include "raylib.h"
#include "helper_functions.h"
#include "constants.h"

#include <stdlib.h>   // malloc and free
#include <time.h>
#include <math.h>


int main(void)
{
  srand((unsigned)time(NULL));
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raytracing");
  SetTargetFPS(144);

  RayLine_t* raylineArray = (RayLine_t*)malloc(sizeof(RayLine_t) * RAY_LINE_COUNT);
  Circle_t* circleArray = (Circle_t*)malloc(sizeof(Circle_t) * RANDOM_CIRCLE_COUNT);

  GenerateRandomCircleInformations(circleArray);
  float circleTimer = 0.0f;
  Vector2 mousePos = { 0 };


  while (!WindowShouldClose())
  {
    mousePos = GetMousePosition();


    BeginDrawing();
  
    // ------- UPDATE ----------
    // -------------------------
    GenerateRayLines(raylineArray, mousePos);

    /* advance timer and regenerate every 2 seconds */
    circleTimer += GetFrameTime();
    if (circleTimer >= 2.0f)
    {
      GenerateRandomCircleInformations(circleArray);
      circleTimer = 0.0f;
    }

    CheckCollisionsUpdateLineInformations(circleArray, raylineArray, mousePos);
    // -------------------------
    // -------------------------
    
    
    // ------- RENDERING -------
    // -------------------------
    ClearBackground(BLACK);

    DrawRandomCircles(circleArray);
    DrawRayLines(raylineArray);
    DrawCircleV((Vector2){ mousePos.x, mousePos.y }, SUN_RADIUS, WHITE);
    // -------------------------
    // -------------------------

    EndDrawing();
  }
  
  free(raylineArray);
  free(circleArray);
  raylineArray = NULL;
  circleArray = NULL;

  CloseWindow();
  return 0;
}