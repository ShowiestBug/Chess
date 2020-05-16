#include <cstdlib>
#include <iostream>
#include "Screen.h"

using namespace std;
int main(int argc, char *argv[]) {
    Screen screen;
    if (screen.init() == false) {
        cout << "SDL initialization failed." << endl;
    }
    bool flag = false;
    SDL_Event event;
    while (!flag) {     
    
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                flag = true;
            }
            else if(event.type == SDL_MOUSEMOTION)
            {
                int x = event.motion.x;
                int y = event.motion.y;
            }
        }
    }
    screen.close();
    return 0;
}
