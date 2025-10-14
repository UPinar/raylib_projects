#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"


#define   ASTEROID_LARGE_TOP_LEFT_X   0.0F
#define   ASTEROID_LARGE_TOP_LEFT_Y   0.0F

#define   ASTEROID_MEDIUM_TOP_LEFT_X   0.0F
#define   ASTEROID_MEDIUM_TOP_LEFT_Y   96.0F

#define   ASTEROID_SMALL_TOP_LEFT_X   0.0F
#define   ASTEROID_SMALL_TOP_LEFT_Y   192.0F

#define   ASTEROID_LARGE_WIDTH        96.0F
#define   ASTEROID_LARGE_HEIGHT       96.0F

#define   ASTEROID_MEDIUM_WIDTH       64.0F
#define   ASTEROID_MEDIUM_HEIGHT      64.0F

#define   ASTEROID_SMALL_WIDTH        32.0F
#define   ASTEROID_SMALL_HEIGHT       32.0F
#define   ASTEROID_MIN_SPEED          50.0F  // Pixels per second
#define   ASTEROID_MAX_SPEED          150.0F // Pixels per second


typedef enum AsteroidSize
{
  ASTEROID_SIZE_LARGE,
  ASTEROID_SIZE_MEDIUM,
  ASTEROID_SIZE_SMALL
} AsteroidSize_t;

typedef struct Asteroid
{
  Vector2           m_direction;
  Vector2           m_position; 
  float             m_speed;
  float             m_lifetime;
  AsteroidSize_t    m_size;
  bool              m_active;
} Asteroid_t;

#endif // ASTEROID_H
