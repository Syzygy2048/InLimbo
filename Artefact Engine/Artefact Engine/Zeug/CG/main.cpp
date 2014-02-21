#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/openglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

#include <FreeImage\FreeImage.h>



#include "Scene\Camera.hpp"
#include "Scene\Obstacles.hpp"

#include "Scene\Montser.hpp"

#define WIDTH 800
#define HEIGHT 600

using namespace projectname;
using namespace projectname::scene;

void init();
void update(double deltaT);
void draw();
void cleanup();
void draw2D();

void checkKeyInput(double deltaT);
void renderString(float x, float y, void *font, const unsigned char* string, glm::vec3 const& rgb);


//Shader
Shader* textureShader;
Shader* boneShader;

Monster* monster;
Camera* camera;
Obstacles* obstacles;

float totalHeight;
float remainingHeight;

//add Texture-Object
Texture* textureMontser;
Texture* textureSkybox;

float vertical;
float horizontal;

//Organizing our Scene
std::vector<SceneObject*> sceneGraph;


//Light
GLfloat lightPosition[3];
GLfloat whiteSpecularLight[3];
GLfloat whiteAmbientLight[3];
GLfloat whiteDiffuseLight[3];


int main(){
	//open window --> funkt noch ned (irgendwelche einstellungen passen ned :((
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);		
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//enables warnings for fixed-functions that should not be used

	if(!glfwInit()){
		std::cout << "Failed to init glfw" << std::endl;
		system("PAUSE");
		exit(-1);		//critical error
	}

	if(!glfwOpenWindow(WIDTH, HEIGHT,0,0,0,0,0,0, GLFW_WINDOW)){	//instead of GLFW_WINDOW is GLFW_FULLSCREEN possible
		glfwTerminate();
		std::cout << "failed to open window" << std::endl;
		system("PAUSE");
		exit(-1);		//critical error
	}

	//set clear color and viewport
	glClearColor(0.14f, 0.25f, 0.43f, 0.0f);		//normalized RGG-Values (from 0 to 1), 4th Value is transparency
	glViewport(0,0,WIDTH, HEIGHT);					//opens at the left-top corner

	//setup render loop
	bool running = true;

	//time
	double deltaT = 0.0;
	double lastTime = 0.0;

	//load opengl 3.3 function
	if(glewInit() != GLEW_OK){
		std::cout << "failed to init glew" << std::endl;
		system("PAUSE");
		exit(-1);
	}

	//Bugfixes
	__glewGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
	__glewBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
	__glewDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
	__glewGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glfwGetProcAddress("glGenerateMipmap");

	init();

	glfwSwapInterval(0);	//switch v-sync off

	while(running) {

		//clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//time
		double time = glfwGetTime();
		deltaT = time - lastTime;
		lastTime = time;

		checkKeyInput(deltaT);

		dynamicsWorld->stepSimulation((btScalar)deltaT);

		update(deltaT);
		draw();
		draw2D();

		//std::cout << "frametime: " << deltaT * 1000 << "ms" << std::endl;     

		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		//query gl errors
		GLenum stratusError = glGetError();
		if(stratusError != GL_NO_ERROR){
			std::cout << "gl error detected: "  << std::endl;		//we need a g-debugger to find the exact OpenGL-error
			std::cout << stratusError << std::endl;
		}
	}
	cleanup();
	glfwTerminate();

}

