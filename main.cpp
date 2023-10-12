#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

class Mandelbrot {
public:
	/*static int calculate(double x0, double y0, int max_iteration) {
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

		if (iteration == max_iteration) return max_iteration;

		return iteration + 1 - log(log(sqrt(xx + yy))) / log(2.0);
	} */
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

		xMin = x - mouseX * xRange / WIDTH;
		xMax = xMin + xRange;
	
		yMin = y - mouseY * yRange / HEIGHT;
		yMax = yMin + yRange;
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
	sf::Shader mandelbrotShader;
	bool needsUpdate = true;

public:
	App() : window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set"), needsUpdate(true) {
		if (!mandelbrotShader.loadFromFile("C:\\Fractal Renderer\\Fractal Renderer\\mandelbrot.frag", sf::Shader::Fragment)) {
			std::cerr << "Failed to load shader." << std::endl;
			exit(-1);
		}
		window.setVerticalSyncEnabled(true);
		window.setFramerateLimit(144);
	}

	void run() {
		while (window.isOpen()) {
			handleEvents();
			renderMandelbrot();
			window.display();
			needsUpdate = false;
		}
	}

private:
	void handleEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				double zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.8 : 1.25;
				// Update the Mandelbrot calculations with new zoom factor and set needsUpdate to true
				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				viewport.zoom(mouseX, mouseY, zoomFactor);
				needsUpdate = true;
			}
		}
	}

	void renderMandelbrot() {
		mandelbrotShader.setUniform("viewportXMin", static_cast<float>(viewport.getXMin()));
		mandelbrotShader.setUniform("viewportXMax", static_cast<float>(viewport.getXMax()));
		mandelbrotShader.setUniform("viewportYMin", static_cast<float>(viewport.getYMin()));
		mandelbrotShader.setUniform("viewportYMax", static_cast<float>(viewport.getYMax()));
		mandelbrotShader.setUniform("width", static_cast<float>(WIDTH));
		mandelbrotShader.setUniform("height", static_cast<float>(HEIGHT));
		mandelbrotShader.setUniform("maxIterations", 1000);

		sf::RectangleShape fullscreenQuad(sf::Vector2f(WIDTH, HEIGHT));
		window.clear();
		window.draw(fullscreenQuad, &mandelbrotShader);
	}
};

int main() {
	App app;
	app.run();
	return 0;
}

