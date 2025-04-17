#include <pch.h>
#include "Render/Camera/GlobalCamera.h"

namespace Cober {

    Ref<Camera> GlobalCamera::camera = nullptr;
    bool GlobalCamera::perspective = false;

}