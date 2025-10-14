#include "raylib.h"
#include "raymath.h"    // Vector2Subtract

#include "player.h"
#include "bullet.h"
#include "asteroid.h"

#include <stdio.h>      // printf
#include <stdlib.h>     // calloc, free
#include <stdbool.h>    // bool
#include <math.h>       // atan2f, sqrtf

#define SCREEN_WIDTH          1920
#define SCREEN_HEIGHT         1080
#define SCREEN_FPS            60

#define CROSSHAIR_SCALE       1.5f

#define DEBUG

Vector2 GetClampedMousePosition(const Texture2D* crosshair)
{
  float halfWidth = (float)crosshair->width * CROSSHAIR_SCALE / 2.0f;
  float halfHeight = (float)crosshair->height * CROSSHAIR_SCALE / 2.0f;

  float mx = (float)GetMouseX();
  float my = (float)GetMouseY();

  if (mx < halfWidth) 
    mx = halfWidth;
  else if (mx > (float)SCREEN_WIDTH - halfWidth) 
    mx = (float)SCREEN_WIDTH - halfWidth;

  if (my < halfHeight) 
    my = halfHeight;
  else if (my > (float)SCREEN_HEIGHT - halfHeight) 
    my = (float)SCREEN_HEIGHT - halfHeight;

  return (Vector2){ mx, my };
}

void UpdateSpaceshipRotation(Spaceship_t* spaceship, Vector2 crosshairCenter)
{
  Vector2 centerOfTheScreen = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
  Vector2 direction = Vector2Subtract(crosshairCenter, centerOfTheScreen);
  // atan2f returns radians; convert to degrees for DrawTexturePro
  float angle = atan2f(direction.y, direction.x) * (180.0f / PI);

  // Raylib's rotation is clockwise-positive when using DrawTexturePro angles in degrees
  spaceship->m_rotation = angle;
}

void UpdateBulletsLocations(Bullet_t* bulletsArray)
{
  float deltaTime = GetFrameTime();

  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {
      bulletsArray[i].m_position.x += bulletsArray[i].m_direction.x * BULLET_SPEED * deltaTime;
      bulletsArray[i].m_position.y += bulletsArray[i].m_direction.y * BULLET_SPEED * deltaTime;

      // Deactivate if off-screen
      if (bulletsArray[i].m_position.x < 0 || bulletsArray[i].m_position.x > SCREEN_WIDTH ||
          bulletsArray[i].m_position.y < 0 || bulletsArray[i].m_position.y > SCREEN_HEIGHT)
      {
        bulletsArray[i].m_active = false;
      }
    }
  }
}

void UpdateSpaceshipPosition(Spaceship_t* spaceship, float deltaTime)
{
  if (IsKeyDown(KEY_D))
    SpaceshipGoRight(spaceship, deltaTime);
  if (IsKeyDown(KEY_A))
    SpaceshipGoLeft(spaceship, deltaTime);
  if (IsKeyDown(KEY_W))
    SpaceshipGoUp(spaceship, deltaTime);
  if (IsKeyDown(KEY_S))
    SpaceshipGoDown(spaceship, deltaTime);
}
/*
void UpdateAsteroids(Asteroid_t* asteroidsArray)
{
  float deltaTime = GetFrameTime();

  for (int i = 0; i < maxAsteroidCount; i++)
  {
    if (asteroidsArray[i].m_active)
    {
      Asteroid_t* asteroid = &asteroidsArray[i];

      asteroid->m_position.x += asteroid->m_direction.x * asteroid->m_speed * deltaTime;
      asteroid->m_position.y += asteroid->m_direction.y * asteroid->m_speed * deltaTime;

      // Deactivate Asteroid 
      if ((asteroid->m_position.x < 0               ||
          asteroid->m_position.y < 0                ||
          asteroid->m_position.x > SCREEN_WIDTH     || 
          asteroid->m_position.y > SCREEN_HEIGHT)   &&
          asteroid->m_lifetime < 2.0f)
      {
        asteroid->m_active = false;
      }
      else if (asteroid->m_lifetime < 2.0f)
      {
        asteroid->m_lifetime += deltaTime;
      }
    }
  }
}
  */

void DrawSpaceship(const Texture2D* texture, Spaceship_t* spaceship)
{
  DrawTexturePro( *texture,
    (Rectangle){PLAYER_TOP_LEFT_X, 
                PLAYER_TOP_LEFT_Y, 
                PLAYER_WIDTH,
                PLAYER_HEIGHT },
    (Rectangle){
      spaceship->m_center.x,
      spaceship->m_center.y,
      PLAYER_WIDTH,
      PLAYER_HEIGHT},
    (Vector2){PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2},
    spaceship->m_rotation,
    WHITE);
}

void DrawCrosshair(const Texture2D* texture, Vector2 crosshairCenter)
{
  float drawWidth = (float)texture->width * CROSSHAIR_SCALE;
  float drawHeight = (float)texture->height * CROSSHAIR_SCALE;

  DrawTexturePro( *texture,
    (Rectangle){0.0f, 0.0f, (float)texture->width, (float)texture->height},
    (Rectangle){ crosshairCenter.x, crosshairCenter.y, drawWidth, drawHeight },
    (Vector2){ drawWidth / 2.0f, drawHeight / 2.0f },
    0.0f,
    WHITE);

  /*
    DrawRectangleLines((int)(crosshairCenter.x - drawWidth / 2.0f),
                      (int)(crosshairCenter.y - drawHeight / 2.0f),
                      (int)drawWidth,
                      (int)drawHeight,
                      GREEN);
  */
}

