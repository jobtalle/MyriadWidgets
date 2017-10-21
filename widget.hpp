#pragma once

#include <myriad.h>
#include <vector>
#include <memory>

class Widget
{
	friend class RootWidget;

public:
	virtual ~Widget();

	void update(const float timeStep);
	void key(const int key, const int code, const int action, const int mods);
	virtual void mouseMove(const int x, const int y);
	void mouseButton(const int button, const int action, const int mods);
	void mouseScroll(const int x, const int y);
	void resize(const unsigned int width, const unsigned int height);
	void move(const int x, const int y);

	unsigned int getMouseX() const;
	unsigned int getMouseY() const;
	unsigned int getX() const;
	unsigned int getY() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	bool getFocus() const;
	myr::RenderTarget *getRenderTarget();

protected:
	Widget(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);
	Widget(const unsigned int width, const unsigned int height);

	virtual void updateHandler(const float timeStep) = 0;
	virtual void keyHandler(const int key, const int code, const int action, const int mods) = 0;
	virtual void mouseMoveHandler() = 0;
	virtual void mouseButtonHandler(const int button, const int action, const int mods) = 0;
	virtual void mouseScrollHandler(const int x, const int y) = 0;
	virtual void resizeHandler() = 0;
	virtual void moveHandler() = 0;
	virtual void focusHandler() {};

	void addChild(Widget *child);
	void removeChild(Widget *child);

private:
	static const myr::Color CLEAR_COLOR;

	virtual void setFocus(const bool focus);
	Widget *getMouseFocus();
	Widget *getRoot();
	void invalidateFocus();

	Widget *parent = nullptr;
	Widget *lastFocus = nullptr;
	std::unique_ptr<myr::RenderTarget> renderTarget;
	std::vector<Widget*> children;
	int mouseX = 0;
	int mouseY = 0;
	int x;
	int y;
	unsigned int width;
	unsigned int height;
	bool focus = false;
};