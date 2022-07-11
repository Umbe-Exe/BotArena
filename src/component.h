#pragma once

struct updatable {
	virtual void update(double delta) = 0;
};

struct drawable {
	virtual void draw() = 0;
};