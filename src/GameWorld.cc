#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {
  asset_manager = std::make_shared<GameAssetManager>(mode);


  asset_manager->AddAsset(std::make_shared<DiamondAsset>(glm::vec3(2,2,2), glm::vec3(0.1,0.2,0.3)));

  loadCubesFromImage("src/New1.ppm");
  player = std::make_shared<Player>(asset_manager->return_token());

}


void GameWorld::loadCubesFromImage(std::string location){
  
  std::ifstream input(location.c_str());

  std::string type;
  input >> type;
  int col;
  int row;
  int colour;
  input >> col;
  input >> row;
  input >> colour;

 
  
  for(int i = 0; i < row; i++){
  for(int ii = 0; ii < col; ii++){
      int colorR, colorG, colorB;

      input >> colorR;
      input >> colorG;
      input >> colorB;      

	 if(colorR+colorG+colorB > 0){  
	    glm::vec3 temppos = glm::vec3(i, 0, ii);
	    glm::vec3 tempcolor = glm::vec3(colorR/255.0, colorG/255.0, colorB/255.0);
	    asset_manager->AddAsset(std::make_shared<CubeAsset>(temppos, tempcolor));
    	}
    }
  } 

  input.close();

	
}

//basic move functions
void GameWorld::moveF(){
player->moveF();

}

void GameWorld::moveB(){
player->moveB();

}

void GameWorld::moveL(){
player->moveL();

}

void GameWorld::moveR(){
 player->moveR();
}

void GameWorld::moveU(){
 player->moveU();
}

void GameWorld::moveD(){
 player->moveD();
}

void GameWorld::setCamera(GLfloat x, GLfloat y){
  player->setCamera(x,y);

}

void GameWorld::Draw() {
  player->Draw();
  asset_manager->Draw();
  //removeBlock();
}
