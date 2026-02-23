///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
// ============
// manage the preparing and rendering of 3D scenes - textures, materials, lighting
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager* pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/


/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh(); // use for floor of scene and monitor screen
	m_basicMeshes->LoadBoxMesh(); //use for monitor 
	m_basicMeshes->LoadCylinderMesh(); //use for monitor stand
	m_basicMeshes->LoadConeMesh(); //use for bottom of computer stand
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadSphereMesh(); //use for desk lamp bulb
	m_basicMeshes->LoadTorusMesh(); //use for coffee mug handle

	//Load Textures
	CreateGLTexture("textures/monitor.jpg", "monitor");
	CreateGLTexture("textures/screen.jpg", "screen");
	CreateGLTexture("textures/dark-metal-texture.jpg", "metal");
	CreateGLTexture("textures/texture-wooden-boards.jpg", "desk");
	BindGLTextures();

	// define materials for objects in the scene
	DefineObjectMaterials();
	// set up the lighting for the scene
	SetupSceneLights();
}

/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for defining the materials that will
 *  be used for the objects in the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	// Glossy material for screen - high shine
	OBJECT_MATERIAL glossyMaterial;
	glossyMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glossyMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glossyMaterial.shininess = 128.0;
	glossyMaterial.tag = "glossy";
	m_objectMaterials.push_back(glossyMaterial);

	// Shiny metal material for stand - medium-high shine
	OBJECT_MATERIAL metalMaterial;
	metalMaterial.diffuseColor = glm::vec3(0.7f, 0.7f, 0.7f);
	metalMaterial.specularColor = glm::vec3(0.9f, 0.9f, 0.9f);
	metalMaterial.shininess = 64.0;
	metalMaterial.tag = "metal";
	m_objectMaterials.push_back(metalMaterial);

	// Wood material for desk - medium shine
	OBJECT_MATERIAL woodMaterial;
	woodMaterial.diffuseColor = glm::vec3(0.6f, 0.4f, 0.3f);
	woodMaterial.specularColor = glm::vec3(0.3f, 0.3f, 0.3f);
	woodMaterial.shininess = 32.0;
	woodMaterial.tag = "wood";
	m_objectMaterials.push_back(woodMaterial);

	// Matte plastic for keyboard and monitor - low shine
	OBJECT_MATERIAL mattePlasticMaterial;
	mattePlasticMaterial.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	mattePlasticMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f);
	mattePlasticMaterial.shininess = 16.0;
	mattePlasticMaterial.tag = "matte";
	m_objectMaterials.push_back(mattePlasticMaterial);

	// Ceramic material for mug - smooth with moderate shine
	OBJECT_MATERIAL ceramicMaterial;
	ceramicMaterial.diffuseColor = glm::vec3(0.9f, 0.9f, 0.9f);
	ceramicMaterial.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	ceramicMaterial.shininess = 48.0;
	ceramicMaterial.tag = "ceramic";
	m_objectMaterials.push_back(ceramicMaterial);

	// Default fallback material
	OBJECT_MATERIAL defaultMaterial;
	defaultMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	defaultMaterial.specularColor = glm::vec3(0.5f, 0.5f, 0.5f);
	defaultMaterial.shininess = 32.0;
	defaultMaterial.tag = "default";
	m_objectMaterials.push_back(defaultMaterial);
}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// Enable lighting in the shader
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	// PRIMARY LIGHT SOURCE: Directional light from above-front
	// Reduced ambient for more dramatic shadows and contrast
	m_pShaderManager->setVec3Value("directionalLight.direction", 0.2f, -1.0f, -0.3f);
	m_pShaderManager->setVec3Value("directionalLight.ambient", 0.25f, 0.25f, 0.25f);
	m_pShaderManager->setVec3Value("directionalLight.diffuse", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("directionalLight.specular", 0.4f, 0.4f, 0.4f);
	m_pShaderManager->setBoolValue("directionalLight.bActive", true);

	// SECONDARY LIGHT SOURCE: Point light from the right side (cool fill light)
	// Slightly blue-tinted to contrast with warm lamp
	m_pShaderManager->setVec3Value("pointLights[0].position", 6.0f, 6.0f, 3.0f);
	m_pShaderManager->setVec3Value("pointLights[0].ambient", 0.1f, 0.1f, 0.15f);
	m_pShaderManager->setVec3Value("pointLights[0].diffuse", 0.4f, 0.4f, 0.5f);  // Slightly cool/blue
	m_pShaderManager->setVec3Value("pointLights[0].specular", 0.5f, 0.5f, 0.6f);
	m_pShaderManager->setBoolValue("pointLights[0].bActive", true);

	// THIRD LIGHT SOURCE (COLORED): Point light from desk lamp - soft red glow
	m_pShaderManager->setVec3Value("pointLights[1].position", -5.2f, 2.6f, 0.8f);
	// Softer red light values 
	m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.25f, 0.05f, 0.05f);   // gentle red ambient tint
	m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.8f, 0.1f, 0.1f);     // main red light cone
	m_pShaderManager->setVec3Value("pointLights[1].specular", 0.6f, 0.2f, 0.2f);    // soft red highlights
	m_pShaderManager->setBoolValue("pointLights[1].bActive", true);

	// Disable remaining point lights
	m_pShaderManager->setBoolValue("pointLights[2].bActive", false);
	m_pShaderManager->setBoolValue("pointLights[3].bActive", false);
	m_pShaderManager->setBoolValue("pointLights[4].bActive", false);

	// Disable spotlight
	m_pShaderManager->setBoolValue("spotLight.bActive", false);
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// DESK - Draw as a box instead of a plane
	// set the XYZ scale for the mesh - WIDER desk
	scaleXYZ = glm::vec3(15.0f, 0.5f, 10.0f);  // Increased width from 10 to 15

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh - raised slightly so top is at y=0
	positionXYZ = glm::vec3(0.0f, -0.25f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(.91, .85, .85, 1);
	SetShaderTexture("desk");           // Apply desk texture
	SetShaderMaterial("wood");          // Wood material with medium shine
	SetTextureUVScale(1.5f, 1.0f);      // Adjust UV scale for wider desk
	// draw the mesh with transformation values

	m_basicMeshes->DrawBoxMesh();  // Changed from DrawPlaneMesh() to DrawBoxMesh()
	/****************************************************************/
	//prepare the montior box
	scaleXYZ = glm::vec3(10.0f, 0.15f, 4.5f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	//set position for montitor box - centered on desk
	positionXYZ = glm::vec3(0.0f, 5.0f, 0.0f);
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	//set color for monitor
	SetShaderColor(0.2f, 0.2f, 0.2f, 1.f);
	//DRAW monitor base
	// When drawing an object
	SetShaderTexture("monitor");  // Use the tag you defined
	SetShaderMaterial("matte");   // Matte plastic for monitor casing
	SetTextureUVScale(1.0f, 1.0f);  // Controls tiling
	m_basicMeshes->DrawBoxMesh();


	//draw the cylinder stand
	scaleXYZ = glm::vec3(.5f, .5f, 2.5f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 90.0f; //level with plane
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);  // Centered on desk
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.1f, 0.1f, 0.1f, 1.0f);
	SetShaderMaterial("metal");  // Shiny metal material
	//DRAW cylinder plate under monitor box
	m_basicMeshes->DrawCylinderMesh();


	//draw tapered cyclinder for monitor stand 
	scaleXYZ = glm::vec3(0.2f, 5.0f, 1.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 90.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);  // Centered on desk
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	//set color of tapered cylinder stand
	SetShaderColor(0.1f, 0.1f, 0.1f, 1.0f);
	SetShaderTexture("metal");
	SetShaderMaterial("metal");  // Shiny metal material
	SetTextureUVScale(1.0f, 2.0f);
	//DRAW cone shape for the montior stand 
	m_basicMeshes->DrawTaperedCylinderMesh();


	// draw plane mesh for the monitor screen
	scaleXYZ = glm::vec3(4.0f, 1.0f, 2.0f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-0.25f, 5.0f, 0.5f);  // Centered with monitor body
	//set to memory
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(1.0f, 1.0f, 1.0f, 1.0f);
	//DRAW screen plane
	SetShaderTexture("screen");
	SetShaderMaterial("glossy");  // Glossy material for shiny screen
	SetTextureUVScale(1.0f, 1.0f);
	m_basicMeshes->DrawPlaneMesh();

	/****************************************************************/
	// KEYBOARD - positioned in front of monitor
	scaleXYZ = glm::vec3(5.0f, 0.15f, 1.0f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 0.075f, 3.0f);  // In front of monitor on desk
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.1f, 0.1f, 0.1f, 1.0f);  // Dark gray/black keyboard
	SetShaderMaterial("matte");  // Matte plastic material
	//DRAW keyboard base
	m_basicMeshes->DrawBoxMesh();

	/****************************************************************/
	// DESK LAMP - positioned to the left of monitor
	// Lamp base (cylinder)
	scaleXYZ = glm::vec3(0.5f, 0.3f, 0.3f);  // Slightly larger base
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-5.5f, 0.10f, 2.0f);  // Adjusted for wider desk
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.2f, 0.2f, 0.2f, 1.0f);  // Dark base
	SetShaderMaterial("metal");  // Metal base
	//DRAW lamp base
	m_basicMeshes->DrawCylinderMesh();

	// Lamp pole (cylinder - not tapered for cleaner look)
	scaleXYZ = glm::vec3(0.12f, 0.12f, 2.8f);
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-5.5f, 0.3f, 2.0f);  // Rising from base
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.15f, 0.15f, 0.15f, 1.0f);  // Dark pole
	SetShaderMaterial("metal");  // Metal pole
	//DRAW lamp pole
	m_basicMeshes->DrawCylinderMesh();

	// Lamp shade (cone)
	scaleXYZ = glm::vec3(0.7f, 0.9f, 0.7f);  // Larger shade
	XrotationDegrees = 180.0f;  // Flip cone upside down for lamp shade
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-5.5f, 3.1f, 2.0f);  // Top of pole
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.9f, 0.85f, 0.7f, 1.0f);  // Warm cream shade
	SetShaderMaterial("matte");  // Matte shade
	//DRAW lamp shade
	m_basicMeshes->DrawConeMesh();

	// Lamp bulb (sphere) - light source
	scaleXYZ = glm::vec3(0.3f, 0.3f, 0.3f);  // Slightly larger bulb
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(-5.5f, 2.7f, 2.0f);  // Inside shade
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(1.0f, 0.95f, 0.8f, 1.0f);  // Warm yellow-white bulb
	SetShaderMaterial("glossy");  // Glossy bulb
	//DRAW lamp bulb
	m_basicMeshes->DrawSphereMesh();

	/****************************************************************/
	// COFFEE MUG - positioned on right side of desk near keyboard
	// Mug body (cylinder)
	scaleXYZ = glm::vec3(0.45f, 0.45f, 0.65f);  // Slightly larger mug
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(5.0f, 0.325f, 2.5f);  // Adjusted for wider desk - right side
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.85f, 0.25f, 0.15f, 1.0f);  // Brighter red mug
	SetShaderMaterial("ceramic");  // Ceramic material with moderate shine
	//DRAW mug body
	m_basicMeshes->DrawCylinderMesh();

	// Mug handle (torus)
	scaleXYZ = glm::vec3(0.28f, 0.38f, 0.1f);  // Proportional handle
	XrotationDegrees = 0.0f;
	YrotationDegrees = 90.0f;  // Rotate to face outward
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(5.5f, 0.325f, 2.5f);  // Attached to side of mug
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ
	);
	SetShaderColor(0.85f, 0.25f, 0.15f, 1.0f);  // Same red as mug
	SetShaderMaterial("ceramic");  // Ceramic material
	//DRAW mug handle
	m_basicMeshes->DrawTorusMesh();
}