#pragma once

class CLifespan
{
public:
	int remaining = 0;	//amount of lifespan remaining on the entity
	int total = 0;		//the total initial amount of lifespan

	CLifespan(int total) : remaining(total), total(total){}

};