#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include<iostream>
#include<fstream>
#include<string>
#include <memory>

#include <GL/gl.h>

#include "common.h"
#include "GameAssetManager.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Player.h"
#include "CubeAsset.h"
#include "DiamondAsset.h"

/**
 * GameWorld allows us to separate the management of the game world from the
 * nuts and bolts of game loop initialisation.  The GameWorld currently has
 * a very simplified scene graph consisiting of a single GameAssetManager.
 */
class GameWorld {
 public:
  /**
   * We thread the ApplicationMode through the GameWorld ss we want to read it
   * in from the user.  Threading the state through the various function calls
   * is preferable (in this case) to having some kind of global state.
   */
  GameWorld(ApplicationMode);

  /**
 * moveF calls the Player Class moveF function to allow the camera to 'walk' forwards
 */
  void moveF();
  
  /**
 * moveB calls the Player Class moveB function to allow the camera to 'walk' backwards
 */
  void moveB();

  /**
 * moveL calls the Player Class moveL function to allow the camera to 'walk' to the left
 */
  void moveL();

  /**
 * moveR calls the Player Class moveR function to allow the camera to 'walk' to the right
 */
  void moveR();

  /**
 * moveU calls the Player Class moveU function to allow the camera to 'fly' upwards
 */
  void moveU();

  /**
 * moveD calls the Player Class moveD function to allow the camera to 'fly' downwards
 */
  void moveD();

  /**
 * setCamera calls the Player Class setCamera function to adjust the camera view. Passed in are
 * the x and y values that the camera has to move by
 */
  void setCamera(GLfloat, GLfloat);
  /**
   * Calling Draw() will draw the entire world.
   */
  void Draw();

  /**
 * loadCubesFromImage will load a plain ppm file, and use each pixels colour and position to place a
 * cube in the GameWorld. Any pixels that are completly black (RGB 0,0,0) will be ignored as to allow
 * the creation of something like a maze
 */
  void loadCubesFromImage(std::string);
 private:

  std::shared_ptr<GameAssetManager> asset_manager;
  std::shared_ptr<Player> player;

  
};
#endif // GAMEWORLD_H
