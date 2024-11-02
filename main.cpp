#include "screen.h"
#include <numeric>
#include <cmath>
#include <vector>

struct vec3 {
    float x, y, z;
};

struct connection {
    int a, b;
};

// Adding the rotation function so you can make the points rotate
void rotate(vec3& point, float rx = 0, float ry = 0, float rz = 0) {
    // Rotation around the x-axis
    float rad = rx;
    float tempY = point.y;
    point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
    point.z = std::sin(rad) * tempY + std::cos(rad) * point.z;

    // Rotation around the y-axis
    rad = ry;
    float tempX = point.x;
    point.x = std::cos(rad) * point.x + std::sin(rad) * point.z;
    point.z = -std::sin(rad) * tempX + std::cos(rad) * point.z;

    // Rotation around the z-axis
    rad = rz;
    tempX = point.x;
    tempY = point.y;
    point.x = std::cos(rad) * tempX - std::sin(rad) * tempY;
    point.y = std::sin(rad) * tempX + std::cos(rad) * tempY;
}

void line(Screen& screen, float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    // Handle horizontal and vertical line drawing
    float length = std::sqrt(dx * dx + dy * dy);
    if (length == 0) return; // Avoid division by zero
    
    float angle = std::atan2(dy, dx);

    for (float i = 0; i <= length; i++) {
        screen.pixel(x1 + std::cos(angle) * i, y1 + std::sin(angle) * i);
    }
}

int main() {
    Screen screen;
    
    // Drawing the points
    std::vector<vec3> points {
        {100, 100, 100},
        {200, 100, 100},
        {200, 200, 100},
        {100, 200, 100},
        {100, 100, 200},
        {200, 100, 200},
        {200, 200, 200},
        {100, 200, 200}
    };
    
    // Creating the connection between the points
    std::vector<connection> connections {
        {0, 4}, {1, 5}, {2, 6}, {3, 7},
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
    };
    
    // Centering the points inside the window
    vec3 c{0, 0, 0};
    for (const auto& p : points) {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }
    c.x /= points.size();
    c.y /= points.size();
    c.z /= points.size();

    while (true) {
        for (auto& p : points) {
            p.x -= c.x;
            p.y -= c.y;
            p.z -= c.z;
            rotate(p, 0.02, 0.01, 0.04);
            p.x += c.x;
            p.y += c.y;
            p.z += c.z;
            screen.pixel(p.x, p.y);
        }

        for (const auto& conn : connections) {
            line(screen, points[conn.a].x, points[conn.a].y,
                 points[conn.b].x, points[conn.b].y);
        }
        
        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}
