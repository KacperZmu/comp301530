#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/teapot.h"
#include "helper/texture.h"
#include "helper/cube.h"
#include "helper/skybox.h"





class SceneBasic_Uniform : public Scene
{
private:
   
    Plane plane;
    SkyBox sky;
 
    std::unique_ptr<ObjMesh> mesh;
    std::unique_ptr<ObjMesh> mesh2;
    float tPrev;
    float angle;
    GLSLProgram prog;
    void setMatrices();
    void compile();
    GLuint ring;
    GLuint ground;
    GLuint Face;
    

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t, glm::vec3 Orientation, glm::vec3 Position, glm::vec3 Up);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
