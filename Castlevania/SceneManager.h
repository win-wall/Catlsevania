#pragma once
#include "Game.h"
#include "Ground.h"
#include "Candle.h"
#include "Utils.h"
#include "Texture.h"
#include "Sprite.h"
#include "Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include "Simon.h"
#include "Whip.h"
#include "TileMap.h"
#include "Item.h"
#include "SubWeapon.h"
#include "Stair.h"
#include "ChangeScene.h"
#include "Black_Knight.h"
#include "Bat.h"
#include "Platform.h"
#include "Grid.h"
#include  "Ghost.h"
namespace core
{
	class SceneManager
	{
		
		
		CGame* game;
		Grid* grid;
		Unit* unit;
		
		static_object::Ground* ground;
		static_object::Candle* candle;
		static_object::Item* item;
		simon::Simon* simons;
		simon::Whip* whip;
		simon::SubWeapon* subweapon;
		static_object::Stair* stair;
		static_object::ChangeScene* changeScene;
		enemy::Black_Knight* black_knight;
		enemy::Bat* bat;
		enemy::Ghost* ghost;
		item::Platform* platform;
		
		TileMaps* tilemaps = TileMaps::GetInstance();
		CTexture* textures = CTexture::GetInstance();
		CSprites* sprites = CSprites::GetInstance();
		CAnimations* animations = CAnimations::GetInstance();

		vector<Unit*> listUnits;
		vector<LPGAMEOBJECT>ListStaticObjectToRender;
		vector<LPGAMEOBJECT>ListEnemyToRender;
		vector<LPGAMEOBJECT>List_Objects_In_Game;
		vector<static_object::LPCHANGESCENEOBJ> listChangeSceneObjs;
		vector<LPGAMEOBJECT> List_Stairs;
		vector<LPGAMEOBJECT>List_Black_Knight;
		int ID_scene;

		
	public:
		SceneManager(CGame* game, int ID_scene);
		~SceneManager();

		void LoadResources();
		void LoadObjectsFromFile(LPCWSTR FilePath);
		void CreateListChangeSceneObjects();
		
		virtual void Update(DWORD dt);
		virtual void Render();
		
		void Init(int id_scene);
		void SetGameState(int state);
		int Get_ID_SCENE() { return this->ID_scene; }

		void Update_Grid();
		void Get_Obj_From_Grid();
		
		
		simon::Simon* Get_Simon() { return this->simons; }
		simon::SubWeapon* Get_Sub_Weapon() { return this->subweapon; };
		vector<LPGAMEOBJECT>* Get_List_Stairs() { return &(this->List_Stairs); }

		//Hàm set status của các item và object khi ra ngoài tầm camera
		void SetStatusByCameraPosition();
	};
}
