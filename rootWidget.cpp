#include "rootWidget.hpp"

#include <iostream> // TODO: Debug

RootWidget *RootWidget::getReplacement()
{
	return replacement;
}

void RootWidget::mouseMove(const int x, const int y)
{
	Widget::mouseMove(x, y);

	updateFocus();
}

void RootWidget::setFocus(const bool focus)
{
	if(!focus && lastFocus != this)
	{
		lastFocus->setFocus(false);
		lastFocus = nullptr;
	}

	Widget::setFocus(focus);
}

RootWidget::RootWidget(const unsigned int width, const unsigned int height)
	:Widget(width, height)
{
	
}

void RootWidget::setReplacement(RootWidget *widget)
{
	delete replacement;

	replacement = widget;
}

void RootWidget::updateFocus()
{
	auto widgetFocus = getMouseFocus();

	if(widgetFocus != lastFocus)
	{
		if(lastFocus)
			lastFocus->setFocus(false);

		lastFocus = widgetFocus;
		lastFocus->setFocus(true);
	}
}