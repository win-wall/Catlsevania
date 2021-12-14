#include "SceneManager.h"
namespace core
{
	SceneManager::SceneManager(CGame* game,int ID_scene)
	{
		this->game = game;
		this->ID_scene = ID_scene;

		
	}
	SceneManager::~SceneManager()
	{
	}
	void SceneManager::Init(int id_scene)
	{
		ID_scene = id_scene;
		switch (id_scene)
		{
		case SCENE_1:
			grid = new Grid(1536, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
			LoadObjectsFromFile(L"Scenes\\Scene1_objects.txt");
			SetGameState(SCENE_1); 
			CreateListChangeSceneObjects();
			break;
		case SCENE_2:
			grid = new Grid(544, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
			LoadObjectsFromFile(L"Scenes\\Scene2_objects.txt");
			SetGameState(SCENE_2);
			CreateListChangeSceneObjects(); 
				break;
		case SCENE_2_2:
			grid = new Grid(1056, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
			LoadObjectsFromFile(L"Scenes\\Scene2.2_objects.txt");
			SetGameState(SCENE_2_2);
			CreateListChangeSceneObjects();
			break;
		case SCENE_3:
			grid = new Grid(49 * 32, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
			LoadObjectsFromFile(L"Scenes\\Scene3_objects.txt");
			SetGameState(SCENE_3);
			CreateListChangeSceneObjects();
			break;
		}
	}
	void SceneManager::LoadResources()
	{
		simons = new simon::Simon();
		simons->LoadResources(textures, sprites, animations);
		
		ground = new static_object::Ground();
		ground->LoadResources(textures, sprites, animations);
		
		subweapon = new simon::SubWeapon();
		subweapon->LoadResources(textures, sprites, animations);
		
		whip = new simon::Whip();
		whip->LoadResources(textures, sprites, animations);
		
		candle = new static_object::Candle();
		candle->LoadResources(textures, sprites, animations);

		item = new static_object::Item();
		item->LoadResources(textures, sprites, animations);

		stair = new static_object::Stair();
		stair->LoadResources(textures, sprites, animations);

		black_knight = new enemy::Black_Knight();
		black_knight->LoadResources(textures, sprites, animations);

		bat = new enemy::Bat();
		bat->LoadResources(textures, sprites, animations);

		ghost = new enemy::Ghost();
		ghost->LoadResources(textures, sprites, animations);
		
		platform = new item::Platform;
		platform->LoadResources(textures, sprites, animations);
		
		tilemaps->Add(SCENE_1, L"TileMap\\Scene_1_map.txt", L"Textures\\Scene_1_bank.png", 47 * 32, 11 * 32, 32, 32);
		tilemaps->Add(SCENE_2, L"TileMap\\Scene_2_map.txt", L"Textures\\Scene_2_bank.png", 16 * 32, 12 * 32, 32, 32);
		tilemaps->Add(SCENE_2_2, L"TileMap\\Scene_2.2_map.txt", L"Textures\\Scene_2.2_bank.png", 32 * 32, 12 * 32, 32, 32);
		tilemaps->Add(SCENE_3, L"TileMap\\Scene_3_map.txt", L"Textures\\Scene_3_bank.png", 48 * 32, 12 * 32, 32, 32);
		textures->Add(ID_TEXTURE_BBOX, L"Textures\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));

		simons = new simon::Simon();
		subweapon = new simon::SubWeapon();
	}
	void SceneManager::LoadObjectsFromFile(LPCWSTR FilePath)
	{
		if(List_Objects_In_Game.size()==0)
		{
			simons = new simon::Simon();
			List_Objects_In_Game.push_back(simons);
			subweapon = new simon::SubWeapon();
			subweapon->SetEnable(false);
			List_Objects_In_Game.push_back(subweapon);
		}
		
		for (int i = 0; i < List_Objects_In_Game.size(); i++)
			{
				if (dynamic_cast<simon::Simon*>(List_Objects_In_Game[i]))
				{
					continue;
				}
				else if (dynamic_cast<simon::SubWeapon*>(List_Objects_In_Game[i]))
				{
					continue;
				}
				else
				{
					List_Objects_In_Game.erase(List_Objects_In_Game.begin() + i);
					i--;
				}
			}
			fstream fs;
			fs.open(FilePath, ios::in);
			if (fs.fail())
			{
				DebugOut(L"[ERROR]  Load data failed: file path = %s\n", FilePath);
				fs.close();
				return;
			}

			int ID_Obj;
			float pos_x, pos_y;
			int status;
			bool isEnable;
			int idItem;
			float start_x;
			float end_x;

			while (!fs.eof())
			{
				fs >> ID_Obj;
				if (ID_Obj != BLACK_KNIGHT && ID_Obj != PLATFORM)
				{
					fs >> pos_x >> pos_y >> status >> isEnable >> idItem;
					switch (ID_Obj)
					{
					case GROUND:
						ground = new static_object::Ground();
						ground->SetPosition(pos_x, pos_y);
						ground->SetStatus(status);
						ground->SetEnable(isEnable);
						ground->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, ground, pos_x, pos_y);
						break;
					case CANDLE:
						candle = new static_object::Candle();
						candle->SetPosition(pos_x, pos_y);
						candle->SetStatus(status);
						candle->SetEnable(isEnable);
						candle->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, candle, pos_x, pos_y);
						break;
					case STAIR:
						stair = new static_object::Stair();
						stair->SetPosition(pos_x, pos_y);
						stair->SetStatus(status);
						stair->SetEnable(isEnable);
						stair->Set_Id_item_that_obj_carry(idItem);

						unit = new Unit(grid, stair, pos_x, pos_y);
						break;
					case BAT:
						bat = new enemy::Bat();
						bat->SetPosition(pos_x, pos_y);
						bat->Set_Start_y(pos_y);
						bat->SetStatus(status);
						bat->SetEnable(isEnable);
						bat->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, bat, pos_x, pos_y);
						break;
					case GHOST:
						ghost = new enemy::Ghost();
						ghost->SetPosition(pos_x, pos_y);
						ghost->SetStatus(status);
						ghost->SetEnable(isEnable);
						ghost->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, ghost, pos_x, pos_y);
					default:
						break;
					}
				}
				else
				{
					fs >> pos_x >> pos_y >> status >> isEnable >> idItem >> start_x >> end_x;
					switch (ID_Obj)
					{
					case BLACK_KNIGHT:
						black_knight = new enemy::Black_Knight();
						black_knight->SetPosition(pos_x, pos_y);
						black_knight->SetStatus(status);
						black_knight->SetEnable(isEnable);
						black_knight->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, black_knight, pos_x, pos_y);
						black_knight->Set_start_end(start_x, end_x);
						break;
					case PLATFORM:
						platform = new item::Platform();
						platform->SetPosition(pos_x, pos_y);
						platform->SetStatus(status);
						platform->SetEnable(isEnable);
						platform->Set_Id_item_that_obj_carry(idItem);
						unit = new Unit(grid, platform, pos_x, pos_y);
						platform->Set_start_end(start_x, end_x);
						break;
					}
				}
			}
		
	}
	void SceneManager::Update(DWORD dt)
	{
		int status;
		simons->GetStatus(status);
		//Cập nhật simon để chuyển màn
		if (simons->Get_Change_Scene()!=-1)
		{
			if(ID_scene == SCENE_1 ||(ID_scene==SCENE_2&&status==STAIR_UP) || ID_scene==SCENE_2_2)
			{
				ID_scene = simons->Get_Change_Scene();
				Init(ID_scene);
				simons->Set_Change_Scene(-1);
				
			}
		}
		Get_Obj_From_Grid();
		for(int i=0;i<List_Objects_In_Game.size();i++)
		{
			if (List_Objects_In_Game[i]->Is_Enable == false)
				continue;
			vector<LPGAMEOBJECT>co0bjects;
			if (dynamic_cast<simon::Simon*>(List_Objects_In_Game[i]))
			{
				for (int j = 0; j < List_Objects_In_Game.size(); j++)
				{
					if (List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					 //nạp tất cả object đang enable vào hàm update simon
					if(dynamic_cast<static_object::Stair*>(List_Objects_In_Game[j]) == nullptr)
					co0bjects.push_back(List_Objects_In_Game[j]);
				}
				simons->CheckChangeScene(&listChangeSceneObjs);
			}
			else if(dynamic_cast<static_object::Item*>(List_Objects_In_Game[i]))
			{
				for(int j=0;j<List_Objects_In_Game.size();j++)
				{
					if(List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					if (dynamic_cast<static_object::Ground*>(List_Objects_In_Game[j]))
					{
						co0bjects.push_back(List_Objects_In_Game[j]);// thêm tất cả object là ground vào hàm update item
					}
				}
			}
			else if(dynamic_cast<simon::SubWeapon*>(List_Objects_In_Game[i]))
			{
				for (int j = 1; j < List_Objects_In_Game.size(); j++)
				{
					if (List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					if (dynamic_cast<static_object::Ground*>(List_Objects_In_Game[j])) //Nạp tất cả object là Ground vào co0jects trong hàm update Subweapon de xu lý va chạm
					{
						co0bjects.push_back(List_Objects_In_Game[j]);
					}
					if (dynamic_cast<static_object::Candle*>(List_Objects_In_Game[j]))//Nạp tất cả object là candle vao co0jects trong hàm update Subweapon de xu ly va cham
					{
						co0bjects.push_back(List_Objects_In_Game[j]);
					}
				}
				
			}
			else if (dynamic_cast<enemy::Black_Knight*>(List_Objects_In_Game[i]))
			{
				for (int j = 1; j < List_Objects_In_Game.size(); j++)
				{
					if (List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					if (dynamic_cast<static_object::Ground*>(List_Objects_In_Game[j])) 
					{
						co0bjects.push_back(List_Objects_In_Game[j]);
					}
					float sx13, sy13;
					simons->GetPosition(sx13, sy13);
					
					black_knight->Is_Simon_in_the_target(sx13, sy13,simons->Get_isTouchAble());
					
				}
			}
			else if (dynamic_cast<enemy::Bat*>(List_Objects_In_Game[i]))
			{
				for (int j = 1; j < List_Objects_In_Game.size(); j++)
				{
					if (List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					float sx13, sy13;
					simons->GetPosition(sx13, sy13);
					bat->Is_Simon_in_the_target(sx13, sy13);
				}
			}
			else if(dynamic_cast<item::Platform*>(List_Objects_In_Game[i]))
			{
				for (int j = 1; j < List_Objects_In_Game.size(); j++)
				{
					if (List_Objects_In_Game[j]->Is_Enable == false)
						continue;
					if (dynamic_cast<static_object::Ground*>(List_Objects_In_Game[j])) 
					{
						co0bjects.push_back(List_Objects_In_Game[j]);
					}
				}
			}
			else if (dynamic_cast<enemy::Ghost*>(List_Objects_In_Game[i]))
			{
				for(int j=1;j<List_Objects_In_Game.size();j++)
				{
					if(List_Objects_In_Game[j]->Is_Enable==false)
					{
						continue;
					}
					float sx13, sy13;
					simons->GetPosition(sx13, sy13);
					ghost->Is_Simon_in_the_target(sx13, sy13,true);
				}
			}
			else
			{
				co0bjects.push_back(List_Objects_In_Game[i]);
			}
			List_Objects_In_Game[i]->Update(dt, &List_Objects_In_Game, &co0bjects);
			
		}
		Update_Grid();
		//cập nhật vị trí CAM
		float sx, sy;
		simons->GetPosition(sx, sy);
		float cx, cy;
		game->GetCameraPositon(cx, cy);
		if (sx > SCREEN_WIDTH / 2 && sx + SCREEN_WIDTH / 2 -16< tilemaps->Get(ID_scene)->Get_Map_Width())
			game->SetCamaraPosition(sx - SCREEN_WIDTH / 2, 0);
		//va chạm cuối map. -70 là right của sprite simon
		if(sx>=tilemaps->Get(ID_scene)->Get_Map_Width()-32)
		{
			sx= tilemaps->Get(ID_scene)->Get_Map_Width()-32;
			simons->SetPosition(sx, sy);
		}
		//
		SetStatusByCameraPosition();
	}
	void SceneManager::Render()
	{
		float cam_x, cam_y;
		game->GetCameraPositon(cam_x, cam_y);
		tilemaps->Get(ID_scene)->Draw(cam_x, cam_y);
		for (int i = 0; i < List_Objects_In_Game.size(); i++)
		{
			if (List_Objects_In_Game[i]->Is_Enable == false)
				continue;

			List_Objects_In_Game[i]->Render();
			List_Objects_In_Game[i]->RenderBBox();
			
		}
	}
	void SceneManager::SetGameState(int state)
	{
		switch (state)
		{
		case SCENE_1:
			simons->SetPosition(0.0f,0.0f);
			game->SetCamaraPosition(0.0f,0.0f);
			break;
		case SCENE_2:
			simons->SetPosition(0.0f, 340.0f);
			game->SetCamaraPosition(0.0f, 0.0f);
			break;
		case SCENE_2_2:
			simons->SetPosition(722.0f, 390.0f);
			int status;
			simons->GetStatus(status);
			if(status!=STAIR_UP)
			{
				simons->SetStatus(STAIR_UP);
				simons->SetOrientation_x(1);
			}
			game->SetCamaraPosition(544.0f, 0.0f);
			break;
		case SCENE_3:
			simons->SetPosition(1472.0f, 224.0f);
			game->SetCamaraPosition(47 * 32 - SCREEN_WIDTH+45, 0.0f);
			break;
		}
	}
	void SceneManager::CreateListChangeSceneObjects()
	{
		listChangeSceneObjs.clear();

		switch (ID_scene)
		{
		case SCENE_1:
			changeScene = new static_object::ChangeScene(1366, 295, SCENE_2);
			listChangeSceneObjs.push_back(changeScene);
			break;
		case SCENE_2:
			changeScene = new static_object::ChangeScene(288, 64, SCENE_2_2);
			listChangeSceneObjs.push_back(changeScene);
			break;
		case SCENE_2_2:
			changeScene = new static_object::ChangeScene(0, 156, SCENE_3);
			listChangeSceneObjs.push_back(changeScene);
			break;
		}
	}
	void SceneManager::Get_Obj_From_Grid()
	{
		for(int i= 0;i<List_Objects_In_Game.size();i++)
		{
			if (dynamic_cast<simon::Simon*>(List_Objects_In_Game[i]))
			{
				continue;
			}
			else if (dynamic_cast<simon::SubWeapon*>(List_Objects_In_Game[i]))
			{
				continue;
			}
			else if(dynamic_cast<static_object::Item*>(List_Objects_In_Game[i]))
			{
				continue;
			}
			else
			{
				List_Objects_In_Game.erase(List_Objects_In_Game.begin()+i);
				i--;
			}
		}
		listUnits.clear();
		List_Stairs.clear();
		float x, y;
		game->GetCameraPositon(x, y);
		grid->Get(x, y, listUnits);
		//DebugOut(L"%d \n", listUnits.size());
		for (int i = 0; i < listUnits.size(); i++)
		{
			LPGAMEOBJECT obj = listUnits[i]->GetObj();
			List_Objects_In_Game.push_back(obj);
			if(dynamic_cast<static_object::Stair*>(obj))
			{
				List_Stairs.push_back(obj);
			}
		}
	}
	void SceneManager::Update_Grid()
	{
		for (int i = 0; i < listUnits.size(); i++)
		{
			float newPos_x, newPos_y;
			listUnits[i]->GetObj()->GetPosition(newPos_x, newPos_y);
			listUnits[i]->Move(newPos_x, newPos_y);
		}
	}


	void SceneManager::SetStatusByCameraPosition()
	{
		bool bool_subweapon;
		subweapon->GetEnable(bool_subweapon);
		float wx, wy;
		float cx1, cy1;
		if (bool_subweapon == true)
		{
			//Subweapon ra khỏi camera thì sẽ biến mất
			
			subweapon->GetPosition(wx, wy);
			game->GetCameraPositon(cx1, cy1);
			if (wx > cx1 + SCREEN_WIDTH - 30 || wy > cy1 + SCREEN_WIDTH - 30 || wx < cx1 || wy < cy1)
			{
				subweapon->SetEnable(false);
			}
		}
		//item ra khỏi camera thì sẽ biến mất
		for (int i = 0; i < List_Objects_In_Game.size(); i++)
		{
			if (dynamic_cast<static_object::Item*>(List_Objects_In_Game[i]))
			{
				game->GetCameraPositon(cx1, cy1);
				float ix, iy;
				List_Objects_In_Game[i]->GetPosition(ix, iy);
				if (ix > cx1 + SCREEN_WIDTH || iy > cy1 + SCREEN_WIDTH || ix < cx1 || iy < cy1)
				{
					List_Objects_In_Game[i]->SetEnable(false);
				}
			}
			if(dynamic_cast<enemy::Ghost*>(List_Objects_In_Game[i]))
			{
				game->GetCameraPositon(cx1, cy1);
				float gx, gy;
				List_Objects_In_Game[i]->GetPosition(gx, gy);
				if(gx>cx1+SCREEN_WIDTH-32||gy>cy1+SCREEN_WIDTH||gx<cx1||gy<cy1)
				{
					List_Objects_In_Game[i]->SetEnable(false);
				}
			}
			if (dynamic_cast<enemy::Bat*>(List_Objects_In_Game[i]))
			{
				game->GetCameraPositon(cx1, cy1);
				float gx, gy;
				List_Objects_In_Game[i]->GetPosition(gx, gy);
				if (gx > cx1 + SCREEN_WIDTH - 32 || gy > cy1 + SCREEN_WIDTH || gx < cx1 || gy < cy1)
				{
					List_Objects_In_Game[i]->SetEnable(false);
				}
			}
		}
	}

}