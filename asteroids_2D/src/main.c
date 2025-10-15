#include "raylib.h"
#include "raymath.h"    // Vector2Subtract

#include "spaceship.h"
#include "bullet.h"
#include "asteroid.h"

#include <stdio.h>      // printf
#include <stdlib.h>     // calloc, free
#include <stdbool.h>    // bool
#include <math.h>       // atan2f, sqrtf

#define SCREEN_WIDTH          1920
#define SCREEN_HEIGHT         1080
#define SCREEN_FPS            144

#define CROSSHAIR_SCALE       1.5f

#define DEBUG

// [+] TODO: Olme mekanigi eklenilecek.
// [-] TODO: Asteroidlerin LARGE -> 2 MEDIUM -> 2 SMALL -> 0 sekilde bolunmesi eklenilecek.
// [-] TODO: Asteroidlerin rotasyon mekanigi eklenilecek
// [-] TODO: Isinlanma mekanigi eklenilecek
// [-] TODO: Score tablosu eklenilecek.

Vector2 GetClampedMousePosition(const Texture2D* crosshair)
{
  float crosshairHalfWidth   = (float)crosshair->width * CROSSHAIR_SCALE / 2.0f;
  float crosshairHalfHeight  = (float)crosshair->height * CROSSHAIR_SCALE / 2.0f;

  float mouseX = (float)GetMouseX();
  float mouseY = (float)GetMouseY();

  if (mouseX < crosshairHalfWidth) 
    mouseX = crosshairHalfWidth;
  else if (mouseX > (float)SCREEN_WIDTH - crosshairHalfWidth) 
    mouseX = (float)SCREEN_WIDTH - crosshairHalfWidth;

  if (mouseY < crosshairHalfHeight) 
    mouseY = crosshairHalfHeight;
  else if (mouseY > (float)SCREEN_HEIGHT - crosshairHalfHeight) 
    mouseY = (float)SCREEN_HEIGHT - crosshairHalfHeight;

  return (Vector2){ mouseX, mouseY };
}

void UpdateSpaceshipRotation(Spaceship_t* spaceship, Vector2 crosshairCenter)
{
  Vector2 centerOfTheScreen = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
  Vector2 spaceshipDirection = Vector2Subtract(crosshairCenter, centerOfTheScreen);

  // atan2f returns radians; convert to degrees for DrawTexturePro
  float rotationAngle = atan2f(spaceshipDirection.y, spaceshipDirection.x) * (180.0f / PI);

  // Raylib's rotation is clockwise-positive when using DrawTexturePro angles in degrees
  spaceship->m_rotation_angle = rotationAngle;
}

void UpdateBulletsLocations(Bullet_t* bulletsArray)
{
  float deltaTime = GetFrameTime();
  Bullet_t* bullet = nullptr;

  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {
      bullet = &bulletsArray[i];

      // Update bullet position
      bullet->m_position.x += bullet->m_direction.x * BULLET_SPEED * deltaTime;
      bullet->m_position.y += bullet->m_direction.y * BULLET_SPEED * deltaTime;

      // Deactivate bullet when it goes off-screen
      if (bullet->m_position.x < 0              || 
          bullet->m_position.x > SCREEN_WIDTH   ||
          bullet->m_position.y < 0              || 
          bullet->m_position.y > SCREEN_HEIGHT)
      {
        bullet->m_active = false;
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
    case 0: // left side
      x = GetRandomValue(-200, 0);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 1: // right side
      x = GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 2: // top side
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, 0);
      break;
    case 3: // bottom side
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(SCREEN_HEIGHT, SCREEN_HEIGHT + 200);
      break;
  }

  return (Vector2){ x, y };
}

static void DeactivateAsteroidsIfOutOfScreen(Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  float deltaTime = GetFrameTime();

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
}

static void SpawnNewAsteroidsIfThereAreInactive(Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
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

void UpdateAsteroids(Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  DeactivateAsteroidsIfOutOfScreen(asteroidsArray, currentAsteroidCount);
  SpawnNewAsteroidsIfThereAreInactive(asteroidsArray, currentAsteroidCount);
}

void CheckCollisionsBetweenBulletsAndAsteroids( Bullet_t* bulletsArray, 
                                                Asteroid_t* asteroidsArray, 
                                                size_t currentAsteroidCount)
{
  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {
      Rectangle bulletRect = {
        bulletsArray[i].m_position.x - BULLET_WIDTH / 2,
        bulletsArray[i].m_position.y - BULLET_HEIGHT / 2,
        BULLET_WIDTH,
        BULLET_HEIGHT
      };

      Rectangle asteroidRect = { 0 };

      for (int j = 0; j < currentAsteroidCount; j++)
        if (asteroidsArray[j].m_active)
        {
          asteroidRect = (Rectangle){
            asteroidsArray[j].m_position.x - asteroidsArray[j].m_width / 2,
            asteroidsArray[j].m_position.y - asteroidsArray[j].m_height / 2,
            asteroidsArray[j].m_width,
            asteroidsArray[j].m_height
          };

          if (CheckCollisionRecs(bulletRect, asteroidRect))
          {
            bulletsArray[i].m_active = false;
            asteroidsArray[j].m_active = false;
            break;
          }
        }
    }
  }
}

void CheckIfSpaceshipIsDead(Spaceship_t* spaceship, 
                            Asteroid_t* asteroidsArray, 
                            size_t currentAsteroidCount, 
                            bool* shouldRestart)
{
  Rectangle spaceshipRect = {
    spaceship->m_center.x - SPACESHIP_WIDTH / 2,
    spaceship->m_center.y - SPACESHIP_HEIGHT / 2,
    SPACESHIP_WIDTH,
    SPACESHIP_HEIGHT
  };

  Rectangle asteroidRect = { 0 };

  for (int i = 0; i < currentAsteroidCount; i++)
  {
    if (asteroidsArray[i].m_active)
    {
      asteroidRect = (Rectangle){
        asteroidsArray[i].m_position.x - asteroidsArray[i].m_width / 2,
        asteroidsArray[i].m_position.y - asteroidsArray[i].m_height / 2,
        asteroidsArray[i].m_width,
        asteroidsArray[i].m_height
      };

      if (CheckCollisionRecs(spaceshipRect, asteroidRect))
        *shouldRestart = true;
    }
  }
}

void DrawBackgroundImage(Texture2D* texture, float scrollOffset)
{
  // Draw the background with horizontal scrolling effect
  // We draw the texture twice to create a seamless loop
  
  int textureWidth = texture->width;
  int textureHeight = texture->height;

  // Calculate how many times we need to tile the texture to cover the screen width
  int tilesNeeded = (SCREEN_WIDTH / textureWidth) + 2; 

  for (int i = 0; i < tilesNeeded; i++)
  {
    float xPos = (i * textureWidth) - scrollOffset;

    // Draw the texture scaled to fit screen height, maintaining aspect ratio
    DrawTexturePro(*texture,
                   (Rectangle){ 0.0f, 0.0f, (float)textureWidth, (float)textureHeight },
                   (Rectangle){ xPos, 0.0f, (float)textureWidth, (float)SCREEN_HEIGHT },
                   (Vector2){ 0.0f, 0.0f },
                   0.0f,
                   WHITE);
  }
}

void DrawSpaceship(const Texture2D* texture, Spaceship_t* spaceship)
{
  DrawTexturePro( *texture,
    (Rectangle){SPACESHIP_TOP_LEFT_X, 
                SPACESHIP_TOP_LEFT_Y, 
                SPACESHIP_WIDTH,
                SPACESHIP_HEIGHT },
    (Rectangle){
      spaceship->m_center.x,
      spaceship->m_center.y,
      SPACESHIP_WIDTH,
      SPACESHIP_HEIGHT},
    (Vector2){SPACESHIP_WIDTH / 2, SPACESHIP_HEIGHT / 2},
    spaceship->m_rotation_angle,
    (Color){ 247, 102, 54, 255 }
  );
}

void DrawCrosshair(const Texture2D* texture, Vector2 crosshairCenter)
{
  float crosshairWidth = (float)texture->width * CROSSHAIR_SCALE;
  float crosshairHeight = (float)texture->height * CROSSHAIR_SCALE;

  DrawTexturePro( *texture,
                  (Rectangle){ 0.0f, 0.0f, (float)texture->width, (float)texture->height },
                  (Rectangle){ crosshairCenter.x, crosshairCenter.y, crosshairWidth, crosshairHeight },
                  (Vector2){ crosshairWidth / 2.0f, crosshairHeight / 2.0f },
                  0.0f,
                  WHITE);

  DrawPixel(crosshairCenter.x, crosshairCenter.y, WHITE);

  /*
    DrawRectangleLines((int)(crosshairCenter.x - crosshairWidth / 2.0f),
                      (int)(crosshairCenter.y - crosshairHeight / 2.0f),
                      (int)crosshairWidth,
                      (int)crosshairHeight,
                      GREEN);
  */
}

