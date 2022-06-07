#pragma once

struct updatable {
	virtual void update(float delta) = 0;
};

struct drawable {
	virtual void draw() = 0;
};