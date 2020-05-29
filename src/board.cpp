#include "board.h"

bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::status::START);
	mass_goal.setStatus(Mass::status::GOAL);

	// オープンリストに開始ノードを追加する
	open_list_.clear();
	open_list_.push_back(&mass_start);

	while (!open_list_.empty()) {
		std::sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::status::GOAL) {
			Mass* p = current;
			while (p) 
			{ 
				if (p->getStatus() == Mass::status::BLANK ||
					p->getStatus() == Mass::status::WATER || 
					p->getStatus() == Mass::status::ROAD )
					p->setStatus(Mass::status::WAYPOINT);
				p = p->getParent(); 
			}
			return true;
		}
		else {
			open_list_.erase(it);
			current->setListed(Mass::listed::CLOSE);

			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto& c : next)
			{
				if (c.x() < 0 || BOARD_SIZE <= c.x())
					continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y())
					continue;

				Mass& m = getMass(c);
				if (!m.isListed(Mass::listed::OPEN) &&
					!m.isListed(Mass::listed::CLOSE) &&
					m.getStatus() != Mass::status::WALL) {

					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::listed::OPEN);
				}
			}
		}
	}

	return false;
}

void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "|";
			switch (mass_[y][x].getStatus()) {
			case Mass::status::BLANK:
				std::cout << " ";
				break;
			case Mass::status::START:
				std::cout << "S";
				break;
			case Mass::status::GOAL:
				std::cout << "G";
				break;
			case Mass::status::WAYPOINT:
				std::cout << "o";
				break;
			case Mass::status::WALL:
				std::cout << "#";
				break;
			case Mass::status::WATER:
				std::cout << "~";
				break;
			case Mass::status::ROAD:
				std::cout << "$";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;

}
