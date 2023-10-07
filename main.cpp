#include <SFML/Graphics.hpp>

const int WIDTH = 2560;
const int HEIGHT = 1440;

int mandelbrot(double x0, double y0, int max_iteration) {
	double x = 0.0;
	double y = 0.0;
	int iteration = 0;
	while (x * x + y * y <= 2 * 2 && iteration < max_iteration) {
		double xtemp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = xtemp;
		iteration++;
	}
	return iteration;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");

	sf::Image image;
	image.create(WIDTH, HEIGHT, sf::Color::Black);

	for (int Px = 0; Px < WIDTH; Px++) {
		for (int Py = 0; Py < HEIGHT; Py++) {
			double x0 = (double)Px / WIDTH * (0.47 + 2.00) - 2.00;
			double y0 = (double)Py / HEIGHT * (1.12 + 1.12) - 1.12;

			int value = mandelbrot(x0, y0, 6000);

			// This part colors the pixel. Adjusts this for your needs:

			
			sf::Color color(value % 255, (value * 2) % 255, (value * 3) % 255);
			if (value == 6000) {
				color = sf::Color::Black; 
			}
			image.setPixel(Px, Py, color);
		}
	}

	sf::Texture texture;
	texture.loadFromImage(image);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}

