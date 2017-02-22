#include "SceneAssignment.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "Strategy_Kill.h"
#include "Strategy_Run.h"

bool SceneAssignment::bReset = false;
bool SceneAssignment::controlswap = false;
bool SceneAssignment::bExit = false;

static float TILE_SIZE = 32.f;
static float TILE_HERO_SIZE = 64.f;

SceneAssignment::SceneAssignment()
	: m_cMinimap(NULL)
	, m_cMap(NULL)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, m_cRearMap(NULL)
	, rearWallOffset_x(0)
	, rearWallOffset_y(0)
	, rearWallTileOffset_x(0)
	, rearWallTileOffset_y(0)
	, rearWallFineOffset_x(0)
	, rearWallFineOffset_y(0)
	, theEnemy(NULL)
	, theEnemy2(NULL)
	, theEnemy3(NULL)
	, sound(NULL)
	, isPaused(false)
	, isBegun(false)
	, isStart(false)
	, iChoice(1)
	, iChoice2(1)
	, bRead(false)
	, bReadPage1(false)
	, bReadPage2(false)
	, bReadPage3(false)
	, win(false)
	, lose(false)
	, treasurehelp(false)
	, getHit(false)
	, getHitTime(0.f)
	, spacebarhelp(false)
	, treasureget(false)
	, lives(3)
	, tutoriallevel(false)
	, enemyhelp(false)
	, enemy1hit(false)
	, enemy2hit(false)
	, enemy3hit(false) 
{
	//when reconstructing this program, this will reset the bool to false again
	//to avoid infinite reconstruct loop
	bReset = false;
	bExit = false;
	controlswap = false;
}
bool SceneAssignment::getControlswap(void)
{
	return controlswap;
}
SceneAssignment::~SceneAssignment()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
	if(theEnemy)
	{
		delete theEnemy;
		theEnemy = NULL;
	}
	if(theEnemy2)
	{
		delete theEnemy2;
		theEnemy2 = NULL;
	}
	if(theEnemy2)
	{
		delete theEnemy2;
		theEnemy2 = NULL;
	}
	if (sound)
	{
		delete sound;
		sound = NULL;
	}
}
void SceneAssignment::InitParameters()
{
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);
}
void SceneAssignment::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	//	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	InitParameters();

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UI", Color(1, 1, 1), 2);
	meshList[GEO_UI]->textureID = LoadTGA("Image//UI.tga");
	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color(1, 1, 1), 2);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//menu.tga");
	meshList[GEO_WIN] = MeshBuilder::GenerateQuad("Win", Color(1, 1, 1), 2);
	meshList[GEO_WIN]->textureID = LoadTGA("Image//youwin_final.tga");

	meshList[GEO_WIN_TUTORIAL] = MeshBuilder::GenerateQuad("GEO_WIN_TUTORIAL", Color(1, 1, 1), 2);
	meshList[GEO_WIN_TUTORIAL]->textureID = LoadTGA("Image//youwin_tutorial.tga");

	meshList[GEO_LOSE] = MeshBuilder::GenerateQuad("Lose", Color(1, 1, 1), 2);
	meshList[GEO_LOSE]->textureID = LoadTGA("Image//youlose.tga");

	meshList[GEO_GUI_LICO] = MeshBuilder::GenerateQuad("Lose", Color(1, 1, 1), 2);
	meshList[GEO_GUI_LICO]->textureID = LoadTGA("Image//youlose.tga");

	meshList[GEO_GUI_LICO] = MeshBuilder::Generate2DMesh("GEO_GUI_LICO", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_GUI_LICO]->textureID = LoadTGA("Image//lico_head.tga");

	//instruction set
	meshList[GEO_INST] = MeshBuilder::GenerateQuad("Instruction", Color(1, 1, 1), 2);
	meshList[GEO_INST]->textureID = LoadTGA("Image//instruction1.tga");
	meshList[GEO_INST2] = MeshBuilder::GenerateQuad("Instruction screen 2", Color(1, 1, 1), 2);
	meshList[GEO_INST2]->textureID = LoadTGA("Image//instruction2.tga");
	meshList[GEO_INST3] = MeshBuilder::GenerateQuad("Instruction screen 3", Color(1, 1, 1), 2);
	meshList[GEO_INST3]->textureID = LoadTGA("Image//instruction3.tga");

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, Application::getWindowWidth(), Application::getWindowHeight());
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//volcano.tga");

	meshList[GEO_DARK_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_DARK_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, Application::getWindowWidth(), Application::getWindowHeight());
	meshList[GEO_DARK_BACKGROUND]->textureID = LoadTGA("Image//darkclouds.tga");

	meshList[GEO_TILEGROUND] = MeshBuilder::Generate2DMesh("GEO_TILEGROUND", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEGROUND]->textureID = LoadTGA("Image//tile1_ground.tga");
	meshList[GEO_TILE_SOIL] = MeshBuilder::Generate2DMesh("GEO_TILE_SOIL", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_SOIL]->textureID = LoadTGA("Image//tile15_soil.tga");
	meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");

	meshList[GEO_TILE_FINISH] = MeshBuilder::Generate2DMesh("GEO_TILE_FINISH", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_FINISH]->textureID = LoadTGA("Image//tile_completelevel.tga");

	meshList[GEO_TILE_TUTORIAL_TRIGGER] = MeshBuilder::Generate2DMesh("GEO_TILE_TUTORIAL_TRIGGER", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_TUTORIAL_TRIGGER]->textureID = LoadTGA("Image//tile_trigger.tga");

	meshList[GEO_TILE_TREASURECHEST] = MeshBuilder::Generate2DMesh("GEO_TILE_TREASURECHEST", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_TREASURECHEST]->textureID = LoadTGA("Image//tile_treasurechest.tga");

	meshList[GEO_TILE_TREASUREHELP] = MeshBuilder::Generate2DMesh("GEO_TILE_TREASUREHELP", Color(1, 1, 1), 0.0f, 0.0f, 4.05f, 1.f);
	meshList[GEO_TILE_TREASUREHELP]->textureID = LoadTGA("Image//treasurehelp.tga");
	meshList[GEO_TILE_SPACEBARHELP] = MeshBuilder::Generate2DMesh("GEO_TILE_SPACEBARHELP", Color(1, 1, 1), 0.0f, 0.0f, 4.05f, 1.f);
	meshList[GEO_TILE_SPACEBARHELP]->textureID = LoadTGA("Image//spacebarhelp.tga");
	meshList[GEO_TILE_ENEMYHELP] = MeshBuilder::Generate2DMesh("GEO_TILE_ENEMYHELP", Color(1, 1, 1), 0.0f, 0.0f, 4.05f, 1.f);
	meshList[GEO_TILE_ENEMYHELP]->textureID = LoadTGA("Image//enemyhelp.tga");

	meshList[GEO_TILE_FINISHLINE_STEM] = MeshBuilder::Generate2DMesh("GEO_TILE_FINISHLINE_STEM", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_FINISHLINE_STEM]->textureID = LoadTGA("Image//tile4_finishlinestem.tga");
	meshList[GEO_TILE_FINISHLINE_SIGN] = MeshBuilder::Generate2DMesh("GEO_TILE_FINISHLINE_SIGN", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_FINISHLINE_SIGN]->textureID = LoadTGA("Image//tile5_finishlinesign.tga");


	meshList[GEO_TILELICO_IDLE_LEFT] = MeshBuilder::GenerateSpriteAnimation("Lico Idle left", 1, 10, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_IDLE_LEFT]->textureID = LoadTGA("Image//lico_stand_left.tga");
	meshList[GEO_TILELICO_MOVE_LEFT] = MeshBuilder::GenerateSpriteAnimation("Lico Move left", 1, 4, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_MOVE_LEFT]->textureID = LoadTGA("Image//lico_move_left.tga");
	meshList[GEO_TILELICO_IDLE_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Lico Idle right", 1, 10, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_IDLE_RIGHT]->textureID = LoadTGA("Image//lico_stand_right.tga");
	meshList[GEO_TILELICO_MOVE_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Lico Move inverted", 1, 4, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_MOVE_RIGHT]->textureID = LoadTGA("Image//lico_move_right.tga");
	meshList[GEO_TILELICO_JUMP_LEFT] = MeshBuilder::GenerateSpriteAnimation("Lico Jump", 1, 1, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_JUMP_LEFT]->textureID = LoadTGA("Image//lico_jump_left.tga");
	meshList[GEO_TILELICO_JUMP_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Lico Jump inverted", 1, 1, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_JUMP_RIGHT]->textureID = LoadTGA("Image//lico_jump_right.tga");
	meshList[GEO_TILELICO_ATK_LEFT] = MeshBuilder::GenerateSpriteAnimation("Lico ATK", 1, 7, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_ATK_LEFT]->textureID = LoadTGA("Image//lico_atk_left.tga");
	meshList[GEO_TILELICO_ATK_RIGHT] = MeshBuilder::GenerateSpriteAnimation("Lico Atk inverted", 1, 7, TILE_HERO_SIZE, TILE_HERO_SIZE);
	meshList[GEO_TILELICO_ATK_RIGHT]->textureID = LoadTGA("Image//lico_atk_right.tga");

	meshList[GEO_TILE_KILLZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_KILLZONE", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_KILLZONE]->textureID = LoadTGA("Image//tile10_killzone.tga");
	meshList[GEO_TILE_SAFEZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_SAFEZONE", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_SAFEZONE]->textureID = LoadTGA("Image//tile11_safezone.tga");

	meshList[GEO_TILE_ENEMY] = MeshBuilder::GenerateSpriteAnimation("GEO_TILE_ENEMY", 1, 11, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_ENEMY]->textureID = LoadTGA("Image//tile_enemy.tga");

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init( Application::getWindowHeight(), Application::getWindowWidth(), 24, 32, Application::getWindowHeight(), Application::getWindowWidth() * 2 );
	m_cMap->LoadMap( "Image//MapDesign.csv" );

	// Initialise and load the REAR tile map
	m_cRearMap = new CMap();
	m_cRearMap->Init( Application::getWindowHeight(), Application::getWindowWidth(), 24, 32, Application::getWindowHeight(), Application::getWindowWidth() * 2 );
	m_cRearMap->LoadMap( "Image//MapDesign_Rear.csv" );

	// Initialise the hero's position
	theHero = new CPlayerInfo();
	theHero->SetPos_x(TILE_SIZE*2);
	theHero->SetPos_y(TILE_SIZE*6);

	//Set the strategy for the enemy
	theEnemy = new CEnemy();
	theEnemy->ChangeStrategy(NULL, false);
	theEnemy->SetPos_x(TILE_SIZE*30);
	theEnemy->SetPos_y(TILE_SIZE*9);

	theEnemy2 = new CEnemy();
	theEnemy2->ChangeStrategy(NULL, false);
	theEnemy2->SetPos_x(TILE_SIZE*103);
	theEnemy2->SetPos_y(TILE_SIZE*10);

	theEnemy3 = new CEnemy();
	theEnemy3->ChangeStrategy(NULL, false);
	theEnemy3->SetPos_x(TILE_SIZE*109);
	theEnemy3->SetPos_y(TILE_SIZE*14);

	if (sound == NULL)
	{
		sound = new CSound();
		sound->Init();
	}


	//total number of lives permitted
	/*theArrayOfGoodies = new CGoodies*[5];
	for(int i = 0; i < lives; i++)
	{
	theArrayOfGoodies[i] = theGoodiesFactory.Create(HEALTH);
	theArrayOfGoodies[i]->SetPos(150 + i*30, 150);
	theArrayOfGoodies[i]->SetMesh(MeshBuilder::Generate2DMesh("GEO_HEALTH", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE));
	theArrayOfGoodies[i]->SetTextureID(LoadTGA("Image//tile_health.tga"));
	}*/

	//animation for sprite facing left
	SpriteAnimation *lico = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_IDLE_LEFT]);
	if(lico)
	{
		lico->m_anim = new Animation();
		lico->m_anim->Set(0, 9, 0, 1.f);
	}
	SpriteAnimation *licomove = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_MOVE_LEFT]);
	if(licomove)
	{
		licomove->m_anim = new Animation();
		licomove->m_anim->Set(0, 3, 0, 1.f);
	}

	//animation for sprite facing right
	SpriteAnimation *licoinv = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_IDLE_RIGHT]);
	if(licoinv)
	{
		licoinv->m_anim = new Animation();
		licoinv->m_anim->Set(0, 9, 0, 1.f);
	}
	SpriteAnimation *licomoveinv = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_MOVE_RIGHT]);
	if(licomoveinv)
	{
		licomoveinv->m_anim = new Animation();
		licomoveinv->m_anim->Set(0, 3, 0, 1.f);
	}

	//animation for sprite facing left and attacking
	SpriteAnimation *licoatk = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_ATK_LEFT]);
	if(lico)
	{
		licoatk->m_anim = new Animation();
		licoatk->m_anim->Set(1, 6, 0, 0.8f);
	}
	//animation for sprite facing right and attacking
	SpriteAnimation *licoatkright = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_ATK_RIGHT]);
	if(licoatkright)
	{
		licoatkright->m_anim = new Animation();
		licoatkright->m_anim->Set(1, 6, 0, 0.8f);
	}

	//animation for enemy
	SpriteAnimation *enemy = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILE_ENEMY]);
	if(enemy)
	{
		enemy->m_anim = new Animation();
		enemy->m_anim->Set(0, 10, 0, 1.f);
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	rotateAngle = 0;

	bLightEnabled = true;
}
void SceneAssignment::UpdateMainMenu(double dt)
{
	//opening screen triggers here
	if(Application::IsKeyPressed('1') && isBegun == false && bRead == false)
	{
		isStart = true;
	}
	//start the game, do not display instructions
	if(bRead == true)
	{
		//display first set of instructions
		if(Application::IsKeyPressed(VK_RETURN) && bReadPage1 == false && bReadPage2 == false && bReadPage3 == false)
		{
			bReadPage1 = true;
			bReadPage2 = false;
			bReadPage3 = false;
			Sleep(250);			//trigger a pause so player can't skip the panels
		}
		if(Application::IsKeyPressed(VK_RETURN) && bReadPage1 == true && bReadPage2 == false && bReadPage3 == false)
		{
			bReadPage1 = false;
			bReadPage2 = true;
			bReadPage3 = false;
			Sleep(250);	
		}
		if(Application::IsKeyPressed(VK_RETURN) && bReadPage1 == false && bReadPage2 == true && bReadPage3 == false)
		{
			bReadPage1 = false;
			bReadPage2 = false;
			bReadPage3 = true;
			Sleep(250);	
		}
		if(bReadPage1 == false && bReadPage2 == false && bReadPage3 == true)
		{
			//reset everything back to false
			bReadPage1 = false;
			bReadPage2 = false;
			bReadPage3 = false;
			bRead = false;
		}
	}

	//triggers the main menu
	if(isStart == true && isBegun == false)
	{
		if (Application::IsKeyPressed(VK_DOWN))
			if (iChoice2 < 3 )
			{
				iChoice2++;
				Sleep(150);	
			}
			if (Application::IsKeyPressed(VK_UP)) 
			{
				if (iChoice2 > 1)
				{
					iChoice2--;
					Sleep(150);
				}
			}
			if (Application::IsKeyPressed(VK_RETURN)) 
			{
				if(iChoice2 == 1)
				{
					//start game
					isBegun = true;
				}
				if (iChoice2 == 2)
				{
					//display instructions
					bRead = true;
				}
				if(iChoice2 == 3)
				{
					//exit game
					bExit = true;
					//exit(0);
				}
			}
	}
}
void SceneAssignment::UpdateWin(double dt)
{
	if(Application::IsKeyPressed('0') && win == true)
	{
		//reset
		bReset = true;
	}

	//when tutorial level is done
	if(Application::IsKeyPressed('0') && tutoriallevel == true)
	{
		delete m_cMap;
		m_cMap = new CMap();
		m_cMap->Init( Application::getWindowHeight(), Application::getWindowWidth(), 24, 32, Application::getWindowHeight(), Application::getWindowWidth() * 4 );
		m_cMap->LoadMap( "Image//MapDesign2.csv" );
		tutoriallevel = false;

		//reset the position
		theHero->SetPos_x(TILE_SIZE*2);
		theHero->SetPos_y(TILE_SIZE*6);

		theHero->setMapOffset_x(0);
		theHero->setMapOffset_y(0);

		theEnemy->SetPos_x(TILE_SIZE*61);
		theEnemy->SetPos_y(TILE_SIZE*2);

		enemy1hit = false;
		enemy2hit = false;
		enemy3hit = false;

		controlswap = true;
	}
}
void SceneAssignment::UpdateLose(double dt)
{
	if(Application::IsKeyPressed('0') && lose == true)
	{
		//reset for now
		bReset = true;
	}
}
void SceneAssignment::UpdatePauseMenu(double dt)
{
	if(isBegun == true && isStart == true)
	{
		if(Application::IsKeyPressed(VK_ESCAPE) || win == true || lose == true || tutoriallevel == true)
		{
			isPaused = true;
		}
		{
			//do not display choices available when any one of the 
			if(isPaused == true && !win && !lose && !tutoriallevel)
			{
				if (Application::IsKeyPressed(VK_DOWN))
					if (iChoice < 3 )
					{
						iChoice++;
						Sleep(150);	
					}
					if (Application::IsKeyPressed(VK_UP)) 
						if (iChoice > 1)
						{
							iChoice--;
							Sleep(150);
						}

						if (Application::IsKeyPressed(VK_RETURN)) 
						{
							if (iChoice == 1) 
							{
								isPaused = false;
							}
							if (iChoice == 2)
							{
								isBegun = false;
								isStart = false;
								bReset = true;
							}
							if(iChoice == 3)
							{
								bExit = true;
								//exit(0);
							}
						}
			}
			else if(isPaused == false)
			{
				UpdateGame(dt);
			}
		}
	}
}
void SceneAssignment::UpdateSounds(double dt)
{
	//play the background sound while the menu is active
	if((isStart && !isBegun) ||(!isStart && !isBegun))
	{
		sound->PlayMainMenu();
	}
	//when the game begins, stop the main menu music
	else if(isStart && isBegun)
	{
		sound->PauseMainMenu();
	}

	////when the game begins in the tutorial level and it is yet to be completed
	//if(isBegun && !tutoriallevel && !controlswap)
	//{
	//	if(TutorialLevel->getIsPaused())
	//	{
	//		TutorialLevel->setIsPaused(false);
	//	}
	//}
	//else if(isBegun && tutoriallevel && !controlswap)
	//{
	//	TutorialLevel->setIsPaused(true);
	//}
	////when player reaches the main level, play the main level song
	//else if(isBegun && controlswap)
	//{
	//	if(MainLevel->getIsPaused())
	//	{
	//		MainLevel->setIsPaused(false);
	//	}
	//}
	////dont play sound when win/lose/win tutorial is triggered
	//else if(tutoriallevel || win || lose)
	//{
	//	MainMenu->setIsPaused(true);
	//	TutorialLevel->setIsPaused(true);
	//}
	////when the player is actively using attack button
	//if(theHero->getAttack())
	//{
	//	if(Attack->getIsPaused())
	//	{
	//		Attack->setIsPaused(false);
	//	}
	//}
	//else
	//{
	//	Attack->setIsPaused(true);
	//	Attack = NULL;
	//}
	////when the player gets hit (true)
	//if(getHit)
	//{
	//	if(GetHit->getIsPaused())
	//	{
	//		GetHit->setIsPaused(false);
	//	}
	//}
	//else
	//{
	//	//reset the sound once completed
	//	GetHit = NULL;
	//}
	////when player is jumping
	//if(theHero->GetToJumpUpwards())
	//{
	//	if(Jump->getIsPaused())
	//	{
	//		Jump->setIsPaused(false);
	//	}
	//}
	//else
	//{
	//	//reset the sound once completed
	//	Jump = NULL;
	//}
	//
	////when the player successfully completes the tutorial level
	//if(tutoriallevel)
	//{
	//	if(WinTutorial->getIsPaused())
	//	{
	//		WinTutorial->setIsPaused(false);
	//	}
	//}

	////when the player successfully completes the main level
	//if(win)
	//{
	//	if(Win->getIsPaused())
	//	{
	//		Win->setIsPaused(false);
	//	}
	//	//stop the main level song
	//	MainLevel->setIsPaused(true);
	//}

	////when player loses
	//if(lose)
	//{
	//	if(Lose->getIsPaused())
	//	{
	//		Lose->setIsPaused(false);
	//	}
	//	TutorialLevel->setIsPaused(true);
	//	MainLevel->setIsPaused(true);
	//}
}
void SceneAssignment::ResetPosition()
{
	lives -= 1;

	//reset the position
	theHero->SetPos_x(TILE_SIZE*2);
	theHero->SetPos_y(TILE_SIZE*6);
	theHero->setMapOffset_x(0);
	theHero->setMapOffset_y(0);
}
void SceneAssignment::UpdateControls(double dt)
{
	// Update the hero
	//this set of controls is active when tutorial level is active
	//check whether there are tiles in front of the character. if there isn't you can move
	if(this->theHero->checkCollision(m_cMap, true, false, false, false) == false)
	{
		if(Application::IsKeyPressed('A'))
		{
			this->theHero->MoveLeftRight( true, 1.0f );
			this->theHero->SetActiveLeft(true);
			this->theHero->SetActiveRight(false);
			this->theHero->SetMoveLeft(true);
			this->theHero->SetMoveRight(false);
		}
	}
	if(this->theHero->checkCollision(m_cMap, false, true, false, false) == false)
	{
		if(Application::IsKeyPressed('D'))
		{
			this->theHero->MoveLeftRight( false, 1.0f );
			this->theHero->SetActiveLeft(false);
			this->theHero->SetActiveRight(true);
			this->theHero->SetMoveLeft(false);
			this->theHero->SetMoveRight(true);
		}
	}
	if(Application::IsKeyPressed('1'))
	{
		this->theHero->setAttack(true);
	}
	if(Application::IsKeyPressed(' '))
	{
		this->theHero->SetToJumpUpwards(true);
	}

	if(!Application::IsKeyPressed(' ') && !Application::IsKeyPressed('D') && !Application::IsKeyPressed('W') &&!Application::IsKeyPressed('A')&& !Application::IsKeyPressed('S'))
	{
		this->theHero->SetMoveLeft(false);
		this->theHero->SetMoveRight(false);
	}
	theHero->HeroUpdate(m_cMap);

}
void SceneAssignment::UpdateTileEffects(double dt)
{
	// ReCalculate the tile offsets
	tileOffset_x = (int) (theHero->GetMapOffset_x() / m_cMap->GetTileSize());
	if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
		tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();

	//if the hero enters the kill zone, then enemy goes into kill strategy mode
	int checkPosition_X = (int) ((theHero->GetMapOffset_x() + theHero->GetPos_x()) / m_cMap->GetTileSize());
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ((theHero->GetPos_y() + m_cMap->GetTileSize())/m_cMap->GetTileSize());

	int collision = theHero->GetPos_x() - (theEnemy->GetPos_x() - theHero->GetMapOffset_x());
	int collision2 = theHero->GetPos_x() - (theEnemy2->GetPos_x() - theHero->GetMapOffset_x());
	int collision3 = theHero->GetPos_x() - (theEnemy3->GetPos_x() - theHero->GetMapOffset_x());

	//this is when player is inside the complete level tile. in other words, he has successfully completed the tutorial level
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 6)
	{
		tutoriallevel = true;
	}
	//check whether player is in treasure help trigger tile
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 7)
	{
		treasurehelp = true;
	}
	else
	{
		//dont render outside of it
		treasurehelp = false;
	}

	//check whether player is within the first jump-able zone
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 9)
	{
		spacebarhelp = true;
	}
	else
	{
		//dont render outside of it 
		spacebarhelp = false;
	}

	//check whether player is near the enemy kill zone to trigger the help panel
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 12)
	{
		enemyhelp = true;
	}
	else
	{
		//dont render outside of it
		enemyhelp = false;
	}

	//check that when the player activates the attack button
	if(theHero->getAttack() == true)
	{
		//increase number of lives only when treasure effect is not activated yet
		if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 8 && treasureget == false)
		{
			treasureget = true;
			lives +=1;
		}
	}
	if(getHit)
	{
		getHitTime += (float) 5 * dt;

		if(getHitTime > 2)
		{
			getHit = false;
		}
	}
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 13)
		{
			theEnemy2->ChangeStrategy(new CStrategy_Kill());
			
			if(collision2 == 0 && !enemy2hit)
			{
				getHit = true;
				ResetPosition();
			}
			if(theHero->getAttack())
			{
				if(theHero->GetActiveLeft())
				{
					if(collision > -5)
					{
						enemy2hit = true;
					}

				}
				else if(theHero->GetActiveRight())
				{
					if(collision < 5)
					{
						enemy2hit = true;
					}
				}
			}
		}
		if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 14)
		{
			theEnemy3->ChangeStrategy(new CStrategy_Kill());

			if(collision3 == 0 && !enemy3hit)
			{
				getHit = true;
				ResetPosition();
			}
			if(theHero->getAttack())
			{
				if(theHero->GetActiveLeft())
				{
					if(collision > -5)
					{
						enemy3hit = true;
					}

				}
				else if(theHero->GetActiveRight())
				{
					if(collision < 5)
					{
						enemy3hit = true;
					}
				}
			}
		}
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 10)
	{
		//	theEnemy->SetDestination(theHero->GetPos_x(), theHero->GetPos_y());
		theEnemy->ChangeStrategy(new CStrategy_Kill());

		//this is when enemy manages to hit the player and is active
		if(collision == 0 && !enemy1hit)
		{
			getHit = true;
			ResetPosition();
		}
		if(theHero->getAttack())
		{
			if(theHero->GetActiveLeft())
			{
				if(collision > -5)
				{
					enemy1hit = true;
				}

			}
			else if(theHero->GetActiveRight())
			{
				if(collision < 5)
				{
					enemy1hit = true;
				}
			}
		}
	}
	else if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 11)
	{
		//theEnemy->SetDestination(theHero->GetPos_x(), theHero->GetPos_y());
		theEnemy->ChangeStrategy(NULL);
		//theEnemy->ChangeStrategy(new CStrategy_Run());

		if(controlswap == true)
		{
			theEnemy2->ChangeStrategy(NULL);
			theEnemy3->ChangeStrategy(NULL);
		}
	}
	if(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 15)
	{
		win = true;
	}
	if(lives ==  0)
	{
		lose = true;
	}
	//Update the enemies
	theEnemy->SetDestination((theHero->GetPos_x() + theHero->GetMapOffset_x()), theHero->GetPos_y());
	theEnemy->Update(m_cMap);

	if(controlswap == true)
	{
		//Update the new enemy positions
		theEnemy2->SetDestination((theHero->GetPos_x() + theHero->GetMapOffset_x()), theHero->GetPos_y());
		theEnemy2->Update(m_cMap);

		theEnemy3->SetDestination((theHero->GetPos_x() + theHero->GetMapOffset_x()), theHero->GetPos_y());
		theEnemy3->Update(m_cMap);
	}
}
void SceneAssignment::UpdateSpriteAnimations(double dt)
{
	//update sprite animation facing left
	SpriteAnimation *lico = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_IDLE_LEFT]);
	if(lico)
	{
		lico->Update(dt);
	}

	SpriteAnimation *licomove = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_MOVE_LEFT]);
	if(licomove)
	{
		licomove->Update(dt);
	}
	//update animation for sprite facing right
	SpriteAnimation *licoinv = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_IDLE_RIGHT]);
	if(licoinv)
	{
		licoinv->Update(dt);
	}
	SpriteAnimation *licomoveinv = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_MOVE_RIGHT]);
	if(licomoveinv)
	{
		licomoveinv->Update(dt);
	}
	//animation for sprite facing left and attacking
	SpriteAnimation *licoatk = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_ATK_LEFT]);
	if(lico)
	{
		licoatk->Update(dt);
	}
	//animation for sprite facing right and attacking
	SpriteAnimation *licoatkright = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILELICO_ATK_RIGHT]);
	if(licoatkright)
	{
		licoatkright->Update(dt);
	}
	SpriteAnimation *enemy = dynamic_cast<SpriteAnimation*>(meshList[GEO_TILE_ENEMY]);
	if(enemy)
	{
		enemy->Update(dt);
	}
}
void SceneAssignment::UpdateGame(double dt)
{
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera.Update(dt);

	UpdateControls(dt);
	UpdateTileEffects(dt);
	UpdateSpriteAnimations(dt);

	fps = (float)(1.f / dt);
}
void SceneAssignment::Update(double dt)
{
	UpdateSounds(dt);
	UpdateMainMenu(dt);
	UpdatePauseMenu(dt);
	UpdateWin(dt);
	UpdateLose(dt);
}

