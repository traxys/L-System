#ifndef LSYSTEM_HPP
#define LSYSTEM_HPP

#include <cmath>
#include <map>
#include <variant>
#include <vector>
#include <utility>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>

template<typename V,typename C>
class LSystem{
	public:
		using letter = std::variant<V,C>;
		using ruleTable = std::map<V, std::vector<letter>>;
		using drawingFunction = std::function<void(sf::RenderWindow*,
				LSystem* )>;
		using drawingTable = std::map<letter,drawingFunction>;

		LSystem(const std::vector<letter>& initial,
				const ruleTable& rules_,
				double turnAngle,
				double initAngle,
				double forwardDistance_,
				sf::Vector2f initalPos_) :
			state(initial),
			rules(rules_),
			turningAngle(turnAngle),
			currentAngle(initAngle),
			forwardDistance(forwardDistance_),
			currentPos(initalPos_),
			initialAngle(initAngle),
			initalPos(initalPos_) {}

		static letter createVariable(const V& variable){
			return letter(std::in_place_index_t<0>(), variable);
		}
		static letter createConstant(const C& constant){
			return letter(std::in_place_index_t<1>(), constant);
		}

		void step(){
			std::vector<letter> newState;
			for(auto& current : state){
				if(auto pval = std::get_if<0>(&current)){
					for(auto& product : rules[*pval]){
						newState.push_back(product);
					}
				}else{
					newState.push_back(current);
				}
			}
			state = newState;
		}

		void printState() const{
			for(auto& current : state){
				std::visit([](auto&& arg){std::cout << arg;}, current);
			}
			std::cout << std::endl;
		}

		sf::VertexArray forward(){
			sf::VertexArray line(sf::Lines);
			line.append(currentPos);
			sf::Vector2f end = { float(forwardDistance * 
										cos(currentAngle)) + currentPos.x,
								 float(forwardDistance * 
										sin(currentAngle)) + currentPos.y};
			line.append(end);
			currentPos = end;
			return line;
		}

		void setAngle(double angle){
			currentAngle = angle;
		}
		void setPos(sf::Vector2f pos){
			currentPos = pos;
		}

		double getAngle() const{
			return currentAngle;
		}
		sf::Vector2f getPos() const{
			return currentPos;
		}

		void turnClockwise(){
			currentAngle -= turningAngle;
		}
		void turnCounterClockwise(){
			currentAngle += turningAngle;
		}

		void reset(){
			currentAngle = initialAngle;
			currentPos = initalPos;
		}

		void draw(const drawingTable& table){
			sf::RenderWindow window(sf::VideoMode(800, 600),
					"L-System");
			while (window.isOpen()){
				sf::Event event;
				while (window.pollEvent(event)){
					if (event.type == sf::Event::Closed)
						window.close();
				}
				window.clear(sf::Color::Black);

				reset();
				for(auto& curr : state){
					table.at(curr)(&window, this);
				}

				window.display();
			}
		}
	private:
		std::vector<letter> state;
		ruleTable rules;

		double turningAngle;
		double currentAngle;

		double forwardDistance;
		sf::Vector2f currentPos;

		double initialAngle;
		sf::Vector2f initalPos;
};

#endif /* LSYSTEM_HPP */
