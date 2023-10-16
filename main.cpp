#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include <iostream>

const int WIDTH = 2560;
const int HEIGHT = 1440;

class Mandelbrot {
public:
	static sf::Color calculate(double x0, double y0, int max_iteration) {
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

		if (iteration == max_iteration) {
			return sf::Color::Black;
		}
		else {
			float normalized = float(iteration) / float(max_iteration);
			return sf::Color(normalized * 255, 0, (1.0f - normalized) * 255);
		}
	}
};

	class Viewport {
	private:
		double xMin, xMax, yMin, yMax;
		double zoomThreshold = 0.0001;

	public:
		Viewport() : xMin(-2.0), xMax(0.47), yMin(-1.12), yMax(1.12) {}

		void zoom(int mouseX, int mouseY, double zoomFactor) {
			double centerX = (xMax + xMin) * 0.5;
			double centerY = (yMax + yMin) * 0.5;

			double xRange = (xMax - xMin) * zoomFactor;
			double yRange = (yMax - yMin) * zoomFactor;

			xMin = centerX - 0.5 * xRange;
			xMax = centerX + 0.5 * xRange;
			yMin = centerY - 0.5 * yRange;
			yMax = centerY + 0.5 * yRange;
		}

		void zoomCenter(double zoomFactor) {
			double centerX = (xMax + xMin) * 0.5;
			double centerY = (yMax + yMin) * 0.5;

			double xRange = (xMax - xMin) * zoomFactor;
			double yRange = (yMax - yMin) * zoomFactor;

			xMin = centerX - 0.5 * xRange;
			xMax = centerX + 0.5 * xRange;
			yMin = centerY - 0.5 * yRange;
			yMax = centerY + 0.5 * yRange;
		}

		void pan(double deltaX, double deltaY) {
			double xRange = xMax - xMin;
			double yRange = yMax - yMin;

			xMin += xRange * deltaX;
			xMax += xRange * deltaX;
			yMin += yRange * deltaY;
			yMax += yRange * deltaY;
		}

		bool isBeyondThreshold() const {
			return (xMax - xMin) < zoomThreshold;
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
		const int maxIterations = 1000;

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
				else if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::W:
						// Zoom in
						viewport.zoomCenter(0.8);
						needsUpdate = true;
						break;
					case sf::Keyboard::S:
						// Zoom out
						viewport.zoomCenter(1.25);
						needsUpdate = true;
						break;
					case sf::Keyboard::Up:
						// Pan up
						viewport.pan(0, -0.05);
						needsUpdate = true;
						break;
					case sf::Keyboard::Down:
						// Pan down
						viewport.pan(0, 0.05);
						needsUpdate = true;
						break;
					case sf::Keyboard::Left:
						// Pan left
						viewport.pan(-0.05, 0);
						needsUpdate = true;
						break;
					case sf::Keyboard::Right:
						// Pan right
						viewport.pan(0.05, 0);
						needsUpdate = true;
						break;
					default:
						break;
					}
				}
			}
		}

		void renderMandelbrot() {
			if (viewport.isBeyondThreshold()) {
				sf::Image image;
				image.create(WIDTH, HEIGHT, sf::Color::Black);
				for (int x = 0; x < WIDTH; ++x) {
					for (int y = 0; y < HEIGHT; ++y) {
						double x0 = viewport.getXMin() + (x / static_cast<double>(WIDTH)) * (viewport.getXMax() - viewport.getXMin());
						double y0 = viewport.getYMin() + (x / static_cast<double>(HEIGHT)) * (viewport.getYMax() - viewport.getYMin());
						image.setPixel(x, y, Mandelbrot::calculate(x0, y0, maxIterations));
					}
				}
				sf::Texture texture;
				texture.loadFromImage(image);
				sf::Sprite sprite(texture);
				window.clear();
				window.draw(sprite);
			}
			else {
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
		}
	};

	int main() {
		App app;
		app.run();
		return 0;
	}

