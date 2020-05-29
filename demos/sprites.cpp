/*

	Simple demo file that showcases the use of
	basic rendering and event polling rutines
	to render sprites loaded from a .bmp.

*/

#include <pixel.hpp>
using namespace pixel;

int main() {
	Window window(vu2d(500, 500), 2, vu2d(500, 5), "Sprites", DrawingMode::FULL_ALPHA);
	Sprite sprite("D:\\dev\\cpp\\pixel\\demos\\sprites.bmp");

	while(window.ShouldExist()) {
		window.Clear();



		if(window.KeyboardKey(Key::ESCAPE).pressed) {
			window.Close();
		}

		window.Update();
	}

	return 0;
}