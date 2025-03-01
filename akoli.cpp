// Phillip Lakes (Koli)
// CS 3350 Software Engineering
//
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

void calculate_framerate() {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = current_time - last_time;
    last_time = current_time;
    float fps = 1.0f / elapsed.count();
    cout << "FPS: " << fps << endl;
}

int main(int argc char *argv[] ) {

  return 0;
}
