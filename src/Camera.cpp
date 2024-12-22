#include "Camera.h"

Camera::Camera()
{}

Camera::~Camera()
{}

void Camera::Rotate(const F32QUAT qRot)
{}

void Camera::Rotate(const FMAT4 mRotate)
{}

void Camera::Translate(const FVEC3 vTrans)
{}

void Camera::Translate(const FMAT4 mTranslate)
{}

void Camera::Init(const FVEC3 vEye, const FVEC3 vAt)
{
    m_vEye = vEye;
    m_vAt = vAt;

    FVEC3 vUp = FVEC3(0.0f, 1.0f, 0.0f);

    FVEC3 vNorN = glm::normalize(m_vEye - m_vAt);
    FVEC3 vNorU = glm::normalize(glm::cross(vUp, vNorN));
    FVEC3 vNorV = glm::normalize(glm::cross(vNorN, vNorU));

    FMAT4 mRot = FMAT4(
        FVEC4(vNorU, 0.0f),
        FVEC4(vNorV, 0.0f),
        FVEC4(vNorN, 0.0f),
        FVEC4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    FMAT4 mTrans = glm::translate(FMAT4(1.0f), m_vEye);

    const auto mInverseRot = glm::inverse(mRot);
    const auto mInverseTrans = glm::inverse(mTrans);
    m_mView = glm::inverse(mRot) * glm::inverse(mTrans);
}

void Camera::Init(const FrustumVolume tFrustVolume, const ViewVolume tViewVolume)
{
    m_FrustVolume = tFrustVolume;
    m_ViewVolume = tViewVolume;

    if (m_FrustVolume.fAspect <= FLT_EPSILON)
    {
        m_FrustVolume.fAspect = 1.0f;
    }

    if (m_FrustVolume.fFovY <= FLT_EPSILON || abs(m_FrustVolume.fFovY - 180.0f) <= FLT_EPSILON)
    {
        m_FrustVolume.fFovY = 90.0f;
    }

    if (abs(m_FrustVolume.nFarPlane - m_FrustVolume.nNearPlane) <= FLT_EPSILON)
    {
        m_FrustVolume.nFarPlane = 1000.0f;        
        m_FrustVolume.nNearPlane = 1.0f;
    }
    else
    {
        if (m_FrustVolume.nFarPlane <= FLT_EPSILON)
        {
            m_FrustVolume.nFarPlane = 1000.0f;
        }

        if (m_FrustVolume.nNearPlane <= FLT_EPSILON)
        {
            m_FrustVolume.nNearPlane = 1.0f;
        }
    }

    if (m_ViewVolume.nWidht == 0)
    {
        m_ViewVolume.nWidht = 1080;
    }

    if (m_ViewVolume.nHeight == 0)
    {
        m_ViewVolume.nHeight = 800;
    }

    FLOAT32 fCotFovy = 1 / glm::tan(m_FrustVolume.fFovY / 2.0f);

    FVEC4 vRow1 = FVEC4( fCotFovy / m_FrustVolume.fAspect, 0.0f, 0.0f, 0.0f);
    FVEC4 vRow2 = FVEC4( 0.0f, fCotFovy, 0.0f, 0.0f);
    FVEC4 vRow3 = FVEC4( 0.0f, 0.0f, (-1.0f) * (m_FrustVolume.nFarPlane + m_FrustVolume.nNearPlane)/(m_FrustVolume.nFarPlane - m_FrustVolume.nNearPlane), -1.0f);
    FVEC4 vRow4 = FVEC4( 0.0f, 0.0f, (-1.0f) * (2.0f * m_FrustVolume.nFarPlane * m_FrustVolume.nNearPlane) / (m_FrustVolume.nFarPlane - m_FrustVolume.nNearPlane), 0.0f);

    m_mProj = FMAT4(
        vRow1, vRow2, vRow3, vRow4
    );

    const auto mTrans = glm::translate(FMAT4(1.0f), FVEC3(1.0f, 2.0f, 3.0f));
    const auto mOtherProj = glm::perspective(m_FrustVolume.fFovY, m_FrustVolume.fAspect, m_FrustVolume.nNearPlane, m_FrustVolume.nFarPlane);
}

FMAT4 Camera::GetViewMatrix()
{
    return m_mView;
}

FMAT4 Camera::GetProjMatrix()
{
    return m_mProj;
}

const Camera::ViewVolume Camera::GetViewVolume()
{
    return m_ViewVolume;
}

const Camera::FrustumVolume Camera::GetFrustumVolume()
{
    return m_FrustVolume;
}