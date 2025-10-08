#include "raylib.h"

#include <math.h>
#include <stdlib.h>

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   900

#define CIRCLE_RADIUS   150
#define CIRCLE_POSITION_VECTOR2  (Vector2){ 720, 450 }

#define RAY_LINE_COUNT  270
#define LIMIT_FOR_REACHING_SCREEN_EDGE 1500


typedef struct RayLine {
    Vector2 start_coordinates;
    Vector2 end_coordinates;  
} RayLine_t;

void generate_ray_lines(RayLine_t*, Vector2);
void draw_ray_lines(const RayLine_t*);
void check_collisions(RayLine_t*, const Vector2);
Vector2 find_nearest_collision_point(const Vector2, const Vector2);

Vector2 collision_point = { 0 };

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World");
  SetTargetFPS(144);

  RayLine_t* rayline_array = (RayLine_t*)malloc(sizeof(RayLine_t) * RAY_LINE_COUNT);
  Vector2 mouse_pos = { 0 }; 
  
  while (!WindowShouldClose())
  {
    mouse_pos = GetMousePosition();

    BeginDrawing();
    
    // ------- UPDATE -------
    generate_ray_lines(rayline_array, mouse_pos);
    check_collisions(rayline_array, mouse_pos);
    // ----------------------
    
    // ------- RENDERING -------
    ClearBackground(BLACK);

    DrawCircleV(CIRCLE_POSITION_VECTOR2, CIRCLE_RADIUS, DARKGRAY);

    draw_ray_lines(rayline_array);
    DrawCircleV((Vector2){ mouse_pos.x, mouse_pos.y }, 60, WHITE);
    // --------------------------
    
    EndDrawing();
  }
  
  free(rayline_array);
  rayline_array = nullptr;

  CloseWindow();
  return 0;
}


void generate_ray_lines(RayLine_t* rayline_array, const Vector2 mouse_pos)
{
  for (int i = 0; i < RAY_LINE_COUNT; i++)
  {
    float angle = (360.0f / RAY_LINE_COUNT) * i;
    rayline_array[i].start_coordinates  = (Vector2){ mouse_pos.x, mouse_pos.y };
    rayline_array[i].end_coordinates    = (Vector2){ 
              mouse_pos.x + cosf(angle * DEG2RAD) * LIMIT_FOR_REACHING_SCREEN_EDGE,
              mouse_pos.y + sinf(angle * DEG2RAD) * LIMIT_FOR_REACHING_SCREEN_EDGE };
  }
}
void draw_ray_lines(const RayLine_t* rayline_array)
{
  for (int i = 0; i < RAY_LINE_COUNT; i++)
    DrawLineV(  rayline_array[i].start_coordinates, 
                rayline_array[i].end_coordinates, RED);
}
void check_collisions(RayLine_t* rayline_array, const Vector2 mouse_pos)
{
  for (int i = 0; i < RAY_LINE_COUNT; i++)
  {
    if (CheckCollisionCircleLine( CIRCLE_POSITION_VECTOR2, 
                                  CIRCLE_RADIUS, 
                                  rayline_array[i].start_coordinates, 
                                  rayline_array[i].end_coordinates))
    {
      collision_point = find_nearest_collision_point( rayline_array[i].start_coordinates, 
                                                      rayline_array[i].end_coordinates);
      rayline_array[i].end_coordinates = collision_point;
    }
  }
}
Vector2 find_nearest_collision_point(const Vector2 line_start, const Vector2 line_end)
{
  float center_x = CIRCLE_POSITION_VECTOR2.x;
  float center_y = CIRCLE_POSITION_VECTOR2.y;
  float r = CIRCLE_RADIUS;
  // Equation: (x - center_x)^2 + (y - center_y)^2 = r * r

  float x1 = line_start.x,  y1 = line_start.y;
  float x2 = line_end.x,    y2 = line_end.y;

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