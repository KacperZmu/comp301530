#include "scenebasic_uniform.h"

#include <cstdio>

#include <cstdlib>
#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100),sky(100.0f){

	mesh = ObjMesh::load("media/Ring3016.obj", true);
	mesh2 = ObjMesh::load("media/Faces.obj", true);
	plane;
	

		
}

void SceneBasic_Uniform::initScene()
{
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

	glDebugMessageControl(
		GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);
    compile();
	glEnable(GL_DEPTH_TEST);
	model = mat4(1.0f);
	view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
	angle = 0.0f;

	glActiveTexture(GL_TEXTURE0);
	GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	glActiveTexture(GL_TEXTURE0);
	ring = Texture::loadTexture("media/texture/GoldB.png");
	glActiveTexture(GL_TEXTURE0);
	ground = Texture::loadTexture("media/texture/moss.png");
	glActiveTexture(GL_TEXTURE0);
	Face = Texture::loadTexture("media/texture/Face.png");

	
	prog.setUniform("Light.L", vec3(0.9f)); 
	prog.setUniform("Light.La", vec3(0.5f));
	prog.setUniform("Fog.MaxDist", 40.0f );
	prog.setUniform("Fog.MinDist", 7.0f);
	prog.setUniform("Fog.Color", vec3(0.6f,0.6f,0.6f));
	

	
	
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float t, glm::vec3 Orientation, glm::vec3 Position, glm::vec3 Up){

	float deltaT = t - tPrev;

	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	angle += 0.1f * deltaT;

	if (angle > glm::two_pi<float>())angle -= glm::two_pi<float>();

	prog.setUniform("ViewMatrix", view);
	view = glm::lookAt(Position, Position + Orientation, Up);
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec4 lightPos = vec4(10.0f*cos(angle), 10.0f, 10.0f*sin(angle), 1.0f);
	prog.setUniform("Light.Position", vec4(view*lightPos));

	prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
	prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
	prog.setUniform("Material.Ka", vec3(0.2f*0.3f, 0.55f*0.3f,0.9f*0.3f));
	prog.setUniform("Material.Shininess", 100.0f);

	prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
	prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
	prog.setUniform("Material.Ka", vec3(0.2f, 0.2f , 0.2f));
	prog.setUniform("Material.Shininess", 180.0f);
	prog.use();


	setMatrices();
	sky.render(); 
	model = mat4(1.0f);
	model = glm::rotate(model, angle, vec3(0.0f, 0.3f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, ring);
	setMatrices();
	mesh->render();
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 2.0f, 0.0f));
	glBindTexture(GL_TEXTURE_2D, Face);
	setMatrices();
	mesh2->render();
	model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
	glBindTexture(GL_TEXTURE_2D, ground);
	setMatrices();

	plane.render();
	
	
	
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    width = w;
    height = h;
    
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}


void SceneBasic_Uniform::setMatrices() 
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix",mv);
	prog.setUniform("NormalMatrix",glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);

}


