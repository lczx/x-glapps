#include "MouseFilter.h"

// Mouse filtering support variables
#define DEF_MOUSE_HISTORY_BUFSIZ 10
const float MouseFilter::MouseFilterWeight_ = 0.75f;

// http://www.flipcode.com/archives/Smooth_Mouse_Filtering.shtml
// A simple average filter of the last 10 inputs weighted based on their temporal distance.
// So the previous input is given more weight and the 5th latest input is given less weight.

// When using filtered mouse input, make sure the history buffer is filled with the
// appropriate initial value, otherwise you will see a sudden jerk in the first few frames.

MouseFilter::MouseFilter() : MouseFilter(DEF_MOUSE_HISTORY_BUFSIZ) { }

MouseFilter::MouseFilter(size_t bufSiz)
	: mouseHistory_(new glm::vec2[bufSiz]), mouseHistorySize_(bufSiz) { }

MouseFilter::~MouseFilter()
{
	delete[] mouseHistory_;
}

MouseFilter& MouseFilter::operator<<(const glm::vec2& mousePos)
{
	if (useFiltering_) {
		// Update history
		for (int i = mouseHistorySize_ - 1; i > 0; --i)
			mouseHistory_[i] = mouseHistory_[i - 1];
	}

	// Store current mouse entry at front of array
	mouseHistory_[0] = mousePos; // ref?

	return *this;
}

void MouseFilter::setEnabled(bool enabled)
{
	if (enabled != useFiltering_) {
		if (enabled) {
			for (int i = 1; i < mouseHistorySize_; ++i)
				mouseHistory_[i] = mouseHistory_[0]; // Check if works
		}
		useFiltering_ = enabled;
	}
}

bool MouseFilter::isEnabled()
{
	return useFiltering_;
}

glm::vec2 MouseFilter::getCurrentMousePos()
{
	if (useFiltering_) {
		float avgX = 0.0f, avgY = 0.0f, avgTotal = 0.0f, currentWeight = 1.0f;

		// Filter mouse input
		for (int i = 0; i < mouseHistorySize_; ++i) {
			glm::vec2 tmp = mouseHistory_[i];
			avgX += tmp.x * currentWeight;
			avgY += tmp.y * currentWeight;
			avgTotal += 1.0f * currentWeight;

			currentWeight *= MouseFilterWeight_;
		}

		return{ avgX / avgTotal, avgY / avgTotal };
	}

	// If filtering is disabled, simply return head of cache
	return mouseHistory_[0];
}
