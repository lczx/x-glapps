#pragma once

#include <glm/vec2.hpp>

// Use vector instead of array, pass buffer size by constructor param
// initPos called on filter toggle with element at head

class MouseFilter
{
public:
	MouseFilter();
	MouseFilter(size_t bufSiz);
	~MouseFilter();

	// Update history with the passed in new value
	MouseFilter &operator<<(const glm::vec2 &mousePos);

	// Disable filtering or enable (by initially filling history with element at head)
	void setEnabled(bool enabled);

	bool isEnabled();

	// Calculate and retrieve average mouse position
	glm::vec2 getCurrentMousePos();

private:
	// Mouse filtering support variables
	static const float MouseFilterWeight_;

	// Mouse history buffer
	glm::vec2 *mouseHistory_;
	size_t mouseHistorySize_;

	// Flag to enable filtering
	bool useFiltering_ = false;

};
