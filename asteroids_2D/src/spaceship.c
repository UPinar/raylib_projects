#include "spaceship.h"
#include "raymath.h"      // Vector2Normalize

#define SCREEN_WIDTH          1920
#define SCREEN_HEIGHT         1080

static void CheckAndLimitSpaceshipBounds(Spaceship_t* spaceship)
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

void SpaceshipGoRight(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  CheckAndLimitSpaceshipBounds(spaceship);
}

void SpaceshipGoLeft(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  CheckAndLimitSpaceshipBounds(spaceship);
}

void SpaceshipGoUp(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  CheckAndLimitSpaceshipBounds(spaceship);
}

void SpaceshipGoDown(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  CheckAndLimitSpaceshipBounds(spaceship);
}