//setup engine functions
void init(){
	glfwSetWindowTitle("project_name");
	glfwSetMousePos(WIDTH/2, HEIGHT/2);

	//Mouse becomes invisible
	glfwDisable(GLFW_MOUSE_CURSOR);	
	glfwEnable(GLFW_STICKY_KEYS);

	//add depth test --> weiß ned ob das so passt laut tutorium schon :/
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable (GL_LIGHT0);

	whiteSpecularLight[0]=1.0f;whiteSpecularLight[1]=1.0f;whiteSpecularLight[2]=1.0f;
	whiteAmbientLight[0] = 1.0f;whiteAmbientLight[1] = 1.0f;whiteAmbientLight[2] = 1.0f;
	whiteDiffuseLight[0] = 1.0f;whiteDiffuseLight[1] = 1.0f;whiteDiffuseLight[2] = 1.0f;
	lightPosition[0]=1.0f;lightPosition[1]=1.0f;lightPosition[2]=1.0f;

	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//load shader
	textureShader = new Shader("Shader/texture.vert","Shader/texture.frag");

	camera = new Camera(glm::mat4(1.0f), textureShader);

	altimeter = new Altimeter(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.005f)),glm::vec3(60.0f,-25.0f,0.0f)), 90.0f, glm::vec3(1.0f,0.0f,0.0f)), textureShader);
	gameovertext = new Altimeter(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.002f, 0.0001f)),glm::vec3(0.0f,20.0f,0.0f)), 90.0f, glm::vec3(1.0f,0.0f,0.0f)), textureShader);
	wintext = new Altimeter(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.001f, 0.0001f)),glm::vec3(0.0f,20.0f,0.0f)), 90.0f, glm::vec3(1.0f,0.0f,0.0f)), textureShader);
	landingtext = new Altimeter(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.001f, 0.0001f)),glm::vec3(7.0f,330.0f,0.0f)), 90.0f, glm::vec3(1.0f,0.0f,0.0f)), textureShader);
	
	

	monster= new Monster(glm::rotate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.002f)), 180.0f, glm::vec3(0.0f,1.0f,0.0f)),65.0f, glm::vec3(1.0f,0.0f,0.0f)), boneShader);

	obstacles = new Obstacles();
	obstacles->initalize(sceneGraph, textureShader, dynamicsWorld);	

	

	textureShader->useShader();
	//UV-Testtexture
	textureMonster = new Texture("Models/...");
	textureSkybox = new Texture("Models/SkyBoxPartUV_work.png");
	
}

void update(double deltaT){
	int key = 0;

	if(win == false && gameover == false && pause == false) {
		if (glfwGetKey('A') == GLFW_PRESS) { //links
			key = 1;
		}
		if (glfwGetKey('D') == GLFW_PRESS) { //rechts
			key = 2;
		}
		if (glfwGetKey('W') == GLFW_PRESS) { //rauf
			key = 3;
		}
		if (glfwGetKey('S') == GLFW_PRESS) { //runter
			key = 4;
		}
		/*if (glfwGetKey('SPACE') == GLFW_PRESS) {     //shoot --> Space groß oder klein ??
			key = 5;
			
		} */
		
	}

	if(glfwGetKey('P') == GLFW_PRESS) {   //pause
		key = 7;
	}

	camera->update(deltaT, key, 0);
}

