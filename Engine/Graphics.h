#pragma once

class Transform;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void initialize();
	Transform *addCube();
	void update();

	
};

