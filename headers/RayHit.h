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
    glm::vec3 rayPlaneHitPoint();

private:
    float pointOnTriangle(glm::vec3& point, glm::vec3& n); 
    

    double& xpos, ypos;
    int& appWidth, appHeight;
    glm::mat4& view, projection, model;
    OrbitCamera& orbitCamera;
    App& app;
    float t;
};

inline glm::vec3 RayHit::rayPlaneHitPoint()
{
    Mesh mesh = app.getScene(0).getMesh(0);

    glm::vec3 p0 = glm::vec3(mesh.vertices[0].pos[0], mesh.vertices[0].pos[1], mesh.vertices[0].pos[2]);
    glm::vec3 p1 = glm::vec3(mesh.vertices[1].pos[0], mesh.vertices[1].pos[1], mesh.vertices[1].pos[2]);
    glm::vec3 p2 = glm::vec3(mesh.vertices[2].pos[0], mesh.vertices[2].pos[1], mesh.vertices[2].pos[2]);

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

    glm::vec3 n1 = p2 - p0;
    glm::vec3 n2 = p0 - p1;
	glm::vec3 n = glm::cross(n2, n1);

    float dot = glm::dot(n, ray_wor);

    if (dot < 0.001)
        return glm::vec3(-1,-1,-1); // Face not facing to ray 

    glm::vec3 diff = p0 - orbitCamera.position();

    float t = glm::dot(diff, n) /dot;

    glm::vec3 point = orbitCamera.position() + (ray_wor * t);

    pointOnTriangle(point, n);

    return point;
}

inline float RayHit::pointOnTriangle(glm::vec3& point, glm::vec3& n)
{
    Mesh mesh = app.getScene(0).getMesh(0);

    glm::vec3 p0 = glm::vec3(mesh.vertices[0].pos[0], mesh.vertices[0].pos[1], mesh.vertices[0].pos[2]);
    glm::vec3 p1 = glm::vec3(mesh.vertices[1].pos[0], mesh.vertices[1].pos[1], mesh.vertices[1].pos[2]);
    glm::vec3 p2 = glm::vec3(mesh.vertices[2].pos[0], mesh.vertices[2].pos[1], mesh.vertices[2].pos[2]);


    const float kNoIntersection =  FLT_MAX;

    glm::vec3 p = point;

    std::cout << "p x: " << p.x << " y: " << p.y << " z: " << p.z << std::endl;

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
    std::cout << "alpha: " << alpha << std::endl;
    if (! (alpha >= 0.0f)) {
        std::cout << "alpha" << std::endl;
        return kNoIntersection;
    }
    
    float beta = (u1 * v0 - v1 * u0) * temp;

    if (! (beta >= 0.0f)) {
        std::cout << "beta" << std::endl;
        return kNoIntersection;
    }

    float gamma = 1.0f - alpha - beta;

    if (!(gamma >= 0.0f)) {
        std::cout << "gamma" << std::endl;
        return kNoIntersection;
    }  

    std::cout << "t: " << t << std::endl;
    
    return 0;
}

#endif