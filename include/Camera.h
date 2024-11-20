#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Type.h"
#include "GeoType.h"

class Camera
{
    struct FrustumVolume
    {

    };

    public : 
        Camera();
        virtual ~Camera();

    public :
        void Init(const FVEC3 vEye, const FVEC3 vAt);
        FMAT4 GetViewMatrix();
        FMAT4 GetProjMatrix();        

    private :
        FVEC3 m_vEye;
        FVEC3 m_vAt;

        FrustumVolume m_ViewVolume;
};

#endif // __CAMERA_H__