void FindFirstAvailableBulletAndShoot(Bullet_t* bulletsArray, Vector2 spaceshipPosition, Vector2 crosshairCenter)
{  
  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (!bulletsArray[i].m_active)
    {
      bulletsArray[i].m_active = true;
      bulletsArray[i].m_position = spaceshipPosition;
      bulletsArray[i].m_direction = Vector2Normalize(Vector2Subtract(crosshairCenter, spaceshipPosition));
      break;
    } 
  }
}

void DrawBullets(const Texture2D* texture, Bullet_t* bulletsArray, Vector2 crosshairCenter)
{
  float deltaTime = GetFrameTime();

  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {      
      Bullet_t* bullet = &bulletsArray[i];

      bullet->m_position.x += bullet->m_direction.x * BULLET_SPEED * deltaTime;
      bullet->m_position.y += bullet->m_direction.y * BULLET_SPEED * deltaTime;

      if (bullet->m_position.x < 0 || bullet->m_position.x > SCREEN_WIDTH ||
          bullet->m_position.y < 0 || bullet->m_position.y > SCREEN_HEIGHT)
      {
        bullet->m_active = false;
      }
    }
  }

  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {
      DrawTexturePro( *texture,
        (Rectangle){ BULLET_TOP_LEFT_X, 
                     BULLET_TOP_LEFT_Y, 
                     BULLET_WIDTH,
                     BULLET_HEIGHT },
        (Rectangle){
          bulletsArray[i].m_position.x,
          bulletsArray[i].m_position.y,
          BULLET_WIDTH,
          BULLET_HEIGHT},
        (Vector2){BULLET_WIDTH / 2, BULLET_HEIGHT / 2},
        0.0f,
        WHITE);
    }
  }
}

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fresh Start Files");

  int monitorCount = GetMonitorCount();
  if (monitorCount > 1)
    SetWindowMonitor(1);
  
  SetTargetFPS(SCREEN_FPS);
  ToggleBorderlessWindowed();
  DisableCursor();

  Texture2D texture = LoadTexture("assets/asteroids.png");
  Texture2D crosshairTexture = LoadTexture("assets/crosshair.png");

  Spaceship_t spaceship = {
    .m_center = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
    .m_rotation = 0.0f,
  };

  Bullet_t* bulletsArray = (Bullet_t*)calloc(MAX_ACTIVE_BULLET_COUNT, sizeof(Bullet_t));
  if (bulletsArray == NULL)
  {
    CloseWindow();
    return -1;
  }

  int maxAsteroidCount = 5;
  Asteroid_t* asteroidsArray = (Asteroid_t*)calloc(maxAsteroidCount, sizeof(Asteroid_t));
  if( asteroidsArray == NULL)
  {
    free(bulletsArray);
    CloseWindow();
    return -1;
  }

  Vector2 crosshairCenter = { 0 };

  // TODO: i want to setup a timer and realloc by 1 asteroid every 2 second
  
  // float asteroidSpawnTimer = 0.0f;

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK);

    // -------------- UPDATING --------------
    crosshairCenter = GetClampedMousePosition(&crosshairTexture);
    UpdateSpaceshipRotation(&spaceship, crosshairCenter);
    UpdateSpaceshipPosition(&spaceship, GetFrameTime());


    // ilk tur 5 adet asteroid spaw edilecek.
    // bu asteroidlerin m_active si true olacak.
    // eger ekranin disina cikma durumu veya patlama durumu ol

    /*
      // -------------- SPAWNING ASTEROIDS BY TIMER --------------
      asteroidSpawnTimer += GetFrameTime();

      if (asteroidSpawnTimer >= 2.0f)
      {
        asteroidSpawnTimer = 0.0f;
        asteroidsArray = realloc(asteroidsArray, (maxAsteroidCount + 1) * sizeof(Asteroid_t));
        if (asteroidsArray == NULL)
        {
          free(bulletsArray);
          CloseWindow();
          return -1;
        }
        

        UpdateAsteroids(asteroidsArray);
        ++maxAsteroidCount;
      }
    */
    UpdateBulletsLocations(bulletsArray);

    // -------------- DRAWING --------------
    DrawSpaceship(&texture, &spaceship);
    DrawCrosshair(&crosshairTexture, crosshairCenter);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      FindFirstAvailableBulletAndShoot(bulletsArray, spaceship.m_center, crosshairCenter);

    DrawBullets(&texture, bulletsArray, crosshairCenter);
    // DrawAsteroids(&texture, asteroidsArray);

    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, RED);
    DrawLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, RED);
    DrawLine(spaceship.m_center.x, spaceship.m_center.y, (int)crosshairCenter.x, (int)crosshairCenter.y, YELLOW);
    DrawFPS(10, 10);

    EndDrawing();
  }

  // --------------------- CLEANUP ---------------------

  free(bulletsArray);
  free(asteroidsArray);
  UnloadTexture(texture);
  UnloadTexture(crosshairTexture);
  CloseWindow();
  return 0;
}

