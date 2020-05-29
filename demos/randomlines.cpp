/*

	Simple demo file that showcases the use of
	basic rendering and event polling rutines
	to draw random lines onto the screen.

*/

#include <pixel.hpp>
using namespace pixel;

int main() {
	Window window(vu2d(500, 500), 2, vu2d(500, 5), "Random lines", DrawingMode::FULL_ALPHA);

	while(window.ShouldExist()) {
		for(uint32_t i = 0; i < 50; i++) {
			window.DrawLine(vu2d(rand() % window.ScreenSize().x, rand() % window.ScreenSize().y),
							vu2d(rand() % window.ScreenSize().x, rand() % window.ScreenSize().y),
							RandPixel());
		}

		if(window.KeyboardKey(Key::ESCAPE).pressed) {
			window.Close();
		}

		window.Update();
	}

	return 0;
}