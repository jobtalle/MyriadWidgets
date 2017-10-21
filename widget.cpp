#include "widget.hpp"
#include "rootWidget.hpp"

#include <iostream>

Widget::~Widget()
{
	for(auto child : children)
		delete child;
}

void Widget::update(const float timeStep)
{
	for(auto child : children)
		child->update(timeStep);
	
	renderTarget->bind();
	renderTarget->clear();

	updateHandler(timeStep);

	renderTarget->setTransform(myr::Transform::IDENTITY);

	for(auto child : children)
		child->getRenderTarget()->draw(child->getX(), child->getY());
}

void Widget::key(const int key, const int code, const int action, const int mods)
{
	for(auto child : children)
		child->key(key, code, action, mods);

	keyHandler(key, code, action, mods);
}

void Widget::mouseMove(const int x, const int y)
{
	mouseX = (unsigned int) x;
	mouseY = (unsigned int) y;

	for(auto child : children)
		child->mouseMove(x - child->getX(), y - child->getY());

	mouseMoveHandler();
}

void Widget::mouseButton(const int button, const int action, const int mods)
{
	for(auto child : children)
		child->mouseButton(button, action, mods);

	mouseButtonHandler(button, action, mods);
}

void Widget::mouseScroll(const int x, const int y)
{
	for(auto child : children)
		child->mouseScroll(x, y);

	mouseScrollHandler(x, y);
}

void Widget::resize(const unsigned int width, const unsigned int height)
{
	renderTarget->setSize(myr::Rect(width, height));

	this->width = width;
	this->height = height;

	invalidateFocus();
	resizeHandler();
}

void Widget::move(const int x, const int y)
{
	this->x = x;
	this->y = y;

	invalidateFocus();
	moveHandler();
}

unsigned int Widget::getMouseX() const
{
	return mouseX;
}

unsigned int Widget::getMouseY() const
{
	return mouseY;
}

unsigned int Widget::getX() const
{
	return x;
}

unsigned int Widget::getY() const
{
	return y;
}

unsigned int Widget::getWidth() const
{
	return width;
}

unsigned int Widget::getHeight() const
{
	return height;
}

bool Widget::getFocus() const
{
	return focus;
}

myr::RenderTarget *Widget::getRenderTarget()
{
	return renderTarget.get();
}

Widget::Widget(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
	:x(x), y(y), width(width), height(height), renderTarget(new myr::RenderTarget(CLEAR_COLOR, myr::Rect(width, height)))
{

}

Widget::Widget(const unsigned int width, const unsigned int height)
	: Widget(0, 0, width, height)
{

}

void Widget::addChild(Widget *child)
{
	child->parent = this;
	child->mouseX = mouseX - child->x;
	child->mouseY = mouseY - child->y;

	children.push_back(child);

	invalidateFocus();
}

void Widget::removeChild(Widget *child)
{
	children.erase(std::find(children.begin(), children.end(), child));

	if(child == lastFocus)
		lastFocus = nullptr;

	invalidateFocus();
}

void Widget::setFocus(const bool focus)
{
	if(this->focus != focus)
	{
		this->focus = focus;

		focusHandler();
	}
}

Widget *Widget::getMouseFocus()
{
	Widget *widgetFocus = nullptr;

	for(auto child : children)
	{
		if(getMouseX() >= child->getX() && getMouseX() < child->getX() + child->getWidth())
			if(getMouseY() >= child->getY() && getMouseY() < child->getY() + child->getHeight())
				widgetFocus = child;
	}

	if(widgetFocus)
		return widgetFocus->getMouseFocus();
	else
		return this;
}

Widget *Widget::getRoot()
{
	if(!parent)
		return this;

	Widget *root = parent;

	while(root->parent)
		root = root->parent;

	return root;
}

void Widget::invalidateFocus()
{
	if(parent)
		reinterpret_cast<RootWidget*>(getRoot())->updateFocus();
}