#include "lsystem.hpp"
#include "fern.hpp"

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <variant>
#include <stack>
#include <iostream>
#include <SFML/Graphics.hpp>

int main(){
	std::vector<fern::Fern::letter> initial = { fern::X };
	fern::Fern::ruleTable table;
	table[fern::X] = { 	fern::F,
						fern::p,
						fern::braopen,
						fern::braopen,
						fern::X,
						fern::braclose,
						fern::m,
						fern::X,
						fern::braclose,
						fern::m,
						fern::F,
						fern::braopen,
						fern::m,
						fern::F,
						fern::X,
						fern::braclose,
						fern::p,
						fern::X};
	table[fern::F] = { fern::F,fern::F };
	fern::Fern fern(initial, table, (25*M_PI)/180, -M_PI_2, 2.5, sf::Vector2f(400,600));
	
	for(int i = 0; i < 6; ++i){
		fern.step();
	}
	std::stack< std::pair<sf::Vector2f, double> > posStack;
	fern::Fern::drawingFunction nop = [](auto*, auto*){};
	fern::Fern::drawingFunction save = 
		[&posStack](auto*, auto* sys){
			posStack.push({sys->getPos(), sys->getAngle()});
		};
	fern::Fern::drawingFunction restore =
		[&posStack](auto*, auto* sys){
			auto tp = posStack.top();
			posStack.pop();
			sys->setPos(tp.first);
			sys->setAngle(tp.second);
		};
	fern::Fern::drawingFunction turnclock = 
		[](auto*, auto* sys){
			sys->turnClockwise();
		};
	fern::Fern::drawingFunction turncounter = 
		[](auto*, auto* sys){
			sys->turnCounterClockwise();
		};
	fern::Fern::drawingFunction forward = 
		[](auto* win, auto* sys){
			auto line = sys->forward();
			win->draw(line);
		};
	fern::Fern::drawingTable drawingTable;
	drawingTable[fern::X] = nop;
	drawingTable[fern::F] = forward;
	drawingTable[fern::m] = turnclock;
	drawingTable[fern::p] = turncounter;
	drawingTable[fern::braopen] = save;
	drawingTable[fern::braclose] = restore;
	fern.draw(drawingTable);
	
	return 0;
}
