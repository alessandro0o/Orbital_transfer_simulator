#pragma once

class Object
{
protected:

	Mesh mesh	= { 0 };
	Model model	= { 0 };
	Color color	= { 0 };	// RGBA

public:

	float radius = 0;	// m
	float mass = 0;		// Kg
	float inertia = 0;	// Kg*m^2

	float torque = 0;					// N * m
	float anglular_acceleration = 0;	// rad / s^2
	float angular_velocity = 0;			// rad / s
	float angular_position = 0;			// rad

	Vector2 force = { 0 };			// N
	Vector2 acceleration = { 0 };	// m/s^2
	Vector2 velocity = { 0 };		// m/s
	Vector2 position = { 0 };		// m

	Object()
	{
	};
	~Object()
	{
	};

	virtual void update(float dt)
	{
		anglular_acceleration = torque / inertia;
		angular_velocity = angular_velocity + anglular_acceleration * dt;
		angular_position = angular_position + angular_velocity * dt;

		torque = 0;

		acceleration = { force.x / mass, force.y / mass };
		velocity = { velocity.x + acceleration.x * dt,	velocity.y + acceleration.y * dt };
		position = { position.x + velocity.x * dt,		position.y + velocity.y * dt };

		force = { 0 };
	}

	virtual void display_object()
	{
	}
};

class Planet : public Object
{
public:

	Planet(float density, float r, Vector2 pos, Vector2 vel, Color c)
	{
		radius = r;
		mass = density * 4.0f / 3.0f * r * r * r * PI;
		inertia = 2.0f / 5.0f * mass * r * r;
		position = pos;
		velocity = vel;
		color = c;
	};

	void display_object() override
	{
		DrawCircleV(position, radius, color);
	}
};

class Ship : public Object
{
private:

	float ship_length = 30.0f;
	float ship_width = 10.0f;

	float main_thrust = 1.0f;	// N
	float rcs_thrust = 5000.0f;


public:

	enum ShipOrientation
	{
		free,
		retrograde,
		prograde
	};

	int target_direction = free;

	Ship(Vector2 pos, Vector2 vel, Color c)
	{
		mass = ship_length * ship_width;
		inertia = 1.0f / 12.0f * mass * ship_length * ship_length;
		position = pos;
		velocity = vel;
		color = c;
	};

	void display_object() override
	{
		DrawLineEx(position, { position.x + cosf(angular_position) * ship_length, position.y + sinf(angular_position) * ship_length }, ship_width, color);
	}

	void ship_controls()
	{
		if (IsKeyDown(KEY_W))
		{
			force.x += main_thrust * cosf(angular_position);
			force.y += main_thrust * sinf(angular_position);
		}
		else if (IsKeyDown(KEY_SPACE))
		{
			force.x += 10 * main_thrust * cosf(angular_position);
			force.y += 10 * main_thrust * sinf(angular_position);
		}

		if (IsKeyPressed(KEY_FIVE)) target_direction++;
		if (target_direction == 3) target_direction = 0;


		if ((velocity.x != 0 && velocity.x != 0) && target_direction == prograde)
		{
			angular_position = asinf(velocity.y / Vector2Length(velocity));
			if (velocity.x < 0) angular_position = PI - angular_position;
		}
		else if ((velocity.x != 0 && velocity.x != 0) && target_direction == retrograde)
		{
			angular_position = asinf(velocity.y / Vector2Length(velocity)) + PI;
			if (velocity.x < 0) angular_position = PI - angular_position;
		}
		else if (target_direction == free)
		{
			if (IsKeyDown(KEY_D)) angular_position += 0.01f;
			if (IsKeyDown(KEY_A)) angular_position -= 0.01f;
		}



		//if (IsKeyDown(KEY_D)) torque += rcs_thrust * ship_length;
		//if (IsKeyDown(KEY_A)) torque -= rcs_thrust * ship_length;

		////	ship rcs stabilizer

		//if (angular_velocity < -0.01f && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) torque += rcs_thrust * ship_length;
		//else if (angular_velocity > 0.01f && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) torque -= rcs_thrust * ship_length;
	}
};