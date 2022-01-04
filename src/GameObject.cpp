
#include "GameObject.h"

GameObject::GameObject(const int x,const int y,
                       const int numOfAnim,const int numOfFrames,const float animTime, float scale, string imagePath)
                       :m_imagePath(imagePath)

{
    if (!m_texture.loadFromFile(m_imagePath))
    {
        // error...
        std::cout<<"error loading font";

    }
    m_sprite.setTexture(m_texture);
    m_animation =new Animation(&m_texture,sf::Vector2u(numOfFrames,numOfAnim),animTime);
    m_sprite.setOrigin(m_sprite.getGlobalBounds().width/numOfFrames/2.f,m_sprite.getGlobalBounds().height/2.f);
    m_sprite.setPosition(sf::Vector2f(x,y));
    m_sprite.scale(scale,scale);// need to make func
}

void GameObject:: updateAndDraw(const int row,float deltaTime,sf::RenderWindow& window)
{
    m_animation->update(row,deltaTime);
    m_sprite.setTextureRect(m_animation->getRect());
    window.draw(m_sprite);

}

void GameObject::setLocation(const float x, const float y )
{
    m_sprite.setPosition(sf::Vector2f(x,y));
}

sf::Vector2<float> GameObject::getLocation( ) const
{
    return m_sprite.getPosition();
}