void FindFirstAvailableBulletAndShoot(Bullet_t* bulletsArray, Vector2 spaceshipPosition, Vector2 crosshairCenter)
{  
  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
    if (!bulletsArray[i].m_active)
    {
      bulletsArray[i].m_active = true;
      bulletsArray[i].m_position = spaceshipPosition;
      bulletsArray[i].m_direction = Vector2Normalize(Vector2Subtract(crosshairCenter, spaceshipPosition));
      break;
    }
}

void DrawBullets(const Texture2D* texture, Bullet_t* bulletsArray, Vector2 crosshairCenter)
{
  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
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
      
      /*
      DrawRectangleLines(
        (int)(bulletsArray[i].m_position.x - BULLET_WIDTH / 2),
        (int)(bulletsArray[i].m_position.y - BULLET_HEIGHT / 2),
        (int)BULLET_WIDTH,
        (int)BULLET_HEIGHT,
        GREEN);
      */
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
        (Color){ 162, 242, 16, 255 });
    }
  }
}

void CleanUp(Bullet_t* bulletsArray, Asteroid_t* asteroidsArray, Texture2D* texture, Texture2D* crosshairTexture, Texture2D* galaxyTexture)
{
  free(bulletsArray);
  free(asteroidsArray);
  UnloadTexture(*texture);
  UnloadTexture(*crosshairTexture);
  UnloadTexture(*galaxyTexture);
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

  bool shouldRestart = true;

  while (!WindowShouldClose() && shouldRestart)
  {
    shouldRestart = false;
    
    Texture2D texture = LoadTexture("assets/asteroids.png");
    Texture2D crosshairTexture = LoadTexture("assets/crosshair.png");
    Texture2D galaxyTexture = LoadTexture("assets/galaxy.jpg");

    Spaceship_t spaceship = {
      .m_center = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
      .m_rotation_angle = 0.0f,
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
    float backgroundScrollOffset = 0.0f;
    float backgroundScrollSpeed = 120.0f; // Pixels per second

    while (!WindowShouldClose() && !shouldRestart)
    {
      BeginDrawing();
      ClearBackground(BLACK);

      // -------------- UPDATING --------------
      crosshairCenter = GetClampedMousePosition(&crosshairTexture);
      UpdateSpaceshipRotation(&spaceship, crosshairCenter);
      UpdateSpaceshipPosition(&spaceship, GetFrameTime());

      // -------------- SPAWNING ASTEROIDS BY TIMER --------------
      asteroidSpawnTimer += GetFrameTime();

      if (asteroidSpawnTimer >= ASTEROID_SPAWN_TIMER)
      {
        asteroidSpawnTimer = 0.0f;

        Asteroid_t* tempAsteroidArray = realloc(asteroidsArray, (++currentAsteroidCount) * sizeof(Asteroid_t));
        if (tempAsteroidArray == NULL)
        {
          free(asteroidsArray);
          free(bulletsArray);
          CloseWindow();
          return -1;
        }
        asteroidsArray = tempAsteroidArray;
      }
      // ---------------------------------------------------------
      
      UpdateBulletsLocations(bulletsArray);
      UpdateAsteroids(asteroidsArray, currentAsteroidCount);

      CheckCollisionsBetweenBulletsAndAsteroids(bulletsArray, asteroidsArray, currentAsteroidCount);
      CheckIfSpaceshipIsDead(&spaceship, asteroidsArray, currentAsteroidCount, &shouldRestart);

      // Update background scroll
      backgroundScrollOffset += backgroundScrollSpeed * GetFrameTime();
      // Wrap around when offset exceeds texture width for seamless loop
      if (backgroundScrollOffset >= galaxyTexture.width)
        backgroundScrollOffset -= galaxyTexture.width;
      
      // -------------- DRAWING --------------
      DrawBackgroundImage(&galaxyTexture, backgroundScrollOffset);

      DrawSpaceship(&texture, &spaceship);
      DrawCrosshair(&crosshairTexture, crosshairCenter);

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        FindFirstAvailableBulletAndShoot(bulletsArray, spaceship.m_center, crosshairCenter);
        
      DrawBullets(&texture, bulletsArray, crosshairCenter);
      DrawAsteroids(&texture, asteroidsArray, currentAsteroidCount);
      
      /*
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, RED);
        DrawLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, RED);
        DrawLine( spaceship.m_center.x, 
                  spaceship.m_center.y, 
                  (int)crosshairCenter.x, 
                  (int)crosshairCenter.y, 
                  YELLOW);
        DrawFPS(10, 10);
      */

      EndDrawing();
    }

    // Clean up before restart
    CleanUp(bulletsArray, asteroidsArray, &texture, &crosshairTexture, &galaxyTexture);
  }

  CloseWindow();
  return 0;
}

