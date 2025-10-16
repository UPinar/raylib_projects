#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"

#include "stddef.h"   // size_t

#define   ASTEROID_LARGE_TOP_LEFT_X       0.0F
#define   ASTEROID_LARGE_TOP_LEFT_Y       0.0F
#define   ASTEROID_LARGE_WIDTH            160.0F
#define   ASTEROID_LARGE_HEIGHT           160.0F

#define   ASTEROID_MEDIUM_TOP_LEFT_X      0.0F
#define   ASTEROID_MEDIUM_TOP_LEFT_Y      160.0F
#define   ASTEROID_MEDIUM_WIDTH           96.0F
#define   ASTEROID_MEDIUM_HEIGHT          96.0F

#define   ASTEROID_SMALL_TOP_LEFT_X       0.0F
#define   ASTEROID_SMALL_TOP_LEFT_Y       256.0F
#define   ASTEROID_SMALL_WIDTH            64.0F
#define   ASTEROID_SMALL_HEIGHT           64.0F
    
#define   ASTEROID_MIN_SPEED              70.0F  
#define   ASTEROID_MAX_SPEED              150.0F

#define   ASTEROID_SPAWN_TIMER            4.0F

#define   ASTEROID_SPAWN_COUNT            5


typedef enum AsteroidSize {
  ASTEROID_SIZE_LARGE,
  ASTEROID_SIZE_MEDIUM,
  ASTEROID_SIZE_SMALL
} AsteroidSize_t;

typedef struct Asteroid {
  Vector2           m_direction;
  Vector2           m_position; 
  Vector2           m_image_location_x_y;
  float             m_width;
  float             m_height;
  float             m_speed;
  float             m_lifetime;
  float             m_rotation_angle; 
  float             m_rotation_speed; 
  AsteroidSize_t    m_size;
  bool              m_active;
  bool              m_is_splitted;
} Asteroid_t;

extern float asteroid_SpawnTimer;

void Asteroid_SetAsteroidProperties(Asteroid_t* asteroid);
void Asteroid_UpdateAsteroids(Asteroid_t** p_asteroidsArray, size_t* p_currentAsteroidCount, float deltaTime);
Asteroid_t* Asteroid_GenerateNewAsteroid(Asteroid_t** p_asteroidsArray, size_t* p_currentAsteroidCount);
void Asteroid_DrawAsteroids(const Texture2D* texture, Asteroid_t* asteroidsArray, size_t currentAsteroidCount);

#endif // ASTEROID_H
