#version 330 core

// Uniforms that you set in your C++ code
uniform float viewportXMin;
uniform float viewportXMax;
uniform float viewportYMin;
uniform float viewportYMax;
uniform float width;
uniform float height;
uniform int maxIterations;
uniform vec3 ColorScale;

out vec4 color;

// Function to map the current pixel to a point in the Mandelbrot set
vec2 mapToMandelbrot(float x, float y) {
    float xMapped = mix(viewportXMin, viewportXMax, x / width);
    float yMapped = mix(viewportYMin, viewportYMax, y / height);
    return vec2(xMapped, yMapped);
}

void main() {
    // Get the coordinates of the current pixel
    vec2 c = mapToMandelbrot(gl_FragCoord.x, gl_FragCoord.y);
    vec2 z = vec2(0.0, 0.0);
    int iterations = 0;

    // Mandelbrot iteration loop
    for (int i = 0; i < maxIterations; ++i) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (2.0 * z.x * z.y) + c.y;

        z = vec2(x, y);

        if (length(z) > 2.0) {
            break;
        }

        iterations++;
    }

    // Color mapping based on the number of iterations
    if (iterations == maxIterations) {
        color = vec4(0.0, 0.0, 0.0, 1.0); // Black for pixels inside the Mandelbrot set
    } else {
        // Color mapping for pixels outside the Mandelbrot set
        float norm = float(iterations) / float(maxIterations);
        vec3 gradientColor = mix(vec3(0.0, 0.0, 0.8), vec3(1.0, 1.0, 0.0), norm); // Blue to Yellow
        vec3 finalColor = ColorScale * gradientColor;
        color = vec4(finalColor, 1.0);
    }
}