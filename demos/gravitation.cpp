/*

	Simple demo file that showcases the use of
	basic rendering rutines with a gravitational
	simulation of two bodies.

*/

#pragma warning(disable:4244)

#include <pixel.hpp>
using namespace pixel;

int main() {
	Application window(vu2d(500, 500), 2, vu2d(500, 5), "Gravitation", DrawingMode::NO_ALPHA);

	float px1 = 250, py1 = 250;
	float sx1 = 0, sy1 = 0;
	float ax1 = 0, ay1 = 0;
	float fx1 = 0, fy1 = 0;
	float m1 = 10000, d1 = 0.005f;

	float px2 = 350, py2 = 250;
	float sx2 = 0, sy2 = -30;
	float ax2 = 0, ay2 = 0;
	float fx2 = 0, fy2 = 0;
	float m2 = 100, d2 = 0.05f;

	float g = 10, s = 2;

	float dx = 0, dy = 0;
	float r = 0, f = 0, t = 0;

	while(window.ShouldExist()) {

		window.Clear();

		dx = px1 - px2;
		dy = py1 - py2;

		r = sqrt((dx * dx) + (dy * dy));
		f = (g * m1 * m2) / (r * r);
		t = atan(dy / dx);

		fx1 = f * cos(t) * (px2 >= px1 ? 1 : -1);
		fy1 = f * sin(t) * (px2 >= px1 ? 1 : -1);
		fx2 = f * cos(t) * (px2 >= px1 ? -1 : 1);
		fy2 = f * sin(t) * (px2 >= px1 ? -1 : 1);

		ax1 = fx1 / m1;
		ay1 = fy1 / m1;
		ax2 = fx2 / m2;
		ay2 = fy2 / m2;

		sx1 += ax1 * window.ElapsedTime() * s;
		sy1 += ay1 * window.ElapsedTime() * s;
		sx2 += ax2 * window.ElapsedTime() * s;
		sy2 += ay2 * window.ElapsedTime() * s;

		px1 += sx1 * window.ElapsedTime() * s;
		py1 += sy1 * window.ElapsedTime() * s;
		px2 += sx2 * window.ElapsedTime() * s;
		py2 += sy2 * window.ElapsedTime() * s;

		window.FillCircle(vu2d(px1, py1), m1 * d1, White);
		window.FillCircle(vu2d(px2, py2), m2 * d2, White);

		window.DrawLine(vu2d(px1, py1), vu2d(px1 + sx1, py1 + sy1), Red);
		window.DrawLine(vu2d(px2, py2), vu2d(px2 + sx2, py2 + sy2), Red);

		window.DrawLine(vu2d(px1, py1), vu2d(px1 + ax1, py1 + ay1), Blue);
		window.DrawLine(vu2d(px2, py2), vu2d(px2 + ax2, py2 + ay2), Blue);

		if(window.KeyboardKey(Key::ESCAPE).pressed) {
			window.Close();
		}

		window.Update();
	}

	return 0;
}
