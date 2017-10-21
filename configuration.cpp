#include <fstream>
#include <vector>
#include <exception>

#include "configuration.hpp"
#include "rapidxml/rapidxml_print.hpp"

const char *Configuration::BOOL_TRUE = "true";
const char *Configuration::BOOL_FALSE = "false";

Configuration::Configuration()
{
	load();
}

Configuration::~Configuration()
{
	if(changed)
		save();
}

unsigned int Configuration::getWindowWidth() const
{
	return windowWidth;
}

unsigned int Configuration::getWindowHeight() const
{
	return windowHeight;
}

int Configuration::getWindowX() const
{
	return windowX;
}

int Configuration::getWindowY() const
{
	return windowY;
}

bool Configuration::getWindowMaximized() const
{
	return windowMaximized;
}

bool Configuration::getWindowFullscreen() const
{
	return windowFullscreen;
}

void Configuration::setWindowWidth(const unsigned int width)
{
	if(!getWindowMaximized())
	{
		windowWidth = width;
		changed = true;
	}
}

void Configuration::setWindowHeight(const unsigned int height)
{
	if(!getWindowMaximized())
	{
		windowHeight = height;
		changed = true;
	}
}

void Configuration::setWindowX(const int x)
{
	windowX = x;
	changed = true;
}

void Configuration::setWindowY(const int y)
{
	windowY = y;
	changed = true;
}

void Configuration::setWindowMaximized(const bool maximized)
{
	windowMaximized = maximized;
	changed = true;
}

void Configuration::setWindowFullscreen(const bool fullscreen)
{
	windowFullscreen = fullscreen;
	changed = true;
}

void Configuration::load()
{
	std::ifstream file(FILE_NAME);

	if(file)
	{
		std::vector<char> fileContents;

		file.seekg(0, std::ios::end);
		fileContents.resize(file.tellg());
		file.seekg(0, std::ios::beg);

		file.read(&fileContents[0], fileContents.size());
		file.close();

		try
		{
			rapidxml::xml_document<> document;
			document.parse<0>(fileContents.data());

			auto *config = validateNode(document.first_node(XML_NODE_ROOT));

			readXMLNodeWindow(config);
		}
		catch(...)
		{
			changed = true;
		}
	}
	else
	{
		changed = true;
	}
}

void Configuration::save()
{
	rapidxml::xml_document<> document;
	auto *config = document.allocate_node(rapidxml::node_element, XML_NODE_ROOT);

	config->append_node(writeXMLNodeWindow(&document));

	document.append_node(config);

	std::ofstream file(FILE_NAME);
	file << document;
	file.close();
}

rapidxml::xml_node<> *Configuration::validateNode(rapidxml::xml_node<> *node)
{
	if(node == nullptr)
		throw std::exception();

	return node;
}

rapidxml::xml_attribute<> *Configuration::validateAttribute(rapidxml::xml_attribute<> *attribute)
{
	if(attribute == nullptr)
		throw std::exception();

	return attribute;
}

rapidxml::xml_node<> *Configuration::writeXMLNodeWindow(rapidxml::xml_document<> *document)
{
	auto *window = document->allocate_node(rapidxml::node_element, XML_NODE_WINDOW);
	auto *dimensions = document->allocate_node(rapidxml::node_element, XML_NODE_WINDOW_DIMENSIONS);

	dimensions->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_DIMENSIONS_WIDTH,
		document->allocate_string(std::to_string(getWindowWidth()).c_str())));
	dimensions->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_DIMENSIONS_HEIGHT,
		document->allocate_string(std::to_string(getWindowHeight()).c_str())));
	dimensions->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_DIMENSIONS_X,
		document->allocate_string(std::to_string(getWindowX()).c_str())));
	dimensions->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_DIMENSIONS_Y,
		document->allocate_string(std::to_string(getWindowY()).c_str())));

	window->append_node(dimensions);
	window->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_MAXIMIZED,
		boolToString(getWindowMaximized())));
	window->append_node(document->allocate_node(
		rapidxml::node_element,
		XML_NODE_WINDOW_FULLSCREEN,
		boolToString(getWindowFullscreen())));

	return window;
}

void Configuration::readXMLNodeWindow(rapidxml::xml_node<> *root)
{
	auto *window = validateNode(root->first_node(XML_NODE_WINDOW));
	auto *dimensions = validateNode(window->first_node(XML_NODE_WINDOW_DIMENSIONS));

	windowWidth = std::stoi(validateNode(dimensions->first_node(XML_NODE_WINDOW_DIMENSIONS_WIDTH))->value());
	windowHeight = std::stoi(validateNode(dimensions->first_node(XML_NODE_WINDOW_DIMENSIONS_HEIGHT))->value());
	windowX = std::stoi(validateNode(dimensions->first_node(XML_NODE_WINDOW_DIMENSIONS_X))->value());
	windowY = std::stoi(validateNode(dimensions->first_node(XML_NODE_WINDOW_DIMENSIONS_Y))->value());
	windowMaximized = stringToBool(validateNode(window->first_node(XML_NODE_WINDOW_MAXIMIZED))->value());
	windowFullscreen = stringToBool(validateNode(window->first_node(XML_NODE_WINDOW_FULLSCREEN))->value());

	if(windowFullscreen && !windowMaximized)
		setWindowFullscreen(false);
}

const char *Configuration::boolToString(const bool boolean)
{
	return boolean?BOOL_TRUE:BOOL_FALSE;
}

const bool Configuration::stringToBool(const char *string)
{
	return strcmp(string, BOOL_TRUE) == 0;
}