#pragma once

#include <myriad.h>
#include <GLFW/glfw3.h>

#include "configuration.hpp"
#include "rootWidget.hpp"

typedef RootWidget *(*WindowInitializer)(const unsigned int, const unsigned int);

class Window final
{
public:
	Window(WindowInitializer initializer);
	~Window();
	void run();

private:
	void initializeGLFW();
	void initializeMyriad();
	void freeGLFW();
	void freeMyriad();
	void key(const int key, const int code, const int action, const int mods);
	void resize(const int width, const int height);
	void move(const int x, const int y);
	void maximize(const bool maximized);
	void mouseMove(const int x, const int y);
	void mouseButton(const int button, const int action, const int mods);
	void mouseScroll(const int x, const int y);
	void mouseEnter(const bool entered);
	void setFullscreen(const bool fullscreen);

	static void key(GLFWwindow *window, int key, int code, int action, int mods);
	static void resize(GLFWwindow *window, int width, int height);
	static void move(GLFWwindow *window, int x, int y);
	static void maximize(GLFWwindow *window, int maximized);
	static void mouseMove(GLFWwindow *window, double x, double y);
	static void mouseButton(GLFWwindow *window, int button, int action, int mods);
	static void mouseScroll(GLFWwindow *window, double x, double y);
	static void mouseEnter(GLFWwindow *window, int entered);

	static const char *TITLE;
	static const myr::Color CLEAR_COLOR;

	float timeStep;
	Configuration configuration;
	RootWidget *content;
	GLFWwindow *window;
	myr::Renderer *renderer;
};