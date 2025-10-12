#include "raylib.h"
#include "raymath.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH    900
#define SCREEN_HEIGHT   600
#define SCREEN_FPS      60

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bouncy Balls");
  SetTargetFPS(SCREEN_FPS);

  Camera3D camera = {
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE,
    .position = (Vector3){ 0.0f, 10.0f, 10.0f },
    .target = (Vector3){ 0.0f, 0.0f, 0.0f },
    .up = (Vector3){ 0.0f, 1.0f, 0.0f }
  };
  
  Mesh meshCylinder = GenMeshCylinder(1.0f, 5.f, 20);
  Model modelCylinder = LoadModelFromMesh(meshCylinder);
  Vector3 modelCylinderPosition = { 0.0f, 0.0f, 0.0f };
  float cylinderRotationDeg = { 0 };


  while (!WindowShouldClose())
  {
    float deltaTime = GetFrameTime();
    BeginDrawing();
    BeginMode3D(camera);
    ClearBackground(WHITE);

    // update rotation animation
    cylinderRotationDeg += deltaTime * 4.0f;
    modelCylinder.transform = MatrixRotateX(cylinderRotationDeg);

    // draw cylinder
    DrawModel(modelCylinder, modelCylinderPosition, 1.0f, RED);
    DrawGrid(10, 1.0f);


    EndMode3D();
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}