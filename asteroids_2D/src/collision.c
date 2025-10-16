#include "collision.h"
#include "raylib.h"

int Collision_CheckCollisionsBetweenBulletsAndAsteroids( Bullet_t* bulletsArray, 
                                                         Asteroid_t* asteroidsArray, 
                                                         size_t currentAsteroidCount)
{
  Rectangle bulletRect = { 0 };
  Rectangle asteroidRect = { 0 };
  int hitCount = 0;

  for (int i = 0; i < MAX_ACTIVE_BULLET_COUNT; i++)
  {
    if (bulletsArray[i].m_active)
    {
      bulletRect = (Rectangle){
        bulletsArray[i].m_position.x - BULLET_WIDTH / 2,
        bulletsArray[i].m_position.y - BULLET_HEIGHT / 2,
        BULLET_WIDTH,
        BULLET_HEIGHT
      };

      for (int j = 0; j < currentAsteroidCount; j++)
      {
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
            if (asteroidsArray[j].m_size == ASTEROID_SIZE_LARGE || 
                asteroidsArray[j].m_size == ASTEROID_SIZE_MEDIUM)
            {
              asteroidsArray[j].m_is_splitted = true;
            }
            asteroidsArray[j].m_active = false;
            bulletsArray[i].m_active = false;
            hitCount++;
            break;
          }
        }
      }
    }
  }
  
  return hitCount;
}