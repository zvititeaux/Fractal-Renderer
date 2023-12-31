#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <vector>
#include <iostream>



const int WIDTH = 2560;
const int HEIGHT = 1440;

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
	/*
		void zoom(double zoomFactor) {
			double centerX = (xMax + xMin) * 0.5;
			double centerY = (yMax + yMin) * 0.5;

			double xRange = (xMax - xMin) * zoomFactor;
			double yRange = (yMax - yMin) * zoomFactor;

			xMin = centerX - 0.5 * xRange;
			xMax = centerX + 0.5 * xRange;
			yMin = centerY - 0.5 * yRange;
			yMax = centerY + 0.5 * yRange;
		}
		*/
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

	sf::Vector2f getCenter() const {
		return sf::Vector2f((getXMax() + getXMin()) / 2.0f, (getYMax() + getYMin()) / 2.0f);
	}

	float getZoom() const {
		return (getXMax() - getXMin()) / 2.0f;
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

	sf::Vector3f colorScale{ 1.0f, 1.0f, 1.0f };
	int maxIterations{500};

public:
	App() : window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set"), needsUpdate(true) {
		if (!mandelbrotShader.loadFromFile("C:\\Fractal Renderer\\Fractal Renderer\\mandelbrot.frag", sf::Shader::Fragment)) {
			std::cerr << "Failed to load shader." << std::endl;
			exit(-1);
		}
		window.setVerticalSyncEnabled(true);
		window.setFramerateLimit(144);
	}

	float getAspect() const {
		return static_cast<float>(WIDTH) / HEIGHT;
	}

	void run() {
		ImGui::SFML::Init(window);
		sf::Clock deltaClock;

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				ImGui::SFML::ProcessEvent(event);
				handleEvents(event);
				/*if (event.type == sf::Event::Closed) {
					window.close();
				}*/
			}

			// Begin ImGui frame
			ImGui::SFML::Update(window, deltaClock.restart());

			// ImGui interface goes here
			ImGui::Begin("Control Panel");
			// ... your ImGui widgets ...
			if (ImGui::SliderInt("Max Iterations", &maxIterations, 100, 100000)) {
				needsUpdate = true;
			}

			if (ImGui::ColorEdit3("Color Scale", reinterpret_cast<float*>(&colorScale))) {
				needsUpdate = true;
			}

			if 

			ImGui::End();

			// Render the ImGui draw lists
			ImGui::Render();

			// The rest of your rendering code
			window.clear();
			renderMandelbrot();
			ImGui::SFML::Render(window);
			window.display();
		}

		// Shutdown ImGui SFML when the window is closed
		ImGui::SFML::Shutdown();
	}

private:
	void handleEvents(const sf::Event& event) {
			ImGui::SFML::ProcessEvent(event);
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


	void renderMandelbrot() {


		mandelbrotShader.setUniform("viewportXMin", static_cast<float>(viewport.getXMin()));
		mandelbrotShader.setUniform("viewportXMax", static_cast<float>(viewport.getXMax()));
		mandelbrotShader.setUniform("viewportYMin", static_cast<float>(viewport.getYMin()));
		mandelbrotShader.setUniform("viewportYMax", static_cast<float>(viewport.getYMax()));
		mandelbrotShader.setUniform("width", static_cast<float>(WIDTH));
		mandelbrotShader.setUniform("height", static_cast<float>(HEIGHT));
		mandelbrotShader.setUniform("maxIterations", maxIterations);
		mandelbrotShader.setUniform("colorScale", sf::Glsl::Vec3(colorScale.x, colorScale.y, colorScale.z));

		sf::RectangleShape fullscreenQuad(sf::Vector2f(WIDTH, HEIGHT));
		window.clear(sf::Color::Red);
		window.draw(fullscreenQuad, &mandelbrotShader);
	}
};

int main() {
	App app;
	app.run();
	return 0;
}