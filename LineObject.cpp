#include "LineObject.h"

LineObject::LineObject(unsigned int mpl, unsigned int spl, size_t time){
	this->mainPartLength = mpl;
	this->spacesLength = spl;
	this->time = time;
}

bool LineObject::operator==(const size_t& s){
	return this->time == s;
}

bool LineObject::operator<(const LineObject & lo1) const {
	return this->time < lo1.time;
}