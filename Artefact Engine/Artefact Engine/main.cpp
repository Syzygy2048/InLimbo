

#include <GL\glew.h> //openGL extension wrangling, include before glfw and openGL because it's a bit "special".
#include <GL\glfw3.h> //window and periphery handling
#include <gl\GL.h> //opengl

#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

#include "Maze\MazeTile.h"
#include "Maze\MazeGenerator.h" //TODO: move the constants into MazeTile, this shouldn't be exposed like this
#include "SceneGraph\SceneNode.h"
#include "SceneGraph\MeshNode.h"
#include "SceneGraph\CameraNode.h"
#include "Asset\AssetLoader.h"
#include "Enums\MazeEnums.h"
#include "Enums\SceneGraphEnums.h"
#include "Enums\MeshPaths.h"


#include "InputHandler.h"

#include <PhysX\PxPhysicsAPI.h> //https://developer.nvidia.com/sites/default/files/akamai/physx/Manual/Startup.html
//I changed a bit in PxPreprocessor.h line 323 because it wouldn't have started otherwise for unknown reasons (_DEBUG was defined even though this is on a release build and that triggered an error)
//-------Loading PhysX libraries (32bit only)----------//


/*#ifdef _DEBUG //If in 'Debug' load libraries for debug mode 
#pragma comment(lib, "PhysX3DEBUG_x86.lib")				//Always be needed  
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")		//Always be needed
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")		//PhysX extended library 
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") //For PVD only 
*/
//#else //Else load libraries for 'Release' mode //not sure if this is needed
//#pragma comment(lib, "PhysX3_x86.lib")	
//#pragma comment(lib, "PhysX3Common_x86.lib") 
//#pragma comment(lib, "PhysX3Extensions.lib")
//#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
//#endif */



static physx::PxPhysics* gPhysicsSDK = NULL;
static physx::PxFoundation* gFoundation = NULL;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

physx::PxScene* gScene = NULL;
physx::PxReal gTimeStep = 1.f / 60.f;
physx::PxRigidDynamic*	gBox = NULL;



//quick and dirty settings
#define WINDOW_TITLE "In Limbo"
#define resX 1280
#define resY 720

int main(){
	//init window
	GLFWwindow* window;
	if (!glfwInit()){
		return -1;
	}

	//init openGL context
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

	//initialize texture library
	FreeImage_Initialise(TRUE);

	//init physics
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale());
	if (gPhysicsSDK == NULL)
	{
		std::cerr << "Error initializing PhysX3" << std::endl;
		return -1;
	}
	physx::PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (cooking == NULL)
	{
		std::cerr << "Error initializing PhysX3 cooking" << std::endl;
		return -1;
	}

	physx::PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	gScene = gPhysicsSDK->createScene(sceneDesc);

	/* //init geometry
		//1-Creating static plane	 
	PxTransform planePos =	PxTransform(PxVec3(0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));	//Position and orientation(transform) for plane actor  
	PxRigidStatic* plane =  gPhysicsSDK->createRigidStatic(planePos);								//Creating rigid static actor	
	plane->createShape(PxPlaneGeometry(), *material);												//Defining geometry for plane actor
	gScene->addActor(*plane);																		//Adding plane actor to PhysX scene


	//2-Creating dynamic cube																		 
	PxTransform		boxPos(PxVec3(0.0f, 10.0f, 0.0f));												//Position and orientation(transform) for box actor 
	PxBoxGeometry	boxGeometry(PxVec3(2,2,2));											//Defining geometry for box actor
	gBox = PxCreateDynamic(*gPhysicsSDK, boxPos, boxGeometry, *material, 1.0f);		//Creating rigid static actor
	gScene->addActor(*gBox);		
	*/
	glm::vec3 startinPos;
	MeshNode* duckMesh = new MeshNode(glm::vec3(0, 2, 0));
	duckMesh->initializeMeshNode(DUCK);
	duckMesh->bind();

	MeshNode* cowMesh = new MeshNode(glm::vec3(2.5, 3.7, 2.8));
	cowMesh->initializeMeshNode(COW);
	cowMesh->bind();

	MazeTile startTile;
	startTile.mergeAsMesh();
	startTile.createCollisionShape(gScene, gPhysicsSDK, cooking);
	startTile.bind();


	//init character controller https://developer.nvidia.com/sites/default/files/akamai/physx/Docs/CharacterControllers.html#character
	physx::PxControllerManager* manager = PxCreateControllerManager(*gScene);
	physx::PxCapsuleControllerDesc characterControllerDescription;
	//fill description here
	characterControllerDescription.position = physx::PxExtendedVec3(0.f, 0.f, 0.f);
	characterControllerDescription.radius = 0.5f;
	characterControllerDescription.height = 1.75f;

	physx::PxController* characterController = manager->createController(characterControllerDescription);
	//everything the controller wants to collide with needs PxShapeFlag::eSCENE_QUERY_SHAPE set to true otherwise it will be ignored






	//init camera and projection matrix
	glm::mat4 projection = glm::perspective((float)80, (float)resX / (float)resY, 0.1f, 100.0f); //FoV, aspect ratio, near clipping plane distance 0.1, far clipping plane distance 100
	glm::mat4 vp;

	CameraNode* activeCamera;				//this decides which VP matrix is used, if you want to switch cameras just set the new camera as the active camera
	CameraNode* freeCamera = new CameraNode();
	CameraNode* playerCamera = new CameraNode();

	
	
	//init scenegraph
	SceneNode sceneGraph(ROOT);
	sceneGraph.addNode(cowMesh);
	sceneGraph.addNode(duckMesh);
	sceneGraph.addNode(&startTile);
	sceneGraph.addNode(playerCamera);
	
	activeCamera = freeCamera;
	
	
	
	double oldTime = glfwGetTime();
	InputHandler input(window);

	
	
	
	
 
	
	//prepare for rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5, 0.5, 0.5, 0.5);
	//gameloop
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
			if (input.w)
			{
				activeCamera->moveForward(0.5f);
			}
			if (input.s){
				activeCamera->moveForward(-0.5f);
			}
			if (input.a){
				activeCamera->moveSideward(0.5f);
			}
			if (input.d){
				activeCamera->moveSideward(-0.5f);
			}
			activeCamera->rotate(input.mPosX - input.oldMousePos.x, input.mPosY - input.oldMousePos.y);
				

			sceneGraph.update(dT, &input);
			gScene->simulate(gTimeStep);
			gScene->fetchResults(true);
			//physx::PxU32 collisionFlags = characterController->move()
			sceneGraph.draw(projection * activeCamera->getViewMatrix());

			glfwSwapBuffers(window); //actually renders the frame
			glfwPollEvents();
		}
	}

	//unloading
	gPhysicsSDK->release();
	gFoundation->release();
	glfwTerminate();
	
	return 0;
}