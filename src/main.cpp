#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include "pathfinding.h"

void centerText(sf::Text* t_)
{
    sf::FloatRect textRect = t_->getLocalBounds();
    t_->setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
}

Pos screen_to_grid_pos(sf::Vector2f screenPos)
{
	screenPos.x = clamp(screenPos.x, 0, SCREEN_W);
	screenPos.y = clamp(screenPos.y, 0, SCREEN_H);
	return Pos(   (int)(  screenPos.x/(float)SCREEN_W * (float)(GRID_W-1)  )   ,   (int)(  screenPos.y/(float)SCREEN_H * (float)(GRID_H-1)  )   );
}

sf::Vector2f grid_to_screen_pos(Pos gridPos)
{
	return sf::Vector2f((float)gridPos.x/(float)GRID_W * (float)SCREEN_W, (float)gridPos.y/(float)GRID_H * (float)SCREEN_H);
}

int main()
{
	srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, 32), "Main", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    Pathfinder pF;
    pF.endPos = Pos(0,0);
    pF.startPos = Pos(GRID_W-1, GRID_H-1);
    pF.generate_wall_map();
    sf::Image visualMap;
    visualMap.create(GRID_W, GRID_H, sf::Color::Black);
    sf::Texture renderTex;
    renderTex.loadFromImage(visualMap);
    sf::Sprite renderSprite;
    renderSprite.setTexture(renderTex, true);
    renderSprite.setScale(sf::Vector2f((float)SCREEN_W/(float)GRID_W, (float)SCREEN_W/(float)GRID_W));

    int tick = 0;
    bool isAddingWall = false;
    std::vector<Pos> path = pF.find_path();

    while(window.isOpen())
    {
    	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                	Pos mouseGridPos = screen_to_grid_pos(mousePos);
                    isAddingWall = !pF.wallMap[mouseGridPos.x][mouseGridPos.y];
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
            	Pos newStartPos = screen_to_grid_pos(mousePos);
            	if (newStartPos != pF.startPos)
            	{
            		pF.startPos = newStartPos;
            		path = pF.find_path();
            	}
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
            	Pos newEndPos = screen_to_grid_pos(mousePos);
            	if (newEndPos != pF.endPos)
            	{
            		pF.endPos = newEndPos;
            		path = pF.find_path();
            	}
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
            	Pos mouseGridPos = screen_to_grid_pos(mousePos);
            	pF.wallMap[mouseGridPos.x][mouseGridPos.y] = isAddingWall;
            	Pos neighbourMove[8] = {Pos(-1,0), Pos(-1,-1), Pos(0,1), Pos(1, 1), Pos(1, 0), Pos(1, -1), Pos(0,-1), Pos(-1, -1)};
            	for (int i = 0; i < 8; ++i)
            	{
            		Pos newPos = mouseGridPos + neighbourMove[i];
            		if (is_valid_pos(newPos))
            		{
            			pF.wallMap[newPos.x][newPos.y] = isAddingWall;
            		}
            	}
            	path = pF.find_path();
            }
        }

        tick++;
        window.clear();

        for (int x = 0; x < GRID_W; ++x)
        {
        	for (int y = 0; y < GRID_H; ++y)
        	{
        		Pos myPos = Pos(x, y);
        		sf::Color renderCol = sf::Color::Black;
        		if (is_in_set(pF.openSet, myPos))
        		{
        			renderCol = sf::Color::Green;
        			renderCol.a = 100;
        		}
        		if (is_in_set(pF.closeSet, myPos))
        		{
        			renderCol = sf::Color::Red;
        			renderCol.a = 100;
        		}
        		if (is_in_set(path, Pos(x,y)))
        		{
        			renderCol = sf::Color::White;
        		}
        		if (pF.wallMap[x][y])
        		{
        			renderCol = sf::Color::Blue;
        		}
        		if (myPos == pF.endPos)
        		{
        			renderCol = sf::Color::Red;
        		}
        		if (myPos == pF.startPos)
        		{
        			renderCol = sf::Color::Green;
        		}
        		visualMap.setPixel(x, y, renderCol);
        	}
        }

        renderTex.loadFromImage(visualMap);
        window.draw(renderSprite);

        window.display();

    }

	return 0;
}