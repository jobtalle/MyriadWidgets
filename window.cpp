#include "window.hpp"

void Window::key(GLFWwindow *window, int key, int code, int action, int mods)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->key(key, code, action, mods);
}

void Window::resize(GLFWwindow *window, int width, int height)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->resize(width, height);
}

void Window::move(GLFWwindow *window, int x, int y)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->move(x, y);
}

void Window::maximize(GLFWwindow *window, int maximized)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->maximize(maximized == GLFW_TRUE);
}

void Window::mouseMove(GLFWwindow *window, double x, double y)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseMove(int(x), int(y));
}

void Window::mouseButton(GLFWwindow *window, int button, int action, int mods)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseButton(button, action, mods);
}

void Window::mouseScroll(GLFWwindow *window, double x, double y)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseScroll(int(x), int(y));
}

void Window::mouseEnter(GLFWwindow *window, int entered)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseEnter(entered == GLFW_TRUE);
}

Window::Window(WindowInitializer initializer)
{
	initializeGLFW();
	initializeMyriad();

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	content = initializer(renderer->getSize().getWidth(), renderer->getSize().getHeight());
	content->mouseMove(static_cast<int>(mouseX), static_cast<int>(mouseY));

	run();
}

Window::~Window()
{
	delete content;

	freeMyriad();
	freeGLFW();
}

void Window::run()
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		content->update(timeStep);

		renderer->getDefaultRenderTarget().bind();
		renderer->getDefaultRenderTarget().clear();

		content->getRenderTarget()->draw(0, 0);
		renderer->render();

		glfwSwapBuffers(window);

		if(content->getReplacement())
		{
			auto old = content;

			content = content->getReplacement();

			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			content->mouseMove(static_cast<int>(mouseX), static_cast<int>(mouseY));

			delete old;
		}
	}
}

void Window::initializeGLFW()
{
	glfwInit();

	int monitorCount;
	timeStep = 1.0f / glfwGetVideoMode(*glfwGetMonitors(&monitorCount))->refreshRate;
	
	if(configuration.getWindowFullscreen())
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	window = glfwCreateWindow(
		configuration.getWindowWidth(),
		configuration.getWindowHeight(),
		TITLE,
		nullptr,
		nullptr);
	glfwSetWindowPos(window, configuration.getWindowX(), configuration.getWindowY());
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, &Window::resize);
	glfwSetWindowPosCallback(window, &Window::move);
	glfwSetWindowMaximizeCallback(window, &Window::maximize);
	glfwSetKeyCallback(window, &Window::key);
	glfwSetCursorPosCallback(window, &Window::mouseMove);
	glfwSetMouseButtonCallback(window, &Window::mouseButton);
	glfwSetScrollCallback(window, &Window::mouseScroll);
	glfwSetCursorEnterCallback(window, &Window::mouseEnter);
}

void Window::initializeMyriad()
{
	myr::initialize();

	renderer = new myr::Renderer(
		CLEAR_COLOR,
		myr::Rect(configuration.getWindowWidth(), configuration.getWindowHeight()));

	if(configuration.getWindowMaximized())
		glfwMaximizeWindow(window);
}

void Window::freeGLFW()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::freeMyriad()
{
	delete renderer;
}

void Window::key(const int key, const int code, const int action, const int mods)
{
	if(action && GLFW_PRESS && key == GLFW_KEY_ENTER && mods == GLFW_MOD_ALT)
		setFullscreen(!configuration.getWindowFullscreen());
	else
		content->key(key, code, action, mods);
}

void Window::resize(const int width, const int height)
{
	configuration.setWindowWidth(width);
	configuration.setWindowHeight(height);

	renderer->setSize(myr::Rect(width, height));
	content->resize(width, height);
}

void Window::move(const int x, const int y)
{
	configuration.setWindowX(x);
	configuration.setWindowY(y);
}

void Window::maximize(const bool maximized)
{
	configuration.setWindowMaximized(maximized);
}

void Window::mouseMove(const int x, const int y)
{
	content->mouseMove(x, y);
}

void Window::mouseButton(const int button, const int action, const int mods)
{
	content->mouseButton(button, action, mods);
}

void Window::mouseScroll(const int x, const int y)
{
	content->mouseScroll(x, y);
}

void Window::mouseEnter(const bool entered)
{
	content->setFocus(entered);
}

void Window::setFullscreen(const bool fullscreen)
{
	if(configuration.getWindowFullscreen() != fullscreen)
	{
		configuration.setWindowFullscreen(fullscreen);

		if(fullscreen)
		{
			glfwRestoreWindow(window);
			glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
			glfwMaximizeWindow(window);
		}
		else
		{
			glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
			glfwRestoreWindow(window);
		}
	}
}