#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Type.h"
#include "GeoType.h"

class Camera
{
    public :
        struct FrustumVolume
        {
            FLOAT32 nNearPlane;
            FLOAT32 nFarPlane;

            FLOAT32 fFovY;
            FLOAT32 fAspect;
        };

        struct ViewVolume
        {
            UINT16 nPosX;
            UINT16 nPoxY;

            UINT16 nWidht;
            UINT16 nHeight;
        };

    public : 
        Camera();
        virtual ~Camera();

    public :
        void Rotate(const F32QUAT qRot);
        void Rotate(const FMAT4 mRotate);

        void Translate(const FVEC3 vTrans);
        void Translate(const FMAT4 mTranslate);
        
        void Init(const FVEC3 vEye, const FVEC3 vAt);
        void Init(const FrustumVolume tFrustVolume, const ViewVolume tViewVolume);

        FMAT4 GetViewMatrix();
        FMAT4 GetProjMatrix();        
        const ViewVolume GetViewVolume();
        const FrustumVolume GetFrustumVolume();

    private :
        FVEC3 m_vEye;
        FVEC3 m_vAt;

        FMAT4 m_mView;
        FMAT4 m_mProj;

        ViewVolume m_ViewVolume;
        FrustumVolume m_FrustVolume;
};

#endif // __CAMERA_H__