#pragma once

#define G 6.6743 * pow(10, -11)


void planetary_bodies_init(std::vector<Planet>& planets)
{
	Planet DidymosA(3500.0f,
		500.0f,
		{ 0 },
		{ 0 },
		DARKGRAY);

	planets.push_back(DidymosA);

	Planet DidymosB(3000.0f,
		80.0f,
		{ 0.0f, -2670.0f },
		{ 0.21f, 0.0f },
		DARKGRAY);

	planets.push_back(DidymosB);
}

void gravitational_attraction(std::vector<Planet>& planets, size_t size, Ship& ship)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = i; j < size; j++)
		{
			if (i != j && (planets[i].position.x != planets[j].position.x || planets[i].position.y != planets[j].position.y))
			{
				float distance_sqr = Vector2DistanceSqr(planets[i].position, planets[j].position);

				float gravitational_force = planets[i].mass * planets[j].mass * G / distance_sqr;

				float distance = sqrtf(distance_sqr);

				float gravitational_force_x = gravitational_force * ((planets[j].position.x - planets[i].position.x) / distance);
				float gravitational_force_y = gravitational_force * ((planets[j].position.y - planets[i].position.y) / distance);

				planets[i].force.x += gravitational_force_x;
				planets[i].force.y += gravitational_force_y;

				planets[j].force.x -= gravitational_force_x;
				planets[j].force.y -= gravitational_force_y;
			}
		}
	}

	for (size_t i = 0; i < size; i++)
	{
		if (planets[i].position.x != ship.position.x || planets[i].position.y != ship.position.y)
		{
			float distance_sqr = Vector2DistanceSqr(planets[i].position, ship.position);

			float gravitational_force = ship.mass * planets[i].mass * G / distance_sqr;

			float distance = sqrtf(distance_sqr);

			float gravitational_force_x = gravitational_force * ((ship.position.x - planets[i].position.x) / distance);
			float gravitational_force_y = gravitational_force * ((ship.position.y - planets[i].position.y) / distance);

			ship.force.x -= gravitational_force_x;
			ship.force.y -= gravitational_force_y;
		}
	}
}