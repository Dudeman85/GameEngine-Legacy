#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include <engine/GameObject.h>

namespace engine
{

///
/// \brief The Application class
///
class Application
{
public:
	///
	/// \brief Application
	/// \param textures
	///
	Application(const std::vector<std::string> textures, sf::RenderWindow* window = 0);

	/// Tällä funktiolla voi spawnata uusia objekteja tyyliin app.spawn<Player()>(textures[0]);
	/// \brief spawn<T>(size_t) -> std::shared_ptr<T>
	/// \param textureIndex		= Tekstuurin indeksi, joka asetetaan peliobjektille.
	/// \return Shared pointer of given GameObject (T) kind.
	///
	template<typename T>
	inline std::shared_ptr<T> spawn(size_t textureIndex) {
		std::shared_ptr<T> p = std::make_shared<T>(m_textures[textureIndex]);
		m_gameObjects.push_back(p);
		return p;
	}

	///
	/// \brief Updates a frame.
	/// \return Returns deltaTime for frame.
	///
	float update();

	///
	/// \brief Renders game to window.
	///
	void render() const;

	///
	/// \brief shouldClose
	/// \return True, if application should shut down.
	///
	bool shouldClose();

	///
	/// \brief getGameObjects
	/// \return Returns all game objects.
	///
	inline auto& getGameObjects() { return m_gameObjects; }
	inline const auto& getGameObjects() const { return m_gameObjects; }

private:
	sf::RenderWindow*								m_window;
	std::vector<sf::Texture>						m_textures;
	std::vector< std::shared_ptr<GameObject> >		m_gameObjects;
	sf::Clock										m_frameTimer;
};

}
