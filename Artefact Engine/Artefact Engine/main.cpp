#include <GL\glew.h> //openGL extension wrangling, include before glfw and openGL because it's a bit "special".
#include <GL\glfw3.h> //window and periphery handling
#include <gl\GL.h> //opengl

#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

#include "Maze/MazeTile.h"
#include "Maze\MazeGenerator.h" //TODO: move the constants into MazeTile, this shouldn't be exposed like this
#include "SceneGraph\SceneNode.h"
#include "SceneGraph\MeshNode.h"
#include "SceneGraph\CameraNode.h"
#include "Asset\AssetLoader.h"

#include "InputHandler.h"

//#include "bullet\btBulletDynamicsCommon.h"

//quick and dirty settings
#define WINDOW_TITLE "In Limbo"
#define resX 1280
#define resY 720

int main(){
	//init
	GLFWwindow* window;
	if (!glfwInit()){
		return -1;
	}

	//settings for the openGL context, additional flags: http://www.glfw.org/docs/3.0/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //openGL context 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	window = glfwCreateWindow(resX, resY, WINDOW_TITLE, nullptr, nullptr);
	glewExperimental = true; //required to get the core profile to work.

	if (!window){
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewInit();
	glGetError();		//glew is buggy and throws an openGL error no matter what, this handlies that error by ignoring it.

	MazeTile startTile;

	InputHandler input(window);

	
	//init physics
	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();	//collision entities are organized via an AABB tree
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	btDiscreteDynamicsWorld* dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0, -10, 0));
	*/
	
	//init camera and projection matrix
	glm::mat4 projection = glm::perspective((float)90, (float)resX / (float)resY, 0.1f, 100.0f); //FoV, aspect ratio, near clipping plane distance 0.1, far clipping plane distance 100
	glm::mat4 vp;

	//init scenegraph
	SceneNode sceneGraph(SceneNode::ROOT);
	CameraNode* camera = new CameraNode();
	sceneGraph.addNode(camera);
	//sceneGraph.addNode(new MeshNode("Asset//Models//duck.dae"), glm::vec3(0, 0, 0)));
	sceneGraph.addNode(AssetLoader::getInstance()->getMesh());

	/*for (int i = 0; i < startTile.TILE_SIZE; i++)
	{
		for (int j = 0; j < startTile.TILE_SIZE; j++)
		{
			if(startTile.tile[j + i * startTile.TILE_SIZE] == MazeGenerator::MAZE_WALL)
				sceneGraph.addNode(new MeshNode(AssetLoader::getInstance()->getMesh(), glm::vec3(i * 2, 0, j * 2)));
		}
	} */

	//gameloop
	double oldTime = glfwGetTime();
 
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5, 0.5, 0.5, 0.5);

	while (!glfwWindowShouldClose(window)){
		

		switch (glGetError()) {
		case GL_INVALID_ENUM: std::cerr << "oGl error: GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE: std::cerr << "oGl error: GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION: std::cerr << "oGl error: GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "oGl error: GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY: std::cerr << "oGl error: GL_OUT_OF_MEMORY" << std::endl; break;
		case GL_STACK_UNDERFLOW: std::cerr << "oGl error: GL_STACK_UNDERFLOW" << std::endl; break;
		case GL_STACK_OVERFLOW: std::cerr << "oGl error: GL_STACK_OVERFLOW" << std::endl; break;
		case GL_NO_ERROR:
		DEFAULT :
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			double newTime = glfwGetTime();
			double dT = newTime - oldTime;
			oldTime = newTime;

			input.handleInput();

			sceneGraph.update(dT, &input);
		    //	dynamicWorld->stepSimulation(dT, 4, 1./60.);
			sceneGraph.draw(projection * camera->getViewMatrix());

			glfwSwapBuffers(window); //actually renders the frame
			glfwPollEvents();
		}
	}
	//unloading
	glfwTerminate();
	
	return 0;
}