#include "Camera.h"
#include <math.h>
#include <iostream>

Camera::Camera(int width, int height, Point pos, Point rot, float fov): position(pos), rotation(rot), fieldOfView(fov), imageWidth(width), imageHeight(height)
{
  aspectRatio = imageWidth / imageHeight;
  cameraToWorld = MatMult(GetRotationMatrix(rot.x, rot.y, rot.z), pos.ToTranslationMatrix());
}


Ray Camera::PixelToRay(int x, int y)
{
  //return Ray(/*origin*/ Point( x - imageWidth / 2, y - imageHeight / 2,-100), /*direction*/ Point(0,0,1), /*initial IOR*/1.0);

  float Px = (2 * ((x + 0.5) / imageWidth) - 1) * tan(fieldOfView / 2 * PI / 180) * aspectRatio;
  float Py = (1 - 2 * ((y + 0.5) / imageHeight)) * tan(fieldOfView / 2 * PI / 180);

  Vec4 pixelWorldPos = VecMatMult({Px, Py, -1, 1} , cameraToWorld);
  Vec4 rayDirection = pixelWorldPos - position.ToVec4();
  rayDirection = HomogVec(rayDirection);
  Point direction = Point(rayDirection[0],rayDirection[1], rayDirection[2]);
  Point::Normalise(&direction);
  return Ray(Point( x - imageWidth / 2, y - imageHeight / 2,-100)/*position*/, direction, 1.0f);
}
