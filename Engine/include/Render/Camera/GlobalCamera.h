#ifndef GLOBAL_CAMERA_H
#define GLOBAL_CAMERA_H
#endif

#include "Render/Camera/Camera.h"

namespace Cober {

    class CB_API GlobalCamera : public Camera
    {
    public:
        static Ref<Camera> camera;
        static bool perspective;
    };
}