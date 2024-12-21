#include "Camera.h"

Camera::Camera()
{}

Camera::~Camera()
{}

void Camera::Init(const FVEC3 vEye, const FVEC3 vAt)
{}

FMAT4 Camera::GetViewMatrix()
{
    return FMAT4();
}

FMAT4 Camera::GetProjMatrix()
{
    return FMAT4();
}