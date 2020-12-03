#ifndef RAYHIT_H
#define RAYHIT_H

#include <pthread.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "App.h"
#include <vector>

struct ThreadResult 
{
    unsigned int polyID = -1;
    float distance = FLT_MAX;
};

// Thread mutex lock
pthread_mutex_t lock;
ThreadResult Tresults;

#define NUMBER_OF_THREADS 32

struct threadData 
{
    Mesh mesh;
    int threadID;
    int appWidth;
    int appHeight;
    double xpos;
    double ypos;
    glm::mat4 projection;
    glm::mat4 view;
    OrbitCamera orbitCamera;
};

class RayHit
{
public:
    static float rayOnTriangle(glm::vec3& point, glm::vec3& n, glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, Mesh& mesh, float t);
    RayHit(double& xpos, double& ypos, int& appWidth, int& appHeight, 
           glm::mat4& view, glm::mat4& projection, glm::mat4& model, OrbitCamera& orbitCamera, App& app) :
           xpos(xpos), ypos(ypos), appWidth(appWidth), appHeight(appHeight), view(view), projection(projection),
           model(model), orbitCamera(orbitCamera), app(app) 
           {
               t = 0.0f;
           }
    pair<float, unsigned int> rayPlaneHitPoint();
    static void* calcRay(void* param);
    

private:
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

    pthread_t thread[NUMBER_OF_THREADS];
    threadData tData[NUMBER_OF_THREADS];

    Tresults.distance = FLT_MAX;
    Tresults.polyID = -1;

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        tData[i].mesh = app.getScene(0).getMesh(0);
        tData[i].threadID = i;
        tData[i].appWidth = appWidth;
        tData[i].appHeight = appHeight;
        tData[i].xpos = xpos;
        tData[i].ypos = ypos;
        tData[i].projection = projection;
        tData[i].view = view;
        tData[i].orbitCamera = orbitCamera;
    }

    for (long i = 0; i < NUMBER_OF_THREADS; i++) 
        pthread_create(&thread[i], NULL, &RayHit::calcRay, &tData[i]);

    for (long i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(thread[i], NULL);
    
    //std::cout << "ID: " << faceID << std::endl;
    results.first = Tresults.distance;
    results.second = Tresults.polyID;
    return results;
}

void* RayHit::calcRay(void* param) 
{
    struct threadData *data = (struct threadData*) param;
    int tID = data->threadID;
    unsigned int faceCount = data->mesh.FaceList.size();
    int appWidth = data->appWidth;
    int appHeight = data->appHeight;
    int lastElements = 0;
    double xpos = data->xpos;
    double ypos = data->ypos;
    glm::mat4 projection = data->projection;
    glm::mat4 view = data->view;
    OrbitCamera orbitCamera = data->orbitCamera;
    float tt = FLT_MAX;
    float distance = FLT_MAX;
    long faceID = -1;
    float minDistance = FLT_MAX;

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


    if (tID == NUMBER_OF_THREADS - 1) {
        lastElements = faceCount % NUMBER_OF_THREADS;
    }

    for (long i = tID * (faceCount / NUMBER_OF_THREADS); i < (faceCount / NUMBER_OF_THREADS)*(tID +1) + lastElements; i++)
    {
        if (data->mesh.FaceList[i].edge == nullptr)
            continue;
       
        Vert* i0 = data->mesh.FaceList[i].edge->vertex;
        Vert* i1 = data->mesh.FaceList[i].edge->next->vertex;
        Vert* i2 = data->mesh.FaceList[i].edge->next->next->vertex;
        
        glm::vec3 p0 = glm::vec3(i0->position[0] , i0->position[1], i0->position[2]);
        glm::vec3 p1 = glm::vec3(i1->position[0] , i1->position[1], i1->position[2]);
        glm::vec3 p2 = glm::vec3(i2->position[0] , i2->position[1], i2->position[2]);

        // FIRST TRIANGLE IN FACE

        glm::vec3 n1 = p1 - p0;
        glm::vec3 n2 = p2 - p0;
        glm::vec3 n = glm::cross(n2, n1);

        float dot = glm::dot(n, ray_wor);

        if (dot < 0.001)
            continue; // polgyon facing away from ray

        Vert* i3 = data->mesh.FaceList[i].edge->next->next->next->vertex;
        glm::vec3 p3 = glm::vec3(i3->position[0] , i3->position[1], i3->position[2]);

        glm::vec3 diff = p0 - orbitCamera.position();
        distance = glm::length(diff);

        float t = glm::dot(diff, n) /dot;
    
        if (t < 0.0f)
            continue;

        if (Tresults.distance < t)
            continue;

        glm::vec3 point = orbitCamera.position() + (ray_wor * t);
        tt = rayOnTriangle(point, n, p0, p1, p2, data->mesh, t);
        
          // SECOND TRIANGLE IN FACE

        if (tt == FLT_MAX)
        {
            tt = rayOnTriangle(point, n, p0, p3, p2, data->mesh, t);
        }
        
        if (tt != FLT_MAX) {
            
            if (t < minDistance) {
                minDistance = t;
                faceID = i;
            }
        }
        else
            continue;
        
        
        pthread_mutex_lock(&lock);
        
        if(minDistance < Tresults.distance)
        {
            Tresults.polyID = faceID;
            Tresults.distance = minDistance;
        }
            
        pthread_mutex_unlock(&lock);

    }
}

inline float RayHit::rayOnTriangle(glm::vec3& point, glm::vec3& n, glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, Mesh& mesh, float t) 
{
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