#define GLEW_STATIC // Easier debugging
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <boost/program_options.hpp>

#define RUN_GRAPHICS_DISPLAY 0x00;

#include "common.h"
#include "GameWorld.h"

const Uint8* keystates;
bool wheelup=false;
bool wheeldown = false;

/*
 * SDL timers run in separate threads.  In the timer thread
 * push an event onto the event queue.  This event signifies
 * to call display() from the thread in which the OpenGL 
 * context was created.
 */
Uint32 tick(Uint32 interval, void *param) {
  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = RUN_GRAPHICS_DISPLAY;
  event.user.data1 = 0;
  event.user.data2 = 0;
  SDL_PushEvent(&event);
  return interval;
}

struct SDLWindowDeleter {
  inline void operator()(SDL_Window* window) {
    SDL_DestroyWindow(window);
  }
};

  /**
 * Updates handles all the user inputs every frame. Passed in the game_world pointer
 * to allow for calling game_world functions.
 */
void Update(const std::shared_ptr<GameWorld> & game_world){

 int x, y;
 SDL_GetRelativeMouseState(&x, &y); 
 game_world->setCamera(x/1000.0, y/1000.0);

 if(keystates[SDL_SCANCODE_W]){
   game_world->moveF();
 }
 if(keystates[SDL_SCANCODE_S]){
   game_world->moveB();
 }
 if(keystates[SDL_SCANCODE_A]){
   game_world->moveL();
 }
 if(keystates[SDL_SCANCODE_D]){
   game_world->moveR();
 }
 if(keystates[SDL_SCANCODE_SPACE]){
   game_world->moveU();
 }
 if(keystates[SDL_SCANCODE_LCTRL]){
   game_world->moveD();
 }
 if(keystates[SDL_SCANCODE_ESCAPE]){
  SDL_Quit();
 }

}
	

void Draw(const std::shared_ptr<SDL_Window> &window, const std::shared_ptr<GameWorld> &game_world) {

  
  glClearColor(0.4f, 0.4f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  game_world->Draw();
   
// Don't forget to swap the buffers
  SDL_GL_SwapWindow(window.get());
}

std::shared_ptr<SDL_Window> InitWorld() {
  Uint32 width = 1280;
  Uint32 height = 720;
  SDL_Window * _window;
  std::shared_ptr<SDL_Window> window;

  // Glew will later ensure that OpenGL 3 *is* supported
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // Do double buffering in GL
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Initialise SDL - when using C/C++ it's common to have to
  // initialise libraries by calling a function within them.
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)<0) {
    std::cout << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
    return nullptr;
  }

  // When we close a window quit the SDL application
  atexit(SDL_Quit);

  // Create a new window with an OpenGL surface
  _window = SDL_CreateWindow("Shader Example"
                             , SDL_WINDOWPOS_CENTERED
                             , SDL_WINDOWPOS_CENTERED
                             , width
                             , height
                             , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!_window) {
    std::cout << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    return nullptr;
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(_window);
  if (!glContext) {
    std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
    return nullptr;
  }

  // Initialise GLEW - an easy way to ensure OpenGl 3.0+
  // The *must* be done after we have set the video mode - otherwise we have no
  // OpenGL context to test.
  glewInit();
  if (!glewIsSupported("GL_VERSION_3_0")) {
    std::cerr << "OpenGL 3.0 not available" << std::endl;
    return nullptr;
  }

  // OpenGL settings
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  window.reset(_window, SDL_DestroyWindow);
  return window;
}

ApplicationMode ParseOptions (int argc, char ** argv) {
  namespace po = boost::program_options;

  po::options_description desc("Allowed options");
  desc.add_options()
     ("help", "print this help message")
     ("translate", "Show translation example (default)")
     ("rotate", "Show rotation example")
     ("scale", "Show scale example")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << std::endl;
    exit(0);
  }

  if(vm.count("rotate")) {
    return ROTATE;
  }

  if(vm.count("scale")) {
    return SCALE;
  }

  // The default
  return TRANSFORM;
}

int main(int argc, char ** argv) {
  Uint32 delay = 1000/60; // in milliseconds

  auto mode = ParseOptions(argc, argv);
  auto window = InitWorld();
  auto game_world = std::make_shared<GameWorld>(mode);


  glEnable(GL_DEPTH_TEST);
  //basically hide and lock mouse to center
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if(!window) {
    SDL_Quit();
  }

  keystates = SDL_GetKeyboardState(NULL);
  // Call the function "tick" every delay milliseconds
  SDL_AddTimer(delay, tick, NULL);

  // Add the main event loop
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      SDL_Quit();
      break;
    case SDL_USEREVENT:
      Draw(window, game_world);
      Update(game_world);
      break;
    default:
      break;
    }
  }
}
