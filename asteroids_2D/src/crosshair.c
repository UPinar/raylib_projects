#include "crosshair.h"
#include "screen.h"    // SCREEN_WIDTH, SCREEN_HEIGHT

Vector2 Crosshair_GetCenterPosition(const Texture2D* crosshair)
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

void Crosshair_Draw(const Texture2D* texture, Vector2 crosshairCenter)
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