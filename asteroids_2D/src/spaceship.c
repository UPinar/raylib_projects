#include "spaceship.h"

#include "raymath.h"      // Vector2Normalize, Vector2Subtract
#include "math.h"         // atan2f

#define SCREEN_WIDTH          1920
#define SCREEN_HEIGHT         1080

void Spaceship_UpdateRotation(Spaceship_t* spaceship, Vector2 crosshairCenter)
{
  Vector2 centerOfTheScreen = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
  Vector2 spaceshipDirection = Vector2Subtract(crosshairCenter, centerOfTheScreen);

  // atan2f returns radians; convert to degrees for DrawTexturePro
  float rotationAngle = atan2f(spaceshipDirection.y, spaceshipDirection.x) * (180.0f / PI);

  // Raylib's rotation is clockwise-positive when using DrawTexturePro angles in degrees
  spaceship->m_rotation_angle = rotationAngle;
}

static void Spaceship_CheckAndLimitBounds(Spaceship_t* spaceship)
{
  float halfWidth = SPACESHIP_WIDTH / 2.0f;
  float halfHeight = SPACESHIP_HEIGHT / 2.0f;

  if (spaceship->m_center.x + halfWidth > SCREEN_WIDTH)
    spaceship->m_center.x = SCREEN_WIDTH - halfWidth;
  if (spaceship->m_center.y + halfHeight > SCREEN_HEIGHT)
    spaceship->m_center.y = SCREEN_HEIGHT - halfHeight;
  if (spaceship->m_center.x - halfWidth < 0)
    spaceship->m_center.x = halfWidth;
  if (spaceship->m_center.y - halfHeight < 0)
    spaceship->m_center.y = halfHeight;
}

static void Spaceship_GoRight(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(spaceship);
}

static void Spaceship_GoLeft(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(spaceship);
}

static void Spaceship_GoUp(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(spaceship);
}

static void Spaceship_GoDown(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(spaceship);
}

void Spaceship_UpdatePosition(Spaceship_t* spaceship, float deltaTime)
{
  if (IsKeyDown(KEY_D))
    Spaceship_GoRight(spaceship, deltaTime);
  if (IsKeyDown(KEY_A))
    Spaceship_GoLeft(spaceship, deltaTime);
  if (IsKeyDown(KEY_W))
    Spaceship_GoUp(spaceship, deltaTime);
  if (IsKeyDown(KEY_S))
    Spaceship_GoDown(spaceship, deltaTime);
}

bool Spaceship_CheckIfSpaceshipIsDead(Spaceship_t* spaceship, 
                                      Asteroid_t* asteroidsArray, 
                                      size_t currentAsteroidCount)
{
  Rectangle spaceshipRect = {
    spaceship->m_center.x - SPACESHIP_WIDTH / 2.0f,
    spaceship->m_center.y - SPACESHIP_HEIGHT / 2.0f,
    SPACESHIP_WIDTH,
    SPACESHIP_HEIGHT
  };

  Asteroid_t* asteroid = nullptr;
  Rectangle asteroidRect = { 0 };
  for (int i = 0; i < currentAsteroidCount; i++)
  {
    asteroid = &asteroidsArray[i];
    if (asteroid->m_active)
    {
      asteroidRect = (Rectangle){
        asteroid->m_position.x - asteroid->m_width / 2.0f,
        asteroid->m_position.y - asteroid->m_height / 2.0f,
        asteroid->m_width,
        asteroid->m_height
      };

      if (CheckCollisionRecs(spaceshipRect, asteroidRect))
        return true;
    }
  }
  return false;
}

void Spaceship_Draw(const Texture2D* texture, Spaceship_t* spaceship)
{
  DrawTexturePro( *texture,
                  (Rectangle){ SPACESHIP_TOP_LEFT_X, 
                               SPACESHIP_TOP_LEFT_Y, 
                               SPACESHIP_WIDTH,
                               SPACESHIP_HEIGHT },
                  (Rectangle){ spaceship->m_center.x, 
                               spaceship->m_center.y,
                               SPACESHIP_WIDTH,
                               SPACESHIP_HEIGHT },
                  (Vector2){ SPACESHIP_WIDTH / 2.0f, SPACESHIP_HEIGHT / 2.0f },
                  spaceship->m_rotation_angle,
                  (Color){ 247, 102, 54, 255 });

  /*
    DrawRectangleLines((int)(spaceship->m_center.x - SPACESHIP_WIDTH / 2.0f),
                      (int)(spaceship->m_center.y - SPACESHIP_HEIGHT / 2.0f),
                      (int)SPACESHIP_WIDTH,
                      (int)SPACESHIP_HEIGHT,
                      GREEN);
  */
}