void draw(){	

	textureShader->useShader();

	//Bind texture to shader
	textureMonster->Bind(0, textureQuality, mipmap); //Bind(x) --> textureID refers to the bound value x
	textureSkybox->Bind(3,textureQuality, mipmap);
	

	//Light-Source Attributes
	glUniform3fv(glGetUniformLocation(textureShader->programHandle, "sDiffuse"),1,whiteDiffuseLight);
	glUniform3fv(glGetUniformLocation(textureShader->programHandle, "sSpecular"),1,whiteSpecularLight);
	glUniform3fv(glGetUniformLocation(textureShader->programHandle, "sAmbient"),1,whiteAmbientLight);

	//Lighting
	glUniform3fv(glGetUniformLocation(textureShader->programHandle, "lightpos"),1, lightPosition);

	//Camera
	glUniformMatrix4fv(glGetUniformLocation(textureShader->programHandle,"viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(textureShader->programHandle, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));

	for (unsigned int i=0; i<sceneGraph.size(); i++) {

		glUniform1i(glGetUniformLocation(textureShader->programHandle, "colorTexture"), sceneGraph[i]->textureID);		//textureID is the actual bind-value of every texture
		glUniformMatrix4fv(glGetUniformLocation(textureShader->programHandle, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(sceneGraph[i]->modelMatrix));

		glUniform4fv(glGetUniformLocation(textureShader->programHandle, "mSpecular"),1,sceneGraph[i]->specular);
		glUniform1f(glGetUniformLocation(textureShader->programHandle, "mShininess"),sceneGraph[i]->shininess);

		//Add red
		glUniform4f(glGetUniformLocation(textureShader->programHandle, "mDiffuse"),sceneGraph[i]->diffuse[0]*std::max(obstacleSpeed/20,1.0f),sceneGraph[i]->diffuse[1],sceneGraph[i]->diffuse[2],sceneGraph[i]->diffuse[3]);
		glUniform4f(glGetUniformLocation(textureShader->programHandle, "mAmbient"),sceneGraph[i]->ambient[0]*std::max(obstacleSpeed/20,1.0f),sceneGraph[i]->ambient[1],sceneGraph[i]->ambient[2],sceneGraph[i]->ambient[3]);

		sceneGraph[i]->draw();
	}

	//Render Montster-------------------------------------------------------------------------------------------------------------
	boneShader->useShader();

	//Light-Source Attributes
	glUniform3fv(glGetUniformLocation(boneShader->programHandle, "sDiffuse"),1,whiteDiffuseLight);
	glUniform3fv(glGetUniformLocation(boneShader->programHandle, "sSpecular"),1,whiteSpecularLight);
	glUniform3fv(glGetUniformLocation(boneShader->programHandle, "sAmbient"),1,whiteAmbientLight);

	//Lighting
	glUniform3fv(glGetUniformLocation(boneShader->programHandle, "lightpos"),1, lightPosition);

	//Camera
	glUniformMatrix4fv(glGetUniformLocation(boneShader->programHandle,"viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(boneShader->programHandle, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));
	glUniform1i(glGetUniformLocation(boneShader->programHandle, "colorTexture"), monsterut->textureID);

	glUniform4fv(glGetUniformLocation(boneShader->programHandle, "mSpecular"),1,monster->specular);
	glUniform1f(glGetUniformLocation(boneShader->programHandle, "mShininess"),monster->shininess);

	//Add red
	glUniform4f(glGetUniformLocation(boneShader->programHandle, "mDiffuse"),monster->diffuse[0]*std::max(obstacleSpeed/20,1.0f),monster->diffuse[1],monster->diffuse[2],monster->diffuse[3]);
	glUniform4f(glGetUniformLocation(boneShader->programHandle, "mAmbient"),monster->ambient[0]*std::max(obstacleSpeed/20,1.0f),monster->ambient[1],monster->ambient[2],monster->ambient[3]);

	monster->draw();
	//----------------------------------------------------------------------------------------------------------------------------
	textureShader->useShader();

	//Camera
	glUniformMatrix4fv(glGetUniformLocation(textureShader->programHandle,"viewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(textureShader->programHandle, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));



void checkKeyInput(double deltaT) {

	//Frame Time: On/Off
	if(glfwGetKey(GLFW_KEY_F2) == GLFW_PRESS && glfwGetKey(GLFW_KEY_F2) == GLFW_RELEASE) {
		frameTime =! frameTime;
	}

	frame++;
	timecounter += deltaT;

	if(timecounter > 1) {
		fps = frame;
		ft = (timecounter/frame)*1000.0f;
		frame = 0;
		timecounter = 0;
	}

	std::ostringstream os;
	os << ft;
	std::string str = "Frame Time: ";
	str += os.str();
	str += " ms, FPS: ";
	std::ostringstream os1;
	os1 << fps;
	str += os1.str();
	str += " , no of objects rendered: ?";

	const char* ch = str.c_str();

	if(frameTime) {
		renderString(10.0f, 20.0f, GLUT_BITMAP_HELVETICA_18, (unsigned char*) ch, glm::vec3(0.5f,0.5f,0.5f));	
	}


	//Pause On/Off 
	if (glfwGetKey('P') == GLFW_PRESS && glfwGetKey('P') == GLFW_RELEASE) {
		pause =! pause;
		elapsedTimePause = 0;
	}

	if(elapsedTimePause < 1) {
		if(pause) {
			renderString(10.0f, 140.0f, GLUT_BITMAP_HELVETICA_18, (unsigned char*) "Pause: On", glm::vec3(0.5f,0.5f,0.5f));
			elapsedTimePause += deltaT;
		}
		else {
			renderString(10.0f, 140.0f, GLUT_BITMAP_HELVETICA_18, (unsigned char*) "Pause: Off", glm::vec3(0.5f,0.5f,0.5f));
			elapsedTimePause += deltaT;
		}
	}
}

void renderString(float x, float y, void *font, const unsigned char *string,  glm::vec3 const& rgb) {

	glUseProgram(0);
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	glColor3f(rgb.r, rgb.g, rgb.b);
	glWindowPos2d(x, y);
	glutBitmapString(font, string);

	glEnable(GL_LIGHTING);
} 

void cleanup(){
	delete textureShader;
	delete camera;
	delete obstacles;

	//Textures
//	delete textureMonster;


	//delete models
	for (unsigned int i=0; i<sceneGraph.size(); i++) {
		if(sceneGraph[i]->rigidExists == true) {
			dynamicsWorld->removeRigidBody(sceneGraph[i]->getRigid());
			delete sceneGraph[i]->getRigid()->getMotionState();
			delete sceneGraph[i]->getRigid();
		}
		delete sceneGraph[i];
	}

}


//--------------------------------------------  PARTICLES --------------------------------------------------------------------

void renderParticles(){ 
	if(particles_leftTop.size() <= PARTICLES_COUNT){	//uses only one Collection

		for(unsigned int i = 0; i <= PARTICLES_COUNT - particles_leftTop.size(); i++){ 
			Particle p; 

			p.startPoint_X = PARTICLE_STARTPOINT_X;
			p.startPoint_Y = PARTICLE_STARTPOINT_Y;
			p.startPoint_Z = PARTICLE_STARTPOINT_Z;

			p.spawnDirection_X = (float)rand();
			p.spawnDirection_Y = (float)rand();
			p.spawnDirection_Z = (float)rand();

			p.lifeTime = rand()%PARTICLE_LIFETIME;  //produces lifetime between 0 and PARTICLE_LIFETIME

			float length = abs(sqrtf(((p.spawnDirection_X * p.spawnDirection_X) + (p.spawnDirection_Y * p.spawnDirection_Y))));

			p.spawnDirection_X = (p.spawnDirection_X / length);
			p.spawnDirection_Y = (p.spawnDirection_Y / length);	//Normalize
			p.spawnDirection_Z = (p.spawnDirection_Z / length);

			p.spawnDirection_X *= PARTICLE_SPEED;
			p.spawnDirection_Y *= PARTICLE_SPEED;		//Add speed
			p.spawnDirection_Z *= PARTICLE_SPEED;

			particles_leftTop.push_back(p);
			particles_rightTop.push_back(p);
			particles_leftBottom.push_back(p);
			particles_rightBottom.push_back(p);
		}
	}
}

void deleteOldParticles(){
	for(unsigned int i = 0; i < particles_leftTop.size(); i++){
		if(particles_leftTop[i].lifeTime > PARTICLE_LIFETIME){
			particles_leftTop.erase(particles_leftTop.begin() + i);
			particles_rightTop.erase(particles_rightTop.begin() + i);
			particles_leftBottom.erase(particles_leftBottom.begin() + i);
			particles_rightBottom.erase(particles_rightBottom.begin() + i);
		}
	}
}

void updateParticles(unsigned int lifeTime){
	for(unsigned int i = 0; i < particles_leftTop.size(); i++){
		particles_leftTop[i].startPoint_X = particles_leftTop[i].startPoint_X - particles_leftTop[i].spawnDirection_X;
		particles_leftTop[i].startPoint_Y = particles_leftTop[i].startPoint_Y + 2*particles_leftTop[i].spawnDirection_Y;
		particles_leftTop[i].startPoint_Z = particles_leftTop[i].startPoint_Z - particles_leftTop[i].spawnDirection_Z;
		particles_leftTop[i].lifeTime += lifeTime;
	}

	for(unsigned int i = 0; i < particles_rightTop.size(); i++){
		particles_rightTop[i].startPoint_X = particles_rightTop[i].startPoint_X + particles_rightTop[i].spawnDirection_X;
		particles_rightTop[i].startPoint_Y = particles_rightTop[i].startPoint_Y + 2*particles_rightTop[i].spawnDirection_Y;
		particles_rightTop[i].startPoint_Z = particles_rightTop[i].startPoint_Z - particles_rightTop[i].spawnDirection_Z;
		particles_rightTop[i].lifeTime += lifeTime;
	}

	for(unsigned int i = 0; i < particles_leftBottom.size(); i++){
		particles_leftBottom[i].startPoint_X = particles_leftBottom[i].startPoint_X + particles_leftBottom[i].spawnDirection_X;
		particles_leftBottom[i].startPoint_Y = particles_leftBottom[i].startPoint_Y + 2*particles_leftBottom[i].spawnDirection_Y;
		particles_leftBottom[i].startPoint_Z = particles_leftBottom[i].startPoint_Z + particles_leftBottom[i].spawnDirection_Z;
		particles_leftBottom[i].lifeTime += lifeTime;
	}

	for(unsigned int i = 0; i < particles_rightBottom.size(); i++){
		particles_rightBottom[i].startPoint_X = particles_rightBottom[i].startPoint_X - particles_rightBottom[i].spawnDirection_X;
		particles_rightBottom[i].startPoint_Y = particles_rightBottom[i].startPoint_Y + 2*particles_rightBottom[i].spawnDirection_Y;
		particles_rightBottom[i].startPoint_Z = particles_rightBottom[i].startPoint_Z + particles_rightBottom[i].spawnDirection_Z;
		particles_rightBottom[i].lifeTime += lifeTime;
	}

	deleteOldParticles();
	renderParticles();
}

void setParticleMatrix(glm::mat4 newMatrix){
	if(particleMatrix[0][0] == 1)				//makes sure that it is only set once
		particleMatrix = newMatrix;
}

