#pragma once
#include <string>

class LineObject {
public:
	LineObject() = default;
	LineObject(unsigned int mpl, unsigned int spl, size_t time);

	bool operator< (const LineObject& lo) const;
	bool operator==(const size_t& s);

	unsigned int mainPartLength;
	unsigned int spacesLength;
	size_t time;
};