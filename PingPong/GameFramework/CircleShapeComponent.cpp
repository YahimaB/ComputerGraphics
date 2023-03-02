#include "CircleShapeComponent.h"
#include "Game.h"


CircleShapeComponent::CircleShapeComponent(float radius, PlatformShapeComponent* platforms[])
{
	pointsCount = 25;
	indicesCount = (pointsCount - 1) * 3;

	InitPoints();
	InitIndeces();

	this->radius = radius;
	this->startSpeed = 0.001f;

	Platforms[0] = platforms[0];
	Platforms[1] = platforms[1];

	LoseZones[0] = { -1.5f, -1.0f, 1.0f, -1.1f };
	LoseZones[1] = { 1.1f, -1.0f, 1.0f, 1.5f };

	Borders[0] = { -1.0f, 1.0f, 1.5f, 1.0f };
	Borders[1] = { -1.0f, -1.5f, -1.0f, 1.0f };

	scores[0] = 0;
	scores[1] = 0;

	Restart();
}

void CircleShapeComponent::InitPoints() {
	points[0].pos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	double PI = 2 * acos(0.0);
	double step = PI * 2 / (pointsCount - 1);
	double angle = 0.0f;
	int i = 1;

	while (angle < PI * 2) {
		float x, y;
		x = cos(angle) * radius;
		y = sin(angle) * radius;

		points[i].pos = float4(x, y, 0.0f, 1.0f);

		i++;
		angle += step;
	}
}

void CircleShapeComponent::InitIndeces() {

	int sl = 1;

	for (int i = 0; i < indicesCount; i += 3) {
		if ((i + 3) < indicesCount) {
			indices[i] = 0;
			indices[i + 1] = sl;
			indices[i + 2] = ++sl;
		}
		else {
			indices[i] = 0;
			indices[i + 1] = indicesCount / 3;
			indices[i + 2] = 1;
		}
	}
};

void CircleShapeComponent::Update(float deltaTime)
{
	offsetX += currentSpeed * deltaTime * direction.x;
	offsetY += currentSpeed * deltaTime * direction.y;

	ConstBuf constBuf;
	constBuf.Offset = float4(offsetX, offsetY, 0.0f, 0.0f);
	game->context->UpdateSubresource(constBuffer, 0, NULL, &constBuf, 0, 0);

	auto rect0 = *GetRect();

	if (Intersects(rect0, Borders[0]) || Intersects(rect0, Borders[1]))
	{
		direction.y *= -1;
		return;
	}

	bool restart = false;
	if (Intersects(rect0, LoseZones[0]))
	{
		restart = true;
		scores[1]++;
	} 
	
	if (Intersects(rect0, LoseZones[1]))
	{
		restart = true;
		scores[0]++;
	}

	if (restart)
	{
		Restart();
		constBuf.Offset = float4(offsetX, offsetY, 0.0f, 0.0f);
		game->context->UpdateSubresource(constBuffer, 0, NULL, &constBuf, 0, 0);		
		return;
	}

	auto rect1 = *Platforms[0]->GetRect();
	auto rect2 = *Platforms[1]->GetRect();
	if (Intersects(rect0, rect1))
	{
		if (rect0.bot > rect1.top || rect0.top < rect1.bot)
		{
			direction.y *= -1;
		}
		else
		{
			direction.x *= -1;
			currentSpeed *= 1.1;
		}
		return;
	}

	if (Intersects(rect0, rect2))
	{
		if (rect0.bot > rect2.top || rect0.top < rect2.bot)
		{
			direction.y *= -1;
		}
		else
		{
			direction.x *= -1;
			currentSpeed *= 1.1;
		}
		return;
	}
}

CustomRect* CircleShapeComponent::GetRect()
{
	CustomRect rect = {};

	rect.left = points[0].pos.x - radius + offsetX;
	rect.bot = points[0].pos.y - radius + offsetY;

	rect.right = points[0].pos.x + radius + offsetX;
	rect.top = points[0].pos.y + radius + offsetY;

	return &rect;
}

void CircleShapeComponent::Restart()
{
	std::cout << "SCORE:    " << scores[0] << " : " << scores[1] << std::endl;

	this->currentSpeed = startSpeed;
	offsetX = 0;
	offsetY = 0;

	double half_PI = acos(0.0);
	double angle = acos(randNum(5, 9) / 10.0f);
	angle += randNum(0, 3) * half_PI;

	direction.x = cos(angle);
	direction.y = sin(angle);
}
