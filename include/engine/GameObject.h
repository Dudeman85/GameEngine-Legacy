#pragma once
#include <SFML/Graphics.hpp>

namespace engine
{

///
/// \brief The GameObject class
///
class GameObject
{
public:
	///
	/// \brief Creates new GameObject from texture.
	/// \param texture		= Texture for sprite.
	///
	GameObject(sf::Texture& texture);

	///
	/// \brief getSprite
	/// \return Returns sprite of the game object.
	///
	inline sf::Sprite& getSprite() { return m_sprite; }
	inline const sf::Sprite& getSprite() const { return m_sprite; }

private:
	sf::Sprite		m_sprite;
};

}
