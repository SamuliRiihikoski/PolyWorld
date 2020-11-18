#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "Primitives.h"
#include <iostream>

float rayTriangleIntersection(Vertex _rayOrg, Vertex _rayDelta, 
                              Vertex _p0, Vertex _p1, Vertex _p2,
                              float minT) 
{
    glm::vec3 rayOrg = glm::vec3(_rayOrg.pos[0], _rayOrg.pos[1], _rayOrg.pos[2]);
    glm::vec3 rayDelta = glm::vec3(_rayDelta.pos[0], _rayDelta.pos[1], _rayDelta.pos[2]);
    glm::vec3 p0 = glm::vec3(_p0.pos[0], _p0.pos[1], _p0.pos[2]);
    glm::vec3 p1 = glm::vec3(_p1.pos[0], _p1.pos[1], _p1.pos[2]);
    glm::vec3 p2 = glm::vec3(_p2.pos[0], _p2.pos[1], _p2.pos[2]);

    const float kNoIntersection = FLT_MAX;

    glm::vec3 v01 = p1 - p0;
    glm::vec3 v02 = p2 - p0;

    glm::vec3 n = glm::cross(v01, v02);

    if (glm::dot(n, rayDelta) > 0)
        return FLT_MAX;

    glm::vec3 p = rayDelta;

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