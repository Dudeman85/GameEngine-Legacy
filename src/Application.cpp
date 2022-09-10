#include <engine/Application.h>

namespace engine
{

// assetit eri kansiosta riippuen ollanko debug vai relese käännöksessä.
#if defined(_DEBUG)
static const  std::string ASSET_PATH = "../assets/";
#else
static const std::string ASSET_PATH = "assets/";
#endif

// Anonyymi nimiavaruus loadTextures funktiolle, jotta
// se "ei näy muualle", kuin vain tähän cpp tiedostoon
namespace {

///
/// \brief loadTextures function.
/// \param fileName		= Vector of filenames to be loaded as textures
/// \return Vector of sf::Texture objects.
///
std::vector<sf::Texture> loadTextures(const std::vector<std::string>& fileNames) {
	std::vector<sf::Texture> textures;
	for(auto& fileName : fileNames)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(ASSET_PATH + fileName))
		{
			throw std::runtime_error("ERROR: Failed to load texture: " + fileName);
		}
		textures.push_back(texture);
	}
	return textures;
}

}


// Application::Application
Application::Application(const std::vector<std::string> textures, sf::RenderWindow* window)
	: m_window(window)
	, m_textures(loadTextures(textures))
{
	m_frameTimer.restart();
}


// Application::update
float Application::update()
{
	// Run window event loop
	if(m_window != 0)
	{
		sf::Event event;
		while (m_window->pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				m_window->close();
		}
	}

	float deltaTime = m_frameTimer.getElapsedTime().asSeconds();
	m_frameTimer.restart();
	for(const auto& gameObject : getGameObjects())
	{
		//gameObject->update(deltaTime));
	}
	return deltaTime;
}


// Application::render
void Application::render() const
{
	if(m_window)
	{
		m_window->clear(sf::Color::Magenta);
		for(const auto& gameObject : getGameObjects())
		{
			m_window->draw(gameObject->getSprite());
		}
		m_window->display();
	}
}


// Application::souldClose
bool Application::shouldClose()
{
	return !m_window->isOpen();
}

}
