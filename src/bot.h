#pragma once
#include "component.h"

struct Bot : updatable, drawable {

	void *initFn;
	void *updateFn;


	void update(float delta) override;
	void draw() override;

};