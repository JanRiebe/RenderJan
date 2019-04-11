
#include "Point.h"
#include "MathFunctions.h"
#include "DataDefinitions.h"

class Camera
{
private:
  float aspectRatio;
  int imageWidth, imageHeight;
  Matrix4x4 cameraToWorld;
  Point position, rotation;

public:
  float fieldOfView;

  Camera(Point pos = Point(0,0,-1000), Point rot= Point(0,0,0), float fov = PI/2, int width=768, int height=576);

  Ray PixelToRay(int x, int y);
};
