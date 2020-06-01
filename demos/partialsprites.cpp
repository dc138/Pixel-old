/*

	Simple demo file that showcases the use of
	basic rendering and event polling rutines
	to render parts od a sprites loaded from a 
	.png file.

*/

#include <pixel.hpp>
using namespace pixel;

int main() {
	Window window(vu2d(500, 500), 2, vu2d(500, 5), "Sprites", DrawingMode::FULL_ALPHA);
	
	Sprite sprite("D:\\dev\\cpp\\pixel\\demos\\partialsprites.jpg");

	while(window.ShouldExist()) {
		window.Clear();

		window.DrawPartialSprite(vf2d(0.0f, 0.0f), vf2d(100.0f, 100.0f), vf2d(100.0f, 100.0f), &sprite);

		if(window.KeyboardKey(Key::ESCAPE).pressed) {
			window.Close();
		}

		window.Update();
	}

	return 0;
}