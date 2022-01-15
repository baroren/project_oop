#pragma once

#include "Static.h"


class Gift : public Static {

public:

	using Static::Static;


	virtual void handleCollision(Player& player) = 0;
	virtual void handleCollision(Dwarf& dwarf)override {};

	virtual void handleCollision(Mage& mage) override {};

private:


};