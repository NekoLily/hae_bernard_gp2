#include <SFML/Graphics.hpp>


using namespace sf;
class Tank : public sf::Drawable, public sf::Transformable
{
public:
    RectangleShape box;
    CircleShape     circle;
    Tank() 
    {
        box.setSize(Vector2f(50, 50));
        circle.setRadius(5);
        circle.setFillColor(Color::Blue);
    }
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(box, states);
        target.draw(circle, states);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    Tank tank;
    tank.setPosition(50, 50);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(tank);
        window.display();
    }

    return 0;
}