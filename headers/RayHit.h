#ifndef RAYHIT_H
#define RAYHIT_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Scene.h"

class RayHit
{
public:
    int rayHitPolygonID(double& xpos, double& ypos, int& appWidth, int& appHeight,
                        glm::mat4& view, glm::mat4& projection, glm::mat4& model,
                        OrbitCamera& orbitCamera, Scene& scene); // -1 = background, other wise returns polygon ID.

private:
    float rayTriangleIntersection(glm::vec3 hitPos, Vertex _p0, Vertex _p1, Vertex _p2, float minT); 

};

inline int RayHit::rayHitPolygonID(double& xpos, double& ypos, int& appWidth, int& appHeight, 
                                   glm::mat4& view, glm::mat4& projection, glm::mat4& model,
                                   OrbitCamera& OrbitCamera, Scene& scene)
{
    // NDC
	float x = 2 * (xpos / appWidth) - 1.0f;
	float y = 1.0f - 2.0f * (ypos / appHeight);
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

	// Camera (EYE) Coordinates
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	
	// World coordinate
	glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);

    //glm::vec3 ray_wor = (glm::inverse(view) * ray_wo);
	
	// Let's calculate plane (0,0,1) - eyeRay intersection
	glm::vec3 p0 = OrbitCamera.position();
	glm::vec3 n = glm::vec3(0.0f, 0.0f, 4.0f);

	float p0_n = glm::dot(p0, n);
	float d_n = glm::dot(ray_wor, n);

	glm::vec3 final = ray_wor - p0_n;
	final = final / d_n;

	glm::vec3 mouseWorldPos = p0 + final*ray_wor;
    std::cout << "P: " << mouseWorldPos.x << " " << mouseWorldPos.y << std::endl;

    float t = rayTriangleIntersection(mouseWorldPos, Vertex(scene.vertices[0].pos[0], scene.vertices[0].pos[1], scene.vertices[0].pos[2]),
                            Vertex(scene.vertices[1].pos[0], scene.vertices[1].pos[1], scene.vertices[1].pos[2]),
                            Vertex(scene.vertices[2].pos[0], scene.vertices[2].pos[1], scene.vertices[2].pos[2]),
                            1.0f);
    
    if (t == 5)
        std::cout << "Polygon found!" << std::endl;

    return 0;
}

inline float RayHit::rayTriangleIntersection(glm::vec3 hitPos, 
                              Vertex _p0, Vertex _p1, Vertex _p2,
                              float minT) 
{
    glm::vec3 p0 = glm::vec3(_p0.pos[0], _p0.pos[1], _p0.pos[2]);
    glm::vec3 p1 = glm::vec3(_p1.pos[0], _p1.pos[1], _p1.pos[2]);
    glm::vec3 p2 = glm::vec3(_p2.pos[0], _p2.pos[1], _p2.pos[2]);

    const float kNoIntersection = FLT_MAX;

    glm::vec3 v01 = p1 - p0;
    glm::vec3 v02 = p2 - p0;

    glm::vec3 n = glm::cross(v01, v02);

    if (glm::dot(n, hitPos) > 0)
        return FLT_MAX;

    glm::vec3 p = hitPos;

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
    
    if (!(temp != 0.0f))
        return kNoIntersection;
    
    temp = 1.0f / temp;
    float alpha = (u0 * v2 - v0 * u2) * temp;
    
    if (! (alpha >= 0.0f)) 
        return kNoIntersection;
    
    float beta = (u1 * v0 - v1 * u0) * temp;

    if (! (beta >= 0.0f))
        return kNoIntersection;

    float gamma = 1.0f - alpha - beta;

    if (!(gamma >= 0.0f))
        return kNoIntersection;
    
    return 5; // BUG 5
}

#endif