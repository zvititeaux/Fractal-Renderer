#version 330 core

// Uniforms that you set in your C++ code
// Uniforms that you set in your C++ code
uniform float viewportXMin;
uniform float viewportXMax;
uniform float viewportYMin;
uniform float viewportYMax;
uniform float width;
uniform float height;
uniform int maxIterations;
uniform vec3 colorScale;

out vec4 color;

// Function to map the current pixel to a point in the Mandelbrot set
vec2 mapToMandelbrot(float x, float y) {
    float xMapped = mix(viewportXMin, viewportXMax, x / width);
    float yMapped = mix(viewportYMin, viewportYMax, y / height);
    return vec2(xMapped, yMapped);
}

vec3 getGradientColor(float norm) {
    vec3 gradientColor;
    if (norm < 0.25) {
        gradientColor = mix(vec3(0.5, 0.0, 0.1), vec3(1.0, 0.8, 0.0), norm * 4.0);
    } else if (norm < 0.5) {
        gradientColor = mix(vec3(1.0, 0.8, 0.0), vec3(1.0, 0.5, 0.0), (norm - 0.25) * 4.0);
    } else if (norm < 0.75) {
        gradientColor = mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 0.4, 0.4), (norm - 0.5) * 4.0);
    } else {
        gradientColor = mix(vec3(1.0, 0.4, 0.4), vec3(0.5, 0.0, 0.1), (norm - 0.75) * 4.0);
    }
    gradientColor *= colorScale; 
    
    return gradientColor;
}

void main() {
    // Get the coordinates of the current pixel
    vec2 c = mapToMandelbrot(gl_FragCoord.x, gl_FragCoord.y);
    vec2 z = vec2(0.0, 0.0);
    int iterations = 0;
    float minDistance = 1000.0;

    // Mandelbrot iteration loop
    for (int i = 0; i < maxIterations; ++i) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (2.0 * z.x * z.y) + c.y;

        z = vec2(x, y);

        float dist = length(z);
        if (dist > 2.0) {
            break;
        }
        
        // Update the minimum distance for orbit trapping
        minDistance = min(minDistance, dist);

        iterations++;
    }

    if (iterations == maxIterations) {
        color = vec4(0.0, 0.0, 0.0, 1.0); // Green for pixels inside the Mandelbrot set
    } else {
        float norm = (float(iterations) + 1.0 - log(log(minDistance + 2.0))) / float(maxIterations);
        vec3 gradientColor = getGradientColor(norm);
        color = vec4(gradientColor, 1.0); 
    }
}