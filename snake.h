#ifndef _SNAKE_H
#define _SNAKE_H

#include<list>
#include<utility>


typedef std::pair<int, int> pos;

enum DIRECT{LEFT,RIGHT,UP,DOWN};
class Map;
class Snake
{
public:
	friend class Map;
	Snake(size_t len);
	pos move(DIRECT direct);
	bool isCurrPosLegal();
	std::list<pos> body;
	size_t length;
};


bool Snake::isCurrPosLegal()
{
	auto head = body.back();
	for (auto iter = body.begin(); iter != --body.end(); ++iter)
	{
		if (*iter == head)
			return false;
	}
	return true;
}

Snake::Snake(size_t len) :length(len)
{
	for (auto i = 0;i<len; ++i)
		body.push_back(std::make_pair(i, 0));
}
pos Snake::move(DIRECT direct)
{
	pos new_head=body.back();
	switch (direct)
	{
	case LEFT:
		new_head.first -= 1;
		break;
	case RIGHT:
		new_head.first += 1;
		break;
	case UP:
		new_head.second -= 1;
		break;
	case DOWN:
		new_head.second += 1;
		break;
	}
	body.push_back(new_head);
	pos tmp = body.front();
	body.pop_front();
	return tmp;
}

#endif
