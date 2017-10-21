#pragma once

#include <string>

#include "rapidxml/rapidxml.hpp"

class Configuration final
{
public:
	Configuration();
	~Configuration();

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;
	int getWindowX() const;
	int getWindowY() const;
	bool getWindowMaximized() const;
	bool getWindowFullscreen() const;
	void setWindowWidth(const unsigned int width);
	void setWindowHeight(const unsigned int height);
	void setWindowX(const int x);
	void setWindowY(const int y);
	void setWindowMaximized(const bool maximized);
	void setWindowFullscreen(const bool fullscreen);

private:
	void load();
	void save();
	rapidxml::xml_node<> *validateNode(rapidxml::xml_node<> *node);
	rapidxml::xml_attribute<> *validateAttribute(rapidxml::xml_attribute<> *attribute);
	rapidxml::xml_node<> *writeXMLNodeWindow(rapidxml::xml_document<> *document);
	void readXMLNodeWindow(rapidxml::xml_node<> *root);
	static const char *boolToString(const bool boolean);
	static const bool stringToBool(const char *string);

	static const char *BOOL_TRUE;
	static const char *BOOL_FALSE;
	const std::string FILE_NAME = "config.xml";
	const char *XML_NODE_ROOT = "config";
	const char *XML_NODE_WINDOW = "window";
	const char *XML_NODE_WINDOW_DIMENSIONS = "dimensions";
	const char *XML_NODE_WINDOW_DIMENSIONS_WIDTH = "width";
	const char *XML_NODE_WINDOW_DIMENSIONS_HEIGHT = "height";
	const char *XML_NODE_WINDOW_DIMENSIONS_X = "x";
	const char *XML_NODE_WINDOW_DIMENSIONS_Y = "y";
	const char *XML_NODE_WINDOW_MAXIMIZED = "maximized";
	const char *XML_NODE_WINDOW_FULLSCREEN = "fullscreen";

	bool changed = false;

	unsigned int windowWidth = 1024;
	unsigned int windowHeight = 768;
	int windowX = 64;
	int windowY = 64;
	bool windowMaximized = false;
	bool windowFullscreen = false;
};