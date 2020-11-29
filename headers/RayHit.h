#ifndef RAYHIT_H
#define RAYHIT_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "App.h"

class RayHit
{
public:
    //float pointInsideTriangle(); // -1 = background, other wise returns polygon ID.

    RayHit(double& xpos, double& ypos, int& appWidth, int& appHeight, 
           glm::mat4& view, glm::mat4& projection, glm::mat4& model, OrbitCamera& orbitCamera, App& app) :
           xpos(xpos), ypos(ypos), appWidth(appWidth), appHeight(appHeight), view(view), projection(projection),
           model(model), orbitCamera(orbitCamera), app(app) 
           {
               t = 0.0f;
           }
    pair<float, unsigned int> rayPlaneHitPoint();

private:
    float pointOnTriangle(glm::vec3& point, glm::vec3& n, 
                          glm::vec3& p1, glm::vec3& p2, glm::vec3& p3); 
    
    double& xpos, ypos;
    int& appWidth, appHeight;
    glm::mat4& view, projection, model;
    OrbitCamera& orbitCamera;
    App& app;
    float t;
};

inline pair<float, unsigned int> RayHit::rayPlaneHitPoint()
{
    Mesh mesh = app.getScene(0).getMesh(0);
    float minDistance = FLT_MAX;
    unsigned int faceID = -1;
    pair<float, unsigned> results;
    float tt = FLT_MAX;
    float distance = FLT_MAX;

    for (int i = 0; i < mesh.FaceList.size(); i++)
    {   
        if (mesh.FaceList[i].edge == nullptr)
            continue;

        Vert* i0 = mesh.FaceList[i].edge->vertex;
        Vert* i1 = mesh.FaceList[i].edge->next->vertex;
        Vert* i2 = mesh.FaceList[i].edge->next->next->vertex;
        Vert* i3 = mesh.FaceList[i].edge->next->next->next->vertex;

        glm::vec3 p0 = glm::vec3(i0->position[0] , i0->position[1], i0->position[2]);
        glm::vec3 p1 = glm::vec3(i1->position[0] , i1->position[1], i1->position[2]);
        glm::vec3 p2 = glm::vec3(i2->position[0] , i2->position[1], i2->position[2]);
        glm::vec3 p3 = glm::vec3(i3->position[0] , i3->position[1], i3->position[2]);

        // NDC
        float x = 2 * (xpos / appWidth) - 1.0f;
        float y = 1.0f - 2.0f * (ypos / appHeight);
        float z = -1.0f;
        glm::vec3 ray_nds = glm::vec3(x, y, z);
        glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

        // Camera (EYE) Coordinates
        glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
        
        // World coordinate
        glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
        ray_wor = glm::normalize(ray_wor);

        // FIRST TRIANGLE IN FACE

        glm::vec3 n1 = p1 - p0;
        glm::vec3 n2 = p2 - p0;
        glm::vec3 n = glm::cross(n2, n1);

        float dot = glm::dot(n, ray_wor);

        if (dot < 0.001)
            continue; // polgyon facing away from ray

        glm::vec3 diff = p0 - orbitCamera.position();
        distance = glm::length(diff);

        float t = glm::dot(diff, n) /dot;

        glm::vec3 point = orbitCamera.position() + (ray_wor * t);

        tt = pointOnTriangle(point, n, p0, p1, p2);

        // SECOND TRIANGLE IN FACE

        if (tt != 0)
        {
            n1 = p3 - p2;
            n2 = p0 - p2;
            float dot = glm::dot(n, ray_wor);
            glm::vec3 diff = p0 - orbitCamera.position();

            float t = glm::dot(diff, n) /dot;

            glm::vec3 point = orbitCamera.position() + (ray_wor * t);

            tt = pointOnTriangle(point, n, p0, p3, p2);
        }

        if (tt == 0) {
            if (distance < minDistance) {
                minDistance = distance;
                faceID = i;
            }
        }
    }
    //std::cout << "ID: " << faceID << std::endl;
    results.first = minDistance;
    results.second = faceID;
    return results;
}

inline float RayHit::pointOnTriangle(glm::vec3& point, glm::vec3& n, glm::vec3& p0, glm::vec3& p1, glm::vec3& p2)
{
    Mesh mesh = app.getScene(0).getMesh(0);

    const float kNoIntersection =  FLT_MAX;

    glm::vec3 p = point;

    float u0, u1, u2;
    float v0, v1, v2;

    if (fabs(n.x) > fabs(n.y)) 
    {
        if (fabs(n.x) > fabs(n.z))
        {
            u0 = p.y - p0.y;
            u1 = p1.y - p0.y;
            u2 = p2.y - p0.y;

            v0 = p.z - p0.z;
            v1 = p1.z - p0.z;
            v2 = p2.z - p0.z;
        }
        else
        {
            u0 = p.x - p0.x;
            u1 = p1.x - p0.x;
            u2 = p2.x - p0.x;

            v0 = p.y - p0.y;
            v1 = p1.y - p0.y;
            v2 = p2.y - p0.y;
        }
    }
    else
    {
        if (fabs(n.y) > fabs(n.z))
        {
            u0 = p.x - p0.x;
            u1 = p1.x - p0.x;
            u2 = p2.x - p0.x;

            v0 = p.z - p0.z;
            v1 = p1.z - p0.z;
            v2 = p2.z - p0.z;
        }
        else
        {
            u0 = p.x - p0.x;
            u1 = p1.x - p0.x;
            u2 = p2.x - p0.x;

            v0 = p.y - p0.y;
            v1 = p1.y - p0.y;
            v2 = p2.y - p0.y;
        }
    }

    float temp = u1 * v2 - v1 * u2;
    
    if (!(temp != 0.0f)) {
        return kNoIntersection;
    }
    
    temp = 1.0f / temp;
    float alpha = (u0 * v2 - v0 * u2) * temp;
    if (! (alpha >= 0.0f)) {
        return kNoIntersection;
    }
    
    float beta = (u1 * v0 - v1 * u0) * temp;

    if (! (beta >= 0.0f)) {
        return kNoIntersection;
    }

    float gamma = 1.0f - alpha - beta;

    if (!(gamma >= 0.0f)) {
        return kNoIntersection;
    }  
    
    return t;
}

#endif