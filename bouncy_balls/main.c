#include "raylib.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   900
#define SPEED_CONSTANT  2.0

typedef struct Ball {
    Vector2 m_center;
    Vector2 m_velocity;
    Color m_color;
    float m_radius;
} Ball_t;

void UpdateSpeed(Ball_t* const ball, double deltaTime);
void CheckCollisionsOnEdges(Ball_t* const ball);



int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bouncy Balls");
  SetTargetFPS(144);

  Ball_t ball = { .m_center = (Vector2){200 , 200},
                  .m_velocity = (Vector2){200, 200},
                  .m_color = RED,
                  .m_radius = 50
                };

  while (!WindowShouldClose())
  {
    double deltaTime = GetFrameTime();
    deltaTime *= SPEED_CONSTANT;
    UpdateSpeed(&ball, deltaTime);

    BeginDrawing();
  
    ClearBackground(BLACK);
    CheckCollisionsOnEdges(&ball);
    DrawCircleV(ball.m_center, ball.m_radius, ball.m_color);

    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}

void UpdateSpeed(Ball_t* ball, double deltaTime)
{
  ball->m_center.x += ball->m_velocity.x * deltaTime;
  ball->m_center.y += ball->m_velocity.y * deltaTime;
}

void CheckCollisionsOnEdges(Ball_t* const ball)
{
  // Tavan
  if (CheckCollisionCircleLine( ball->m_center,
                                ball->m_radius,
                                (Vector2){0, 0},
                                (Vector2){SCREEN_WIDTH, 0}))
    ball->m_velocity.y *= -1;
    
  // Yuzey
  if (CheckCollisionCircleLine( ball->m_center,
                                ball->m_radius,
                                (Vector2){0, 0},
                                (Vector2){0, SCREEN_HEIGHT}))
    ball->m_velocity.x *= -1;
      
  // Sag 
  if (CheckCollisionCircleLine( ball->m_center,
                                ball->m_radius,
                                (Vector2){SCREEN_WIDTH, 0},
                                (Vector2){SCREEN_WIDTH, SCREEN_HEIGHT}))
    ball->m_velocity.x *= -1;
        
  // Sol
  if (CheckCollisionCircleLine( ball->m_center,
                                ball->m_radius,
                                (Vector2){0, SCREEN_HEIGHT},
                                (Vector2){SCREEN_WIDTH, SCREEN_HEIGHT}))
    ball->m_velocity.y *= -1;
}

