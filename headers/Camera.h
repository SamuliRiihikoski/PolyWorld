#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"


class Camera 
{
public:

    glm::mat4 getViewMatrix() const;
    virtual void rotate(float yaw, float pitch) {}

protected:
    Camera();

    glm::vec3 mPosition;
    glm::vec3 mTargetPos;
    glm::vec3 mUp;

    // Eulers angles (in radians)
    float mYaw;
    float mPitch;
};

inline Camera::Camera() : 
    mPosition(glm::vec3(0.0f, 0.0f, 3.0f)),
    mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
    mUp(0.0f, 1.0f, 0.0f),
    mYaw(0.0f), mPitch(0.0f) {}
                          
inline glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(mPosition, mTargetPos, mUp);
}

/****  ORBIT CAMERA ****/

class OrbitCamera : public Camera
{
public:
    
    OrbitCamera();

    virtual void rotate(float yaw, float pitch);
    void setLookAt(const glm::vec3& target);
    void setRadius(float radius);

private:

    void updateCameraVectors();
    float mRadius;


};

inline OrbitCamera::OrbitCamera() :
    mRadius(10.0f) {}

inline void OrbitCamera::setLookAt(const glm::vec3& target)
{
    mTargetPos = target;
}

inline void OrbitCamera::setRadius(float radius) 
{
    mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

inline void OrbitCamera::rotate(float yaw, float pitch)
{
    mYaw = glm::radians(yaw);
    mPitch = glm::radians(pitch);

    mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    updateCameraVectors();
}

inline void OrbitCamera::updateCameraVectors()
{
    mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
    mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
    mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}

#endif