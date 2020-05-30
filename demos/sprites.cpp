/*

	Simple demo file that showcases the use of
	basic rendering and event polling rutines
	to render sprites loaded from a .png.

*/

#include <pixel.hpp>
using namespace pixel;

int main() {
	Window window(vu2d(500, 500), 2, vu2d(500, 5), "Sprites", DrawingMode::FULL_ALPHA);
	Sprite sprite("D:\\dev\\cpp\\pixel\\demos\\sprites.png");

	float r = 100, g = 200, b = 0;

	while(window.ShouldExist()) {
		window.Clear();

		window.Draw(window.MousePos(), White);
		window.DrawSprite(vf2d(0, 0), &sprite, vf2d(0.25f, 0.25f), Pixel(r, g, b, 255));

		if(window.KeyboardKey(Key::ESCAPE).pressed) {
			window.Close();
		}

		r += 0.03;
		b += 0.02;
		g += 0.06;

		if(r >= 255.0f) r = 0;
		if(g >= 255.0f) g = 0;
		if(b >= 255.0f) b = 0;

		window.Update();
	}

	return 0;
}