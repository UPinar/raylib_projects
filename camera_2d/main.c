#include "raylib.h"
#include "raymath.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080
#define SCREEN_FPS      60

#define CIRCLE_COUNT    (size_t)1000

#define PLAYER_SPEED    500.0f
#define CAMERA_SPEED_HORIZONTAL   100.0f
#define CAMERA_SPEED_VERTICAL     1.0f

typedef struct {
  Vector2 m_coordinates;
  Vector2 m_directions;
  float m_radius;
  Color m_color;
} Circle_t;


void GenerateCircles(Circle_t* circleArray, size_t circleCount);
void DrawCircles(Circle_t* circleArray, size_t circleCount);

void PlayerGoRight(Circle_t* playerCircle, float deltaTime);
void PlayerGoLeft(Circle_t* playerCircle, float deltaTime);
void PlayerGoUp(Circle_t* playerCircle, float deltaTime);
void PlayerGoDown(Circle_t* playerCircle, float deltaTime);

void CameraRotateRight(Camera2D* camera, float deltaRotation);
void CameraRotateLeft(Camera2D* camera, float deltaRotation);
void CameraZoomIn(Camera2D* camera, float deltaZoom);
void CameraZoomOut(Camera2D* camera, float deltaZoom);


int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bouncy Balls");

  Circle_t* circleArray = malloc(sizeof(Circle_t) * CIRCLE_COUNT);
  if (!circleArray)
    exit(EXIT_FAILURE);

  GenerateCircles(circleArray, CIRCLE_COUNT);

  Circle_t playerCircle = {
    .m_coordinates = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
    .m_radius = 50.0f,
    .m_color = RED,
  };

  float deltaTime = 0.0f;

  Camera2D camera = { 
    .target = playerCircle.m_coordinates,
    .zoom = 1.0f,
    .offset = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
    .rotation = 0.0f,
   };

  while (!WindowShouldClose())
  {
    deltaTime = GetFrameTime();

    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(BLACK);
    DrawCircles(circleArray, CIRCLE_COUNT);

    // Update Player Movement
  
    if (IsKeyDown(KEY_D))
      PlayerGoRight(&playerCircle, deltaTime);
    if (IsKeyDown(KEY_A))
      PlayerGoLeft(&playerCircle, deltaTime);
    if (IsKeyDown(KEY_W))
      PlayerGoUp(&playerCircle, deltaTime);
    if (IsKeyDown(KEY_S))
      PlayerGoDown(&playerCircle, deltaTime);


    // Update Camera Movement
    camera.target = playerCircle.m_coordinates;

    if (IsKeyDown(KEY_RIGHT))
      CameraRotateRight(&camera, CAMERA_SPEED_HORIZONTAL * deltaTime);
    if (IsKeyDown(KEY_LEFT))
      CameraRotateLeft(&camera, CAMERA_SPEED_HORIZONTAL * deltaTime);
    if (IsKeyDown(KEY_UP))
      CameraZoomIn(&camera, CAMERA_SPEED_VERTICAL * deltaTime);
    if (IsKeyDown(KEY_DOWN))
      CameraZoomOut(&camera, CAMERA_SPEED_VERTICAL * deltaTime);

    // DRAW
    DrawCircleV(playerCircle.m_coordinates, playerCircle.m_radius, playerCircle.m_color);

    printf("Camera Zoom: %.2f | Camera Rotation: %.2f \n", camera.zoom, camera.rotation);

    EndMode2D();
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}

void GenerateCircles(Circle_t* circleArray, size_t circleCount)
{
  for (size_t i = 0; i < circleCount; ++i)
  {
    circleArray[i] = (Circle_t){
      .m_coordinates = (Vector2) {GetRandomValue(0, 2000), GetRandomValue(0, 2000)},
      .m_radius = (float)GetRandomValue(10, 30),
      .m_color = (Color){
        (unsigned char)GetRandomValue(0, 255),
        (unsigned char)GetRandomValue(0, 255),
        (unsigned char)GetRandomValue(0, 255),
        (unsigned char)255
      }
    };
  }
}

void DrawCircles(Circle_t* circleArray, size_t circleCount)
{
  for (size_t i = 0; i < circleCount; ++i)
  {
    DrawCircleV(circleArray[i].m_coordinates,
                circleArray[i].m_radius,
                circleArray[i].m_color);
  }
}

void PlayerGoRight(Circle_t* playerCircle, float deltaTime)
{
  playerCircle->m_directions.x = 1.0f;
  Vector2Normalize(playerCircle->m_directions);
  playerCircle->m_coordinates.x += playerCircle->m_directions.x *deltaTime * PLAYER_SPEED;
}

void PlayerGoLeft(Circle_t* playerCircle, float deltaTime)
{
  playerCircle->m_directions.x = -1.0f;
  Vector2Normalize(playerCircle->m_directions);
  playerCircle->m_coordinates.x += playerCircle->m_directions.x *deltaTime * PLAYER_SPEED;
}

void PlayerGoUp(Circle_t* playerCircle, float deltaTime)
{
  playerCircle->m_directions.y = -1.0f;
  Vector2Normalize(playerCircle->m_directions);
  playerCircle->m_coordinates.y += playerCircle->m_directions.y *deltaTime * PLAYER_SPEED;
}

void PlayerGoDown(Circle_t* playerCircle, float deltaTime)
{
  playerCircle->m_directions.y = 1.0f;
  Vector2Normalize(playerCircle->m_directions);
  playerCircle->m_coordinates.y += playerCircle->m_directions.y *deltaTime * PLAYER_SPEED;
}

void CameraRotateRight(Camera2D* camera, float deltaRotation)
{
  camera->rotation += deltaRotation;
}

void CameraRotateLeft(Camera2D* camera, float deltaRotation)
{
  camera->rotation -= deltaRotation;
}

void CameraZoomIn(Camera2D* camera, float deltaZoom)
{
  camera->zoom += deltaZoom;
}

void CameraZoomOut(Camera2D* camera, float deltaZoom)
{
  camera->zoom -= deltaZoom;
  if (camera->zoom < 0.1f)
    camera->zoom = 0.1f;
}