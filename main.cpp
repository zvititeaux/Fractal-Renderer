#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

const int WIDTH = 3840;
const int HEIGHT = 2160;

class Mandelbrot {
public:
	static int calculate(double x0, double y0, int max_iteration) {
		double x = 0.0;
		double y = 0.0;
		int iteration = 0;
		double xx = x * x; // Cached squared value
		double yy = y * y; // Cached squared value
		
		while (xx + yy <= 4.0 && iteration < max_iteration) {
			y = 2 * x * y + y0;
			x = xx - yy + x0;
			xx = x * x;
			yy = y * y;
			iteration++;
		}
		return iteration;
	}
};

class Viewport {
private:
	double xMin, xMax, yMin, yMax;

public:
	Viewport() : xMin(-2.0), xMax(0.47), yMin(-1.12), yMax(1.12) {}

	void zoom(int mouseX, int mouseY, double zoomFactor) {
		double x = xMin + mouseX * (xMax - xMin) / WIDTH;
		double y = yMin + mouseY * (yMax - yMin) / HEIGHT;

		double xRange = (xMax - xMin) * zoomFactor;
		double yRange = (yMax - yMin) * zoomFactor;

		xMin = x - xRange / 2.0;
		xMax = x + xRange / 2.0;
		yMin = y - yRange / 2.0;
		yMax = y + yRange / 2.0;
	}

	double getXMin() const { return xMin; }
	double getXMax() const { return xMax; }
	double getYMin() const { return yMin; }
	double getYMax() const { return yMax; }
};

class App {
private: 
	sf::RenderWindow window;
	Viewport viewport;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	App() : window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set") {}

	void run() {
		while (window.isOpen()) {
			handleEvents();

			renderMandelbrot();
			
			texture.loadFromImage(image);
			sprite.setTexture(texture, true);
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}

private:
	void handleEvents() {
	sf::Event event;
	while(window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::MouseWheelScrolled) {
			if (event.mouseWheelScroll.delta > 0) {
				viewport.zoom(event.mouseWheelScroll.x, event.mouseWheelScroll.y, 0.8);
			} else {
				viewport.zoom(event.mouseWheelScroll.x, event.mouseWheelScroll.y, 1.25);
				}
			}
		}
	}

	void renderMandelbrot() {
		double scaleX = (viewport.getXMax() - viewport.getXMin()) / WIDTH;
		double scaleY = (viewport.getYMax() - viewport.getYMin()) / HEIGHT;

		image.create(WIDTH, HEIGHT);

		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				double x0 = viewport.getXMin() + x * scaleX;
				double y0 = viewport.getYMin() + y * scaleY;
				
				int value = Mandelbrot::calculate(x0, y0, 1000);
				sf::Color color(value % 255, (value * 2) % 255, (value * 3) % 255);
				if (value == 1000) color = sf::Color::Black;
				image.setPixel(x, y, color);
			}
		}
	}
};

int main() {
	App app;
	app.run();
	return 0; 
}

