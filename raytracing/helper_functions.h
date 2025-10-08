#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "raylib.h"

typedef struct RayLine {
    Vector2 m_start_coordinates;
    Vector2 m_end_coordinates;  
} RayLine_t;

typedef struct Circle {
  Vector2 m_center_coordinates;
  float m_radius;
  Color m_color;
} Circle_t;

void GenerateRandomCircleInformations(Circle_t* const circleArray);
void GenerateRayLines(RayLine_t* const raylineArray, 
                      const Vector2 mousePos);

void DrawRayLines(const RayLine_t* const raylineArray);
void DrawRandomCircles(const Circle_t* const circleArray);

void CheckCollisionsUpdateLineInformations( const Circle_t* const circleArray, 
                                            RayLine_t* const      raylineArray, 
                                            const Vector2         mousePos);

Vector2 FindNearestCollisionPoint(const Circle_t* const circle, 
                                  const Vector2         raylineStart, 
                                  const Vector2         raylineEnd);

#endif // HELPER_FUNCTIONS_H