
#include "Point.h"
#include "MathFunctions.h"
#include "DataDefinitions.h"

class Camera
{
private:
  float aspectRatio;
  Matrix4x4 cameraToWorld;
  Point position, rotation;

public:
  float fieldOfView;
  int imageWidth, imageHeight;

  Camera(int width=768, int height=576, Point pos = Point(0,0,-100), Point rot= Point(0,PI,0), float fov = PI/2);

  Ray PixelToRay(int x, int y);
};
