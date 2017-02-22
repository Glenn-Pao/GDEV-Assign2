#ifndef SCENE_ASSIGNMENT_H
#define SCENE_ASSIGNMENT_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Minimap.h"
#include "Map.h"
#include "Vector2.h"
#include "PlayerInfo.h"
#include "Enemy.h"
#include "Sound.h"

//Goodies and Goodies Factory
#include "Goodies.h"
#include "GoodiesFactory.h"
#include "TreasureChest.h"

class SceneAssignment : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_QUAD,

		GEO_UI,
		GEO_MENU,
		GEO_HEALTH,
		GEO_WIN,
		GEO_WIN_TUTORIAL,
		GEO_LOSE,

		GEO_INST,
		GEO_INST2,
		GEO_INST3,

		GEO_GUI_LICO,

		GEO_BACKGROUND,
		GEO_DARK_BACKGROUND,
		GEO_TILEGROUND,
		GEO_TILEHERO,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILE_SOIL,

		GEO_TILE_FINISHLINE_SIGN,
		GEO_TILE_FINISHLINE_STEM,

		GEO_TILE_SPACEBARHELP,
		GEO_TILE_TREASUREHELP,
		GEO_TILE_ENEMYHELP,

		GEO_TILELICO_IDLE_LEFT,
		GEO_TILELICO_IDLE_RIGHT,
		GEO_TILELICO_MOVE_LEFT,
		GEO_TILELICO_MOVE_RIGHT,
		GEO_TILELICO_JUMP_LEFT,
		GEO_TILELICO_JUMP_RIGHT,
		GEO_TILELICO_ATK_LEFT,
		GEO_TILELICO_ATK_RIGHT,

		GEO_TILE_TUTORIAL_TRIGGER,
		GEO_TILE_FINISH,

		GEO_TILE_KILLZONE,
		GEO_TILE_SAFEZONE,

		GEO_TILE_ENEMY,

		GEO_TILE_TREASURECHEST,
		GEO_OBJECT,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

public:
	SceneAssignment();
	~SceneAssignment();

	virtual void Init();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	virtual void Render();
	virtual void Exit();

	//OpenGL parameters initialization
	void InitParameters();

	//menu system..
	virtual void UpdateMainMenu(double dt);
	void RenderMainMenu();

	void RenderInstruction();

	virtual void UpdatePauseMenu(double dt);
	void RenderPauseMenu();

	void UpdateGame(double dt);
	void RenderGame();

	void UpdateWin(double dt);
	void RenderWin();

	void UpdateLose(double dt);
	void RenderLose();

	void RenderWinOrLose();

	void RenderHelp();
		
	void UpdateSounds(double dt);			//trigger sounds at relevant locations
	void ResetPosition();					//when player gets hit, reset and deduct 1 life

	void UpdateControls(double dt);			//update your player controls
	void UpdateTileEffects(double dt);		//update the tile effect collisions
	void UpdateSpriteAnimations(double dt);	//update of sprite animations here

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTextOnScreen2(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, bool rotate=false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderBackground();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false, const bool flip=false);

	static bool bReset;	//resets the game if true
	static bool bExit;	//exits the app if true


	static bool getControlswap(void);
	//static bool getExit(void);
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;

	// Handle to the minimap
	CMinimap* m_cMinimap;

	// Handle to the tilemaps
	CMap* m_cMap;
	void RenderTileMap();
	// Hero's information
	CPlayerInfo* theHero;

	// Codes for Scrolling
	int tileOffset_x, tileOffset_y;

	// Codes for Parallax Scrolling
	CMap* m_cRearMap;
	void RenderRearTileMap();
	int rearWallOffset_x, rearWallOffset_y;
	int rearWallTileOffset_x, rearWallTileOffset_y;
	int rearWallFineOffset_x, rearWallFineOffset_y;

	//Enemies
	CEnemy* theEnemy;
	CEnemy* theEnemy2;
	CEnemy* theEnemy3;

	//Goodies and Goodies factory
	CGoodiesFactory theGoodiesFactory;
	CGoodies** theArrayOfGoodies;
	void RenderGoodies(void);

	bool isPaused;		//pauses the game if true
	bool isBegun;		//triggers the menu if true
	bool isStart;		//triggers the start menu is true
	int iChoice;		//the choices available for pause menu
	int iChoice2;		//the choices available for main menu

	bool bRead;			//triggers the instructions panel when true
	bool bReadPage1;	//triggers first instruction panel
	bool bReadPage2;	//triggers second instruction panel
	bool bReadPage3;	//triggers third instruction panel

	bool win;			//when player completes all the levels
	bool lose;			//when player lose

	bool treasurehelp;	//triggers the treasure help screen when true
	bool spacebarhelp;	//triggers spacebar help screen when true
	bool enemyhelp;		//triggers help panel on how to handle an enemy when true

	bool treasureget;	//toggle to true when treasure is gained in tutorial
	int lives;			//number of lives for the player

	bool getHit;		//trigger true when player gets hit by enemy
	float getHitTime;	//this float value will allow the game to render the "GetHit" sound before flipping the boolean to false.

	bool tutoriallevel;	//it is tutorial level when true
	static bool controlswap;	//this set of controls work on tutorial levels only. false = side scroll
	

	bool enemy1hit;		//when the enemy is successfully hit
	bool enemy2hit;		//when the enemy2 is successfully hit
	bool enemy3hit;		//when the enemy3 is successfully hit

	CSound *sound;		//Handle to the sound system
};

#endif