/********************************************************************************
Update Camera position
********************************************************************************/
void SceneAssignment::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}
void SceneAssignment::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	//	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//	glEnable(GL_DEPTH_TEST);
}
void SceneAssignment::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	//	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i + 0.5f, 0.3f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	//	glEnable(GL_DEPTH_TEST);
}
void SceneAssignment::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}
void SceneAssignment::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
void SceneAssignment::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void SceneAssignment::RenderHelp()
{
	if(treasurehelp == true)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_TILE_TREASUREHELP], false, TILE_SIZE*3, 0, 0);
		modelStack.PopMatrix();
	}
	if(spacebarhelp == true)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_TILE_SPACEBARHELP], false, TILE_SIZE*3, 0, 0);
		modelStack.PopMatrix();
	}

	if(enemyhelp == true)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_TILE_ENEMYHELP], false, TILE_SIZE*3, 0, 0);
		modelStack.PopMatrix();
	}
	//	if(spacebarhelp = true)
	{
		/*	modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_TILE_SPACEBARHELP], false, Application::getWindowWidth()*0.2, 0, 0);
		modelStack.PopMatrix();*/
	}
}
void SceneAssignment::RenderBackground()
{
	if(controlswap == false)
	{
		// Render the background
		Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f);
	}
	else
	{
		Render2DMesh(meshList[GEO_DARK_BACKGROUND], false, 1.0f);
	}
}
void SceneAssignment::RenderMainMenu()
{
	//opening screen render
	//before the player does anything
	if(isBegun == false && isStart == false && bRead == false)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_MENU], false, Application::getWindowWidth()*0.40, 400, 275);
		modelStack.PopMatrix();
	}
	//player press '1', this trigger isStart = true
	//main menu render
	if (isStart == true && isBegun == false && bRead == false) 
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_UI], false, TILE_SIZE*5, Application::getWindowWidth()*0.3 + TILE_SIZE*3, Application::getWindowWidth()*0.3);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "MENU" , Color(0, 0, 1), 50, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*6);

		if (iChoice2 == 1)
			RenderTextOnScreen(meshList[GEO_TEXT], "Start" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3 - 3, Application::getWindowWidth()*0.3 + TILE_SIZE);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Start" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3 - 3, Application::getWindowWidth()*0.3 + TILE_SIZE);
		if(iChoice2 == 2)
			RenderTextOnScreen(meshList[GEO_TEXT], "Inst" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3 - 3, Application::getWindowWidth()*0.3 - TILE_SIZE);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Inst" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3 - 3, Application::getWindowWidth()*0.3 - TILE_SIZE);
		if (iChoice2 == 3)
			RenderTextOnScreen(meshList[GEO_TEXT], "Exit" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.3 - TILE_SIZE*3);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Exit" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.3 - TILE_SIZE*3);
	}

	//instructions render
	if(isStart == true && isBegun == false && bRead == true)
	{
		RenderInstruction();
	}
}
void SceneAssignment::RenderInstruction()
{
	if(bReadPage1 == false && bReadPage2 == false && bReadPage3 == false)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_INST], false, Application::getWindowWidth()*0.3, 400, 300);
		modelStack.PopMatrix();
	}
	else if(bReadPage1 == true && bReadPage2 == false && bReadPage3 == false)
	{

		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_INST2], false, Application::getWindowWidth()*0.3, 400, 300);
		modelStack.PopMatrix();
	}
	else if(bReadPage1 == false && bReadPage2 == true && bReadPage3 == false)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_INST3], false, Application::getWindowWidth()*0.3, 400, 300);
		modelStack.PopMatrix();
	}
}
void SceneAssignment::RenderPauseMenu()
{
	if (isPaused == true && isBegun == true) 
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_UI], false, TILE_SIZE*5, Application::getWindowWidth()*0.3 + TILE_SIZE*4.5, Application::getWindowWidth()*0.3);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "PAUSED" , Color(0, 0, 1), 50, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*6);

		if (iChoice == 1)
			RenderTextOnScreen(meshList[GEO_TEXT], "Resume" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*4);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Resume" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*4);
		if (iChoice == 2)
			RenderTextOnScreen(meshList[GEO_TEXT], "Restart" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*2);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Restart" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2 + TILE_SIZE*2);
		if (iChoice == 3)
			RenderTextOnScreen(meshList[GEO_TEXT], "Exit" , Color(0, 0, 1), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2);
		else 
			RenderTextOnScreen(meshList[GEO_TEXT], "Exit" , Color(1, 0, 0), 40, Application::getWindowWidth()*0.3- 3, Application::getWindowWidth()*0.2);
	}
}
void SceneAssignment::RenderWin()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_WIN], false, Application::getWindowWidth()*0.3, 400, 300);
	modelStack.PopMatrix();
}
void SceneAssignment::RenderLose()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_LOSE], false, Application::getWindowWidth()*0.3, 400, 300);
	modelStack.PopMatrix();
}
void SceneAssignment::RenderWinOrLose()
{
	if(win == true && lose == false)
	{
		RenderWin();
	}
	if(win == false && lose == true)
	{
		RenderLose();
	}
	if(tutoriallevel == true)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_WIN_TUTORIAL], false, Application::getWindowWidth()*0.3, 400, 300);
		modelStack.PopMatrix();
	}
}
void SceneAssignment::RenderGame()
{
	// Render the background image
	RenderBackground();
	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render the goodies
	RenderGoodies();

	//render the GUI
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_GUI_LICO], false, 2.f, 0, TILE_SIZE*17);
	modelStack.PopMatrix();

	//On screen text
	/*std::ostringstream ss;
	ss.precision(5);
	ss << "mapoffset x: " << theHero->GetMapOffset_x();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);*/
	std::ostringstream sss;
	sss.precision(5);
	sss << "  X"<< lives;
	RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(1, 0, 1), 30, 0, TILE_SIZE*17);

	RenderMainMenu();
	RenderPauseMenu();
	RenderHelp();
	RenderWinOrLose();
}
void SceneAssignment::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderGame();
}
void SceneAssignment::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void SceneAssignment::RenderTileMap()
{
	int m = 0;
	for(int i = 0; i < m_cMap->GetNumOfTiles_Height(); i ++)
	{
		for(int k = 0; k < m_cMap->GetNumOfTiles_Width()+1; k ++)
		{
			m = tileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ( (tileOffset_x+k) >= m_cMap->getNumOfTiles_MapWidth() )
				break;
			if (m_cMap->theScreenMap[i][m] == 1)
			{
				Render2DMesh(meshList[GEO_TILEGROUND], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 2)
			{
				Render2DMesh(meshList[GEO_TILETREE], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 4)
			{
				Render2DMesh(meshList[GEO_TILE_FINISHLINE_STEM], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 5)
			{
				Render2DMesh(meshList[GEO_TILE_FINISHLINE_SIGN], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == 6)
			{
				Render2DMesh(meshList[GEO_TILE_FINISH], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == 7 || m_cMap->theScreenMap[i][m] == 9 || m_cMap->theScreenMap[i][m] == 12)
			{
				Render2DMesh(meshList[GEO_TILE_TUTORIAL_TRIGGER], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}

			else if(m_cMap->theScreenMap[i][m] == 8 && treasureget == false)
			{
				Render2DMesh(meshList[GEO_TILE_TREASURECHEST], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == 10 || m_cMap->theScreenMap[i][m] == 13 || m_cMap->theScreenMap[i][m] == 14)
			{
				Render2DMesh(meshList[GEO_TILE_KILLZONE], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == 11)
			{
				Render2DMesh(meshList[GEO_TILE_SAFEZONE], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == 15)
			{
				Render2DMesh(meshList[GEO_TILE_SOIL], false, 1.0f, k*m_cMap->GetTileSize()-theHero->GetMapFineOffset_x(), (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cMap->GetTileSize());
			}
		}
	}

	//active left refers to the sprite is actively looking leftwards
	//move left refers to the sprite actively moving towards the left
	if(theHero->GetActiveLeft() == true)
	{
		//check if sprite is moving
		if(theHero->GetMoveLeft() == false)
		{
			//check if it is jumping
			if(theHero->GetToJumpUpwards() == false)
			{
				if (theHero->GetAnimationInvert() == false && theHero->getAttack() == false)
					Render2DMesh(meshList[GEO_TILELICO_IDLE_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());		
			}
			//when it is jumping..
			else if (theHero->GetToJumpUpwards() == true && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_JUMP_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}
			//check if it is attacking
			if(theHero->getAttack() == true)
			{
				Render2DMesh(meshList[GEO_TILELICO_ATK_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
			}
		}
		else
		{
			//check if it is jumping and it is not attacking
			if(theHero->GetToJumpUpwards() == false && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_MOVE_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}
			//when it is jumping and it is not attacking
			else if(theHero->GetToJumpUpwards() == true && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_JUMP_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}

			//check if it is attacking
			if(theHero->getAttack() == true)
			{
				Render2DMesh(meshList[GEO_TILELICO_ATK_LEFT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
			}
		}	
	}
	//active left refers to the sprite is actively looking leftwards
	//move left refers to the sprite actively moving towards the left
	if(theHero->GetActiveRight() == true)
	{
		//check if sprite is moving
		if(theHero->GetMoveRight() == false)
		{
			//check if it is jumping
			if(theHero->GetToJumpUpwards() == false)
			{
				if (theHero->GetAnimationInvert() == false && theHero->getAttack() == false)
					Render2DMesh(meshList[GEO_TILELICO_IDLE_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());		
			}
			//when it is jumping..
			else if (theHero->GetToJumpUpwards() == true && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_JUMP_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}
			//check if it is attacking
			if(theHero->getAttack() == true)
			{
				Render2DMesh(meshList[GEO_TILELICO_ATK_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
			}
		}
		else
		{
			//check if it is jumping and it is not attacking
			if(theHero->GetToJumpUpwards() == false && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_MOVE_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}
			//when it is jumping and it is not attacking
			else if(theHero->GetToJumpUpwards() == true && theHero->getAttack() == false)
			{
				Render2DMesh(meshList[GEO_TILELICO_JUMP_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
				theHero->SetAnimationInvert(false);
			}

			//check if it is attacking
			if(theHero->getAttack() == true)
			{
				Render2DMesh(meshList[GEO_TILELICO_ATK_RIGHT], false, 1.f, theHero->GetPos_x(), theHero->GetPos_y());
			}
		}	
	}

	//Render the enemy
	int theEnemy_x = theEnemy->GetPos_x()-theHero->GetMapOffset_x();
	int theEnemy_y = theEnemy->GetPos_y();

	if((theEnemy_x >= 0) && (theEnemy_x < Application::getWindowWidth())&& ((theEnemy_y >= 0) && (theEnemy_y < Application::getWindowHeight())) && !enemy1hit )
	{
		Render2DMesh(meshList[GEO_TILE_ENEMY], false, 1.0f, theEnemy_x, theEnemy_y);
	}

	if(controlswap == true)
	{
		//Render the enemy2
		int theEnemy2_x = theEnemy2->GetPos_x()-theHero->GetMapOffset_x();
		int theEnemy2_y = theEnemy2->GetPos_y();

		if((theEnemy2_x >= 0) && (theEnemy2_x < Application::getWindowWidth())&& ((theEnemy2_y >= 0) && (theEnemy2_y < Application::getWindowHeight())) && !enemy2hit )
		{
			Render2DMesh(meshList[GEO_TILE_ENEMY], false, 1.0f, theEnemy2_x, theEnemy2_y);
		}

		//Render the enemy
		int theEnemy3_x = theEnemy3->GetPos_x()-theHero->GetMapOffset_x();
		int theEnemy3_y = theEnemy3->GetPos_y();

		if((theEnemy3_x >= 0) && (theEnemy3_x < Application::getWindowWidth())&& ((theEnemy3_y >= 0) && (theEnemy3_y < Application::getWindowHeight())) && !enemy3hit )
		{
			Render2DMesh(meshList[GEO_TILE_ENEMY], false, 1.0f, theEnemy3_x, theEnemy3_y);
		}
	}
}
/********************************************************************************
Render the rear tile map. This is a private function for use in this class only
********************************************************************************/
void SceneAssignment::RenderRearTileMap()
{
	rearWallOffset_x = (int) (theHero->GetMapOffset_x() / 2);
	rearWallOffset_y = 0;
	rearWallTileOffset_y = 0;
	rearWallTileOffset_x = (int) (rearWallOffset_x / m_cRearMap->GetTileSize());
	if (rearWallTileOffset_x+m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->getNumOfTiles_MapWidth())
		rearWallTileOffset_x = m_cRearMap->getNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

	int m = 0;
	for(int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i ++)
	{
		for(int k = 0; k < m_cRearMap->GetNumOfTiles_Width()+1; k ++)
		{
			m = rearWallTileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ( (rearWallTileOffset_x+k) >= m_cRearMap->getNumOfTiles_MapWidth() )
				break;
			if (m_cRearMap->theScreenMap[i][m] == 3 && controlswap == false)
			{
				Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1.0f, k*m_cRearMap->GetTileSize()-rearWallFineOffset_x, (Application::getWindowHeight()-(2*TILE_SIZE))-i*m_cRearMap->GetTileSize());
			}
		}
	}
}
void SceneAssignment::RenderGoodies()
{
	////Render the goodies
	//for(int i = 0; i < lives; i++)
	//{
	//	Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, 1.0f, theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	//}
}