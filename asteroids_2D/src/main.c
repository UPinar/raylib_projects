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

size_t maxAsteroidCount = 100;

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

static bool CheckIfAsteroidIsOutOfScreen(Asteroid_t* asteroid)
{
  return (  asteroid->m_position.x < 0              || 
            asteroid->m_position.x > SCREEN_WIDTH   ||
            asteroid->m_position.y < 0              || 
            asteroid->m_position.y > SCREEN_HEIGHT);

}

static Vector2 GetRandomAsteroidDirection(void)
{
  float x = GetRandomValue(0, SCREEN_WIDTH);
  float y = GetRandomValue(0, SCREEN_HEIGHT);
  return Vector2Normalize((Vector2){ x, y });
}

static Vector2 GetRandomAsteroidSpawnLocation(void)
{
  float x = 0.0f;
  float y = 0.0f;

  int side = GetRandomValue(0, 3); 

  switch (side)
  {
    case 0: // left
      x = GetRandomValue(-200, 0);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 1: // right
      x = GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 2: // top
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, 0);
      break;
    case 3: // bottom
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(SCREEN_HEIGHT, SCREEN_HEIGHT + 200);
      break;
  }

  return (Vector2){ x, y };
}

void UpdateAsteroids(Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  float deltaTime = GetFrameTime();

  // Deactivate asteroids that are out of screen for more than 2 seconds
  for (int i = 0; i < currentAsteroidCount; i++)
    if (asteroidsArray[i].m_active)
    {
      Asteroid_t* asteroid = &asteroidsArray[i];

      asteroid->m_position.x += asteroid->m_direction.x * asteroid->m_speed * deltaTime;
      asteroid->m_position.y += asteroid->m_direction.y * asteroid->m_speed * deltaTime;

      bool asteroidIsOutOfScreen = CheckIfAsteroidIsOutOfScreen(asteroid);
      if (asteroidIsOutOfScreen && asteroid->m_lifetime < 2.0f)
        asteroid->m_lifetime += deltaTime;
      else if (asteroidIsOutOfScreen && asteroid->m_lifetime >= 2.0f)
        asteroid->m_active = false;
    }


  // Spawn new asteroids if there is an inactive one
  for (int i = 0; i < currentAsteroidCount; i++)
    if (!asteroidsArray[i].m_active)
    {
      Asteroid_t* asteroid = &asteroidsArray[i];
      asteroid->m_active = true;
      asteroid->m_position = GetRandomAsteroidSpawnLocation();
      asteroid->m_direction = GetRandomAsteroidDirection();
      asteroid->m_speed = (float)GetRandomValue((int)ASTEROID_MIN_SPEED, (int)ASTEROID_MAX_SPEED);
      asteroid->m_lifetime = 0.0f;

      int randomSize = GetRandomValue(0, 2);
      switch (randomSize)
      {
        case 0:
          asteroid->m_size                = ASTEROID_SIZE_LARGE;
          asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_LARGE_TOP_LEFT_X, ASTEROID_LARGE_TOP_LEFT_Y};
          asteroid->m_width               = ASTEROID_LARGE_WIDTH;
          asteroid->m_height              = ASTEROID_LARGE_HEIGHT;
          break;
        case 1:
          asteroid->m_size = ASTEROID_SIZE_MEDIUM;
          asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_MEDIUM_TOP_LEFT_X, ASTEROID_MEDIUM_TOP_LEFT_Y};
          asteroid->m_width               = ASTEROID_MEDIUM_WIDTH;
          asteroid->m_height              = ASTEROID_MEDIUM_HEIGHT;
          break;
        case 2:
          asteroid->m_size = ASTEROID_SIZE_SMALL;
          asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_SMALL_TOP_LEFT_X, ASTEROID_SMALL_TOP_LEFT_Y};
          asteroid->m_width               = ASTEROID_SMALL_WIDTH;
          asteroid->m_height              = ASTEROID_SMALL_HEIGHT;
          break;
      }
    }
}

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

void DrawAsteroids(const Texture2D* texture, Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  for (int i = 0; i < currentAsteroidCount; i++)
  {
    if (asteroidsArray[i].m_active)
    {
      Asteroid_t* asteroid = &asteroidsArray[i];

      DrawTexturePro( *texture,
        (Rectangle){ asteroid->m_image_location_x_y.x, 
                     asteroid->m_image_location_x_y.y, 
                     asteroid->m_width,
                     asteroid->m_height },
        (Rectangle){
          asteroid->m_position.x,
          asteroid->m_position.y,
          asteroid->m_width,
          asteroid->m_height},
        (Vector2){ asteroid->m_width / 2, asteroid->m_height / 2},
        0.0f,
        WHITE);
    }
  }
}

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids 2D");

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

  size_t currentAsteroidCount = 5;
  Asteroid_t* asteroidsArray = (Asteroid_t*)calloc(currentAsteroidCount, sizeof(Asteroid_t));
  if( asteroidsArray == NULL)
  {
    free(bulletsArray);
    CloseWindow();
    return -1;
  }

  Vector2 crosshairCenter = { 0 };

  float asteroidSpawnTimer = 0.0f;

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK);

    // -------------- UPDATING --------------
    crosshairCenter = GetClampedMousePosition(&crosshairTexture);
    UpdateSpaceshipRotation(&spaceship, crosshairCenter);
    UpdateSpaceshipPosition(&spaceship, GetFrameTime());


    // -------------- SPAWNING ASTEROIDS BY TIMER --------------
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= 5.0f)
    {
      asteroidSpawnTimer = 0.0f;
      Asteroid_t* tempAsteroidArray = realloc(asteroidsArray, (currentAsteroidCount + 1) * sizeof(Asteroid_t));
      if (tempAsteroidArray == NULL)
      {
        free(bulletsArray);
        CloseWindow();
        return -1;
      }
      asteroidsArray = tempAsteroidArray;
      ++currentAsteroidCount;
    }
    
    UpdateBulletsLocations(bulletsArray);
    UpdateAsteroids(asteroidsArray, currentAsteroidCount);

    // -------------- DRAWING --------------
    DrawSpaceship(&texture, &spaceship);
    DrawCrosshair(&crosshairTexture, crosshairCenter);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      FindFirstAvailableBulletAndShoot(bulletsArray, spaceship.m_center, crosshairCenter);

    DrawBullets(&texture, bulletsArray, crosshairCenter);
    DrawAsteroids(&texture, asteroidsArray, currentAsteroidCount);

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

