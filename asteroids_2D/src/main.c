#include "raylib.h"

#include "screen.h"
#include "crosshair.h"
#include "spaceship.h"
#include "bullet.h"
#include "asteroid.h"
#include "collision.h"

#include <stdio.h>  
#include <stdlib.h>     // calloc, free


// [-] TODO: Score tablosu eklenilecek.


void UpdateBackgroundImage(Texture2D* galaxyTexture, float* backgroundScrollOffset);
void DrawBackgroundImage(Texture2D* texture, float scrollOffset);

void CleanUp( Bullet_t* bulletsArray, 
              Asteroid_t* asteroidsArray, 
              Texture2D* texture, 
              Texture2D* crosshairTexture, 
              Texture2D* galaxyTexture);

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids 2D");

  int monitorCount = GetMonitorCount();
  if (monitorCount > 1)
    SetWindowMonitor(1);
  
  // SetTargetFPS(SCREEN_FPS);
  // ToggleBorderlessWindowed();
  DisableCursor();

  bool shouldRestart = true;

  Texture2D texture = { 0 };     
  Texture2D crosshairTexture = { 0 };
  Texture2D galaxyTexture = { 0 };

  Spaceship_t spaceship = { 0 };

  Bullet_t* bulletsArray = nullptr; 
  Asteroid_t* asteroidsArray = nullptr;
  size_t currentAsteroidCount = ASTEROID_SPAWN_COUNT;

  Vector2 crosshairCenter = { 0 };
  float backgroundScrollOffset = 0.0f;
  float deltaTime = 0.0f;

  while (shouldRestart != false)
  {
    shouldRestart = false;

    SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    texture           = LoadTexture("assets/asteroids.png");
    crosshairTexture  = LoadTexture("assets/crosshair.png");
    galaxyTexture     = LoadTexture("assets/galaxy.jpg");

    spaceship = (Spaceship_t){
      .m_center = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
      .m_rotation_angle = 0.0f,
    };

    bulletsArray = (Bullet_t*)calloc(MAX_ACTIVE_BULLET_COUNT, sizeof(Bullet_t));
    if (bulletsArray == NULL)
    {
      CloseWindow();
      return -1;
    }

    asteroidsArray = (Asteroid_t*)calloc(ASTEROID_SPAWN_COUNT, sizeof(Asteroid_t));
    if( asteroidsArray == NULL)
    {
      CloseWindow();
      return -1;
    }
    currentAsteroidCount = ASTEROID_SPAWN_COUNT;

    for (int i = 0; i < ASTEROID_SPAWN_COUNT; i++)
    Asteroid_SetAsteroidProperties(&asteroidsArray[i]);
    

    while (!WindowShouldClose())
    {
      deltaTime = GetFrameTime();

      BeginDrawing();
      ClearBackground(BLACK);

      // -------------- UPDATING --------------
      crosshairCenter = Crosshair_GetCenterPosition(&crosshairTexture);

      UpdateBackgroundImage(&galaxyTexture, &backgroundScrollOffset);

      Spaceship_UpdateRotation(&spaceship, crosshairCenter);
      Spaceship_UpdatePosition(&spaceship, deltaTime);
      Asteroid_UpdateAsteroids(&asteroidsArray, &currentAsteroidCount, deltaTime);
      Bullet_UpdateBulletsLocations(bulletsArray, deltaTime);

      shouldRestart = Spaceship_CheckIfSpaceshipIsDead(&spaceship, asteroidsArray, currentAsteroidCount);
      if (shouldRestart)
        break;

      // -------------- SPAWNING ASTEROIDS BY TIMER --------------
      asteroid_SpawnTimer += deltaTime;
      if (asteroid_SpawnTimer >= ASTEROID_SPAWN_TIMER)
      {
        asteroid_SpawnTimer = 0.0f;
        Asteroid_t* new_asteroid = Asteroid_GenerateNewAsteroid(&asteroidsArray, &currentAsteroidCount);
        Asteroid_SetAsteroidProperties(new_asteroid);
      }
      // ---------------------------------------------------------

      Asteroid_UpdateAsteroids(&asteroidsArray, &currentAsteroidCount, deltaTime);

      Collision_CheckCollisionsBetweenBulletsAndAsteroids(bulletsArray, asteroidsArray, currentAsteroidCount);


      // -------------- DRAWING --------------
      DrawBackgroundImage(&galaxyTexture, backgroundScrollOffset);

      Spaceship_Draw(&texture, &spaceship);
      Crosshair_Draw(&crosshairTexture, crosshairCenter);

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        Bullet_FindFirstAvailableBulletAndShoot(bulletsArray, spaceship.m_center, crosshairCenter);
        
      Bullet_DrawBullets(&texture, bulletsArray, crosshairCenter);
      Asteroid_DrawAsteroids(&texture, asteroidsArray, currentAsteroidCount);
      
      DrawFPS(10, 10);
      EndDrawing();
    }

    CleanUp(bulletsArray, asteroidsArray, &texture, &crosshairTexture, &galaxyTexture);
  }

  CloseWindow();
  return 0;
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

void CleanUp( Bullet_t* bulletsArray, 
              Asteroid_t* asteroidsArray, 
              Texture2D* texture, 
              Texture2D* crosshairTexture, 
              Texture2D* galaxyTexture)
{
  free(bulletsArray);
  free(asteroidsArray);
  UnloadTexture(*texture);
  UnloadTexture(*crosshairTexture);
  UnloadTexture(*galaxyTexture);
}

void UpdateBackgroundImage(Texture2D* galaxyTexture, float* backgroundScrollOffset)
{
  *backgroundScrollOffset += GetFrameTime() * 90.0f; // Scroll speed in pixels per second
  if (*backgroundScrollOffset >= galaxyTexture->width)
    *backgroundScrollOffset = 0.0f;
}