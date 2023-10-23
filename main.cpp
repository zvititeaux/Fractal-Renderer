#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include <iostream>

const int WIDTH = 2560;
const int HEIGHT = 1440;

class Mandelbrot {
public:
    // The Mandelbrot calculation function has been commented out, but essentially 
    // it calculates the number of iterations required for a given point (x0, y0) 
    // to determine whether it is inside the Mandelbrot set or not.

    // static int calculate( ... ) {...}
};

class Viewport {
private:
    double xMin, xMax, yMin, yMax;  // Boundary values for the current view.

public:
    // Default constructor initializes the boundaries to show a typical section of the Mandelbrot set.
    Viewport() : xMin(-2.0), xMax(0.47), yMin(-1.12), yMax(1.12) {}

    // Adjusts the viewport for zooming centered on the current view.
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

    // Adjusts the viewport by panning in the specified directions.
    void pan(double deltaX, double deltaY) {
        double xRange = xMax - xMin;
        double yRange = yMax - yMin;

        xMin += xRange * deltaX;
        xMax += xRange * deltaX;
        yMin += yRange * deltaY;
        yMax += yRange * deltaY;
    }

    // Getter for the center point of the current viewport.
    sf::Vector2f getCenter() const {
        return sf::Vector2f((getXMax() + getXMin()) / 2.0f, (getYMax() + getYMin()) / 2.0f);
    }

    // Getter for the current zoom level.
    float getZoom() const {
        return (getXMax() - getXMin()) / 2.0f;
    }

    // Various getters for the boundary values.
    double getXMin() const { return xMin; }
    double getXMax() const { return xMax; }
    double getYMin() const { return yMin; }
    double getYMax() const { return yMax; }
};

class App {
private:
    sf::RenderWindow window;  // Window object for SFML rendering.
    Viewport viewport;        // The current viewport object.
    sf::Shader mandelbrotShader;  // Shader object for rendering the Mandelbrot set.
    bool needsUpdate = true;  // Flag to determine whether the current view needs to be re-rendered.

public:
    // Constructor sets up the window and the shader.
    App() : window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set"), needsUpdate(true) {
        if (!mandelbrotShader.loadFromFile("C:\\Fractal Renderer\\Fractal Renderer\\mandelbrot.frag", sf::Shader::Fragment)) {
            std::cerr << "Failed to load shader." << std::endl;
            exit(-1);
        }
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(144);
    }

    // Get the aspect ratio of the current window size.
    float getAspect() const {
        return static_cast<float>(WIDTH) / HEIGHT;
    }

    // Main loop of the application.
    void run() {
        while (window.isOpen()) {
            handleEvents();  // Process any user input or other events.
            if (needsUpdate) {
                renderMandelbrot();  // If needed, re-render the Mandelbrot set.
                window.display();
                needsUpdate = false;
            }
        }
    }

private:
    // Handle all events, such as user input.
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::W:
                    viewport.zoomCenter(0.8);
                    needsUpdate = true;
                    break;
                case sf::Keyboard::S:
                    viewport.zoomCenter(1.25);
                    needsUpdate = true;
                    break;
                    // ... (Other key events)
                default:
                    break;
                }
            }
        }
    }

    // Render the Mandelbrot set using the shader.
    void renderMandelbrot() {
        // Setting various shader uniforms.
        mandelbrotShader.setUniform("ColorScale", sf::Vector3f(4.0f, 5.0f, 6.0f));
        // ... (Other uniforms)

        // Create a full-screen quad and render the Mandelbrot set on it.
        sf::RectangleShape fullscreenQuad(sf::Vector2f(WIDTH, HEIGHT));
        window.clear(sf::Color::Red);  // Clearing the window.
        window.draw(fullscreenQuad, &mandelbrotShader);
    }
};

int main() {
    App app;  // Create the application object.
    app.run();  // Run the main application loop.
    return 0;
}
