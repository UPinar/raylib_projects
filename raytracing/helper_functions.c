#include "helper_functions.h"
#include "constants.h"

#include <math.h>
#include <stdlib.h>   // rand

Vector2 collisionPoint = { 0 };

void GenerateRandomCircleInformations(Circle_t* const circleArray)
{
  for (int i = 0; i < RANDOM_CIRCLE_COUNT; i++)
  {
    circleArray[i].m_center_coordinates = (Vector2){ 
      (float)(rand() % (SCREEN_WIDTH - 200)) + 100, 
      (float)(rand() % (SCREEN_HEIGHT - 200)) + 100 
    };
    // radius between 30 and 130
    circleArray[i].m_radius = (float)(rand() % 101) + 30.0f;
    circleArray[i].m_color = (Color){ 
      (unsigned char)(rand() % 256), 
      (unsigned char)(rand() % 256), 
      (unsigned char)(rand() % 256), 
      255 
    };
  }
}

void CheckCollisionsUpdateLineInformations( const Circle_t* const   circleArray, 
                                            RayLine_t* const        raylineArray, 
                                            const Vector2           mousePos)
{
  for (int i = 0; i < RANDOM_CIRCLE_COUNT; i++)
  {
    for (int j = 0; j < RAY_LINE_COUNT; j++)
    {
      if (CheckCollisionCircleLine( circleArray[i].m_center_coordinates, 
                                    circleArray[i].m_radius, 
                                    raylineArray[j].m_start_coordinates, 
                                    raylineArray[j].m_end_coordinates))
      {
        collisionPoint = FindNearestCollisionPoint( &circleArray[i],
                                                    raylineArray[j].m_start_coordinates, 
                                                    raylineArray[j].m_end_coordinates);
        raylineArray[j].m_end_coordinates = collisionPoint;
      }
    }
  }
}

void DrawRandomCircles(const Circle_t* const circleArray)
{
  for (int i = 0; i < RANDOM_CIRCLE_COUNT; i++)
    DrawCircleV(circleArray[i].m_center_coordinates, 
                circleArray[i].m_radius, 
                circleArray[i].m_color);
}

Vector2 FindNearestCollisionPoint(  const Circle_t* const circle, 
                                    const Vector2         raylineStart, 
                                    const Vector2         raylineEnd)
{
  float center_x = circle->m_center_coordinates.x;
  float center_y = circle->m_center_coordinates.y;
  float r = circle->m_radius;
  // Equation: (x - center_x)^2 + (y - center_y)^2 = r * r

  float x1 = raylineStart.x,  y1 = raylineStart.y;
  float x2 = raylineEnd.x,    y2 = raylineEnd.y;

  float dx = x2 - x1;
  float dy = y2 - y1;

  float A = dx * dx + dy * dy;
  float B = 2 * (dx * (x1 - center_x) + dy * (y1 - center_y));
  float C = (x1 - center_x) * (x1 - center_x) + 
            (y1 - center_y) * (y1 - center_y) - r * r;

  float discriminant = B * B - 4 * A * C;

  float sqrt_discriminant = sqrtf(discriminant);
  float t1 = (-B - sqrt_discriminant) / (2 * A);
  float t2 = (-B + sqrt_discriminant) / (2 * A);

  float t = (t1 >= 0 && t1 <= 1) ? t1 : t2; 
  float intersection_x = x1 + t * dx;
  float intersection_y = y1 + t * dy;
  return (Vector2){ intersection_x, intersection_y };
}

void DrawRayLines(const RayLine_t* const raylineArray)
{
  for (int i = 0; i < RAY_LINE_COUNT; i++)
    DrawLineV(  raylineArray[i].m_start_coordinates, 
                raylineArray[i].m_end_coordinates, RED);
}

void GenerateRayLines(RayLine_t* const raylineArray, const Vector2 mousePos)
{
  for (int i = 0; i < RAY_LINE_COUNT; i++)
  {
    float angle = (360.0f / RAY_LINE_COUNT) * i;
    raylineArray[i].m_start_coordinates  = (Vector2){ mousePos.x, mousePos.y };
    raylineArray[i].m_end_coordinates    = (Vector2){ 
              mousePos.x + cosf(angle * DEG2RAD) * LIMIT_FOR_REACHING_SCREEN_EDGE,
              mousePos.y + sinf(angle * DEG2RAD) * LIMIT_FOR_REACHING_SCREEN_EDGE };
  }
}