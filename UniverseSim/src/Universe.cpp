#include "Universe.h"
#include <cmath>

void Universe::update(float dt)
{
    int n = bodies.size();

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            Body& a = bodies[i];
            Body& b = bodies[j];

            float dx = b.x - a.x;
            float dy = b.y - a.y;

            float dist2 = dx*dx + dy*dy + 0.01f;
            float dist = sqrt(dist2);

            float force =
                G * a.mass * b.mass / dist2;

            float fx = force * dx / dist;
            float fy = force * dy / dist;

            a.vx += fx / a.mass * dt;
            a.vy += fy / a.mass * dt;

            b.vx -= fx / b.mass * dt;
            b.vy -= fy / b.mass * dt;
        }
    }

    for (auto& b : bodies)
    {
        b.x += b.vx * dt;
        b.y += b.vy * dt;
    }
}