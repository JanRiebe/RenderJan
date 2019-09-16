
#include "Point.h"
#include "MathFunctions.h"
#include "DataDefinitions.h"

class Camera : public SceneElement
{
private:
  float aspectRatio;
  Matrix4x4 cameraToWorld;

public:
  float fieldOfView;
  int imageWidth, imageHeight;

  Camera(int width=768, int height=576, Point pos = Point(0,0,-100), Point rot= Point(0,PI,0), float fov = 90);

  Ray PixelToRay(int x, int y);
};
