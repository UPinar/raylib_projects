#include "bullet.h"
#include "screen.h"

#include "raymath.h"    // Vector2Subtract, Vector2Normalize

void Bullet_UpdateBulletsLocations(Bullet_t* bulletsArray, float deltaTime)
{
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

void Bullet_FindFirstAvailableBulletAndShoot(Bullet_t* bulletsArray, Vector2 spaceshipPosition, Vector2 crosshairCenter)
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

void Bullet_DrawBullets(const Texture2D* texture, Bullet_t* bulletsArray, Vector2 crosshairCenter)
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