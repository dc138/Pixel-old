/*

	Simple demo file that showcases the use of
	basic rendering and event polling rutines
	to draw random lines onto the screen.

*/

#include <pixel.hpp>
using namespace pixel;

class RandomLines: public Application {

public:
	inline bool OnUpdate(float et) override {
		
		for(uint8_t i = 0; i < 50; i++) {
			DrawLine(vu2d(rand() % pScreenSize.x, rand() % pScreenSize.y),
					 vu2d(rand() % pScreenSize.x, rand() % pScreenSize.y),
					 RandPixel());
		}

		if(KeyboardKey(Key::ESCAPE).pressed) {
			Close();
		}

		return true;
	}
};

int main() {
	RandomLines application;
	application.Launch(vu2d(500, 500), 2, vu2d(500, 5), "Random lines", DrawingMode::FULL_ALPHA);
	
	return 0;
}