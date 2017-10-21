#pragma once

#include "widget.hpp"

class RootWidget : public Widget
{
	friend class Widget;

public:
	RootWidget *getReplacement();
	void mouseMove(const int x, const int y);
	void setFocus(const bool focus);

protected:
	RootWidget(const unsigned int width, const unsigned int height);
	void setReplacement(RootWidget *widget);

private:
	void updateFocus();

	RootWidget *replacement = nullptr;
};