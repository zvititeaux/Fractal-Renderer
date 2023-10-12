#version 330 core

out vec4 color;

uniform float viewportXMin;
uniform float viewportXMax;
uniform float viewportYMin;
uniform float viewportYMax;
uniform float width;
uniform float height;
uniform int maxIterations;

vec3 getColor(int iteration) {
    if (iteration == maxIterations) {
        return vec3(0.0, 0.0, 0.0); // black color for points inside the Mandelbrot set
    } else {
        float normalized = float(iteration) / float(maxIterations);
        return vec3(normalized, 0.0, 1.0 - normalized); // Simple gradient (blue to red)
    }
}

void main() {
    float x0 = viewportXMin + (gl_FragCoord.x / width) * (viewportXMax - viewportXMin);
    float y0 = viewportYMin + (gl_FragCoord.y / height) * (viewportYMax - viewportYMin);

    float x = 0.0;
    float y = 0.0;
    int iteration = 0;

    while (x*x + y*y <= 4.0 && iteration < maxIterations) {
        float tempX = x*x - y*y + x0;
        y = 2.0 * x * y + y0;
        x = tempX;
        iteration++;
    }

    vec3 pixelColor = getColor(iteration);
    color = vec4(pixelColor, 1.0);
}