#ifndef _MAP_H
#define _MAP_H
#include "snake.h"
#include<windows.h>
#include<random>
typedef pos pixel_pos;

class Map
{
public:
	Map(size_t x_len,size_t y_len,size_t step); //屏幕像素长宽，地图大小，
	RECT genRECT();
	void randFood();
	void genPane(HDC hdc); //产生方格线
	void drawMap(HDC hdc);
	bool moveSnakeOneStep(){ forward_tail = snake.move(direct); if (snake.body.back() == food){ snake.body.push_front(forward_tail); randFood(); }return isCurrPosLegal(); }
	void changeDirect(DIRECT direct){ this->direct = direct; }
	void gameAI();
	void setHWND(HWND hwnd){ this->hwnd = hwnd; }
	void clearMap(){ InvalidateRect(hwnd, NULL, FALSE); }
	void gameOver(){ MessageBox(NULL, TEXT("游戏结束"), TEXT("Snake"), MB_ICONERROR); exit(0); }
	void genmatrix();
private:
	bool isCurrPosLegal();
	bool findWay(pos beg, pos end);
	void drawSquare(HDC hdc,pixel_pos p, int is_head);
	pos genNewPos(DIRECT dic)
	{
		pos new_head = snake.body.back();
		switch (dic)
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
		return new_head;
	}
	pos genNewPos(DIRECT dic, pos new_head)
	{

		switch (dic)
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
		return new_head;
	}
	bool isLegalPos(pos p)
	{
		if (p.first<0 || p.first>x_len - 1 || p.second<0 || p.second>y_len - 1) return false;
		for (auto iter = snake.body.begin(); iter != --snake.body.end(); ++iter)
		{
			if (*iter == p)
				return false;
		}
		return true;
	}
	pos getHead(){ return snake.body.back(); }
	pixel_pos genPixelPos(pos p);
	size_t x_len, y_len, step;
	Snake snake;
	pos food;
	DIRECT direct;
	HWND hwnd;
	pos forward_tail;
	bool** matrix;
};



void Map::genmatrix()
{
	if (matrix)
	{
		for (int i = 0; i < y_len; ++i)
			for (int j = 0; j < x_len; ++j)
				matrix[i][j] = false;
		for (auto body : snake.body)
			matrix[body.second][body.first] = true;
		auto tail = snake.body.front();
		matrix[tail.second][tail.first] = false;
	}
	else //初始化坐标矩阵
	{
		matrix = new bool*[y_len];
		for (int i = 0; i < y_len; ++i)
			matrix[i] = new bool[x_len];
		for (int i = 0; i < y_len; ++i)
			for (int j = 0; j < x_len; ++j)
				matrix[i][j] = false;
		for (auto body : snake.body)
			matrix[body.second][body.first] = true;
		auto tail = snake.body.front();
		matrix[tail.second][tail.first] = false;
	}
}
bool Map::findWay(pos beg, pos end)
{
	if (beg == end)
		return true;
	bool result = false;
	matrix[beg.second][beg.first] = true;
	if (beg.first - 1 >= 0 && matrix[beg.second][beg.first - 1] == false)
	{
		auto new_beg = beg;
		new_beg.first--;
		result= findWay(new_beg, end);
		if (result)
			return true;
	}
	if (beg.first + 1 < x_len && matrix[beg.second][beg.first + 1] == false)
	{
		auto new_beg = beg;
		new_beg.first++;
		result=findWay(new_beg, end);
		if (result)
			return true;
	}if (beg.second - 1 >= 0 && matrix[beg.second - 1][beg.first] == false)
	{
		auto new_beg = beg;
		new_beg.second--;
		result =  findWay(new_beg, end);
		if (result)
			return true;
	}if (beg.second + 1 < y_len && matrix[beg.second + 1][beg.first] == false)
	{
		auto new_beg = beg;
		new_beg.second++;
		result = findWay(new_beg, end);
		if (result)
			return true;
	}
	return result;
}
void Map::gameAI()
{
	std::random_device rd;
	static int loop_lock = 0;
	int st = rd()%10+5;
	auto direct_bak = direct;
	if (food.second > getHead().second)
	{
		if (food.first > getHead().first)
		{
			if (loop_lock<=st)
			{
				if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				loop_lock=(loop_lock+1)%20;
			}
			else
			{
				if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				loop_lock=(loop_lock+1)%20;
			}
		}
		else if (food.first < getHead().first)
		{
			if (loop_lock<=st)
			{
				if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				loop_lock=(loop_lock+1)%20;
			}
			else
			{
				if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				loop_lock=(loop_lock+1)%20;
			}
		}
		else
		{
			if (isLegalPos(genNewPos(DOWN)))
				direct = DOWN;
			else if (isLegalPos(genNewPos(LEFT)))
				direct = LEFT;
			else if (isLegalPos(genNewPos(RIGHT)))
				direct = RIGHT;
			else if (isLegalPos(genNewPos(UP)))
				direct = UP;
		}
	}
	else if (food.second < getHead().second)
	{
		if (food.first > getHead().first)
		{
			if (loop_lock<=st)
			{
				if (isLegalPos(genNewPos(UP)))
					direct = UP;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				loop_lock=(loop_lock+1)%20;
			}
			else
			{
				if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				loop_lock=(loop_lock+1)%20;
			}
		}
		else if (food.first < getHead().first)
		{
			if (loop_lock<=st)
			{
				if (isLegalPos(genNewPos(UP)))
					direct = UP;
				else if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				loop_lock=(loop_lock+1)%20;
			}
			else
			{
				if (isLegalPos(genNewPos(LEFT)))
					direct = LEFT;
				else if (isLegalPos(genNewPos(UP)))
					direct = UP;
				else if (isLegalPos(genNewPos(RIGHT)))
					direct = RIGHT;
				else if (isLegalPos(genNewPos(DOWN)))
					direct = DOWN;
				loop_lock=(loop_lock+1)%20;
			}
		}
		else
		{
			if (isLegalPos(genNewPos(UP)))
				direct = UP;
			else if (isLegalPos(genNewPos(LEFT)))
				direct = LEFT;
			else if (isLegalPos(genNewPos(RIGHT)))
				direct = RIGHT;
			else if (isLegalPos(genNewPos(DOWN)))
				direct = DOWN;
		}
	}
	else
	{
		if (food.first > getHead().first)
		{
			if (isLegalPos(genNewPos(RIGHT)))
				direct = RIGHT;
			else if (isLegalPos(genNewPos(LEFT)))
				direct = LEFT;
			else if (isLegalPos(genNewPos(UP)))
				direct = UP;

			else if (isLegalPos(genNewPos(DOWN)))
				direct = DOWN;
		}
		else if (food.first < getHead().first)
		{
			if (isLegalPos(genNewPos(LEFT)))
				direct = LEFT;
			else if (isLegalPos(genNewPos(RIGHT)))
				direct = RIGHT;
			else if (isLegalPos(genNewPos(UP)))
				direct = UP;
			else if (isLegalPos(genNewPos(DOWN)))
				direct = DOWN;
		}
		else
		{

		}
	}
	/*
	if (direct == RIGHT || direct == LEFT)
	{
	if (food.second - getHead().second > 0)
	{
	if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	else if (isLegalPos(genNewPos(UP)))
	direct = UP;

	}
	else if (food.second - getHead().second < 0)
	{
	if (isLegalPos(genNewPos(UP)))
	direct = UP;
	else if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	}
	else
	{
	if (direct == RIGHT&&food.first - getHead().first < 0)
	{
	if (isLegalPos(genNewPos(UP)))
	direct = UP;
	else if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	}
	else if (direct == LEFT&&food.first - getHead().first > 0)
	{
	if (isLegalPos(genNewPos(UP)))
	direct = UP;
	else if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	}
	else
	{
	if (isLegalPos(genNewPos(direct)))
	direct;
	else if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	else if (isLegalPos(genNewPos(UP)))
	direct = UP;
	}
	}
	}
	else if (direct == UP || direct == DOWN)
	{
	if (food.first - getHead().first > 0)
	{
	if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	else if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	}
	else if (food.first - getHead().first < 0)
	{
	if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	else if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	}
	else
	{
	if (direct == DOWN&&food.second - getHead().second < 0)
	{
	if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	else if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	}
	else if (direct == UP&&food.second - getHead().second > 0)
	{
	if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	else if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	}
	else
	{
	if (isLegalPos(genNewPos(direct)))
	direct;
	else if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	else if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	}
	}
	}
	*/
	//是否为环


	pos new_pos = genNewPos(direct);
	if (!findWay(new_pos, snake.body.front()))
	{
		if (direct_bak == LEFT || direct_bak == RIGHT)
		{
			if (isLegalPos(genNewPos(UP))&&findWay(genNewPos(UP), snake.body.front()) )
				direct = UP;
			else if (isLegalPos(genNewPos(DOWN))&&findWay(genNewPos(DOWN), snake.body.front()))
				direct = DOWN;
			else if ( isLegalPos(genNewPos(LEFT))&&findWay(genNewPos(LEFT), snake.body.front()))
				direct = LEFT;
			else if (isLegalPos(genNewPos(RIGHT)))
				direct = RIGHT;
		}
		else
		{
			if (isLegalPos(genNewPos(LEFT)) && findWay(genNewPos(LEFT), snake.body.front()))
				direct = LEFT;
			else if (isLegalPos(genNewPos(RIGHT)) && findWay(genNewPos(RIGHT), snake.body.front()))
				direct = RIGHT;
			else if (isLegalPos(genNewPos(UP)) && findWay(genNewPos(UP), snake.body.front()))
				direct = UP;
			else if (isLegalPos(genNewPos(DOWN)))
				direct = DOWN;
		}

	}
	else
	{
	
	}


	/*
	auto isInBody = [&](const pos& p)
	{
	return std::find(snake.body.begin(), snake.body.end(), p) != snake.body.end();
	};

	pos new_pos = genNewPos(direct);
	pos head = getHead();
	if (direct == LEFT || direct == RIGHT)
	{
	head.first += 1;
	if (!isLegalPos(head))
	{
	head.first -= 2;
	if (!isLegalPos(head))
	{

	auto iter = ++snake.body.begin();
	while ((*iter).second == snake.body.begin()->second)
	++iter;
	if (iter->second > snake.body.begin()->second)
	direct = UP;
	else
	direct = DOWN;
	}
	}
	new_pos.second += 1;
	if (!isLegalPos(new_pos))
	{
	new_pos.second -= 2;
	if (!isLegalPos(new_pos))
	{
	if (direct == UP || direct == DOWN)
	{

	}
	else
	{
	if (isLegalPos(genNewPos(UP)))
	direct = UP;
	else if (isLegalPos(genNewPos(DOWN)))
	direct = DOWN;
	}
	}
	}
	}
	else
	{
	head.second += 1;
	if (!isLegalPos(head))
	{
	head.second -= 2;
	if (!isLegalPos(head))
	{

	auto iter = ++snake.body.begin();
	while ((*iter).first == snake.body.begin()->first)
	++iter;
	if (iter->first > snake.body.begin()->first)
	direct = LEFT;
	else
	direct = RIGHT;
	}
	}
	new_pos.first += 1;
	if (!isLegalPos(new_pos))
	{
	new_pos.first -= 2;
	if (!isLegalPos(new_pos))
	{
	if (direct == LEFT || direct == RIGHT)
	{

	}
	else
	{
	if (isLegalPos(genNewPos(LEFT)))
	direct = LEFT;
	else if (isLegalPos(genNewPos(RIGHT)))
	direct = RIGHT;
	}
	}
	}
	}
	*/
}
void Map::randFood()
{
	std::random_device rd;
	food = std::make_pair(rd() % x_len, rd() % y_len);
	if (std::find(snake.body.begin(), snake.body.end(), food) != snake.body.end())
		randFood();
}
bool Map::isCurrPosLegal()
{
	pos head = snake.body.back();
	if (head.first<0 || head.first>x_len - 1 || head.second<0 || head.second>y_len - 1) return false;
	return snake.isCurrPosLegal();
}
pixel_pos Map::genPixelPos(pos p)
{
	return std::make_pair(p.first*step, p.second*step);
}
void Map::drawSquare(HDC hdc, pixel_pos p, int is_head = 0)
{
	static HBRUSH brush;
	if (is_head==0)
		brush = CreateSolidBrush(RGB(50, 205, 20));
	else if (is_head==1)
		brush = CreateSolidBrush(RGB(100, 50, 20));
	else
		brush = CreateSolidBrush(RGB(255, 255, 255));
	
	SelectObject(hdc, brush);
	//SelectObject(hdc, GetStockObject(NULL_PEN));
	Rectangle(hdc, p.first, p.second, p.first + step+1, p.second + step+1);
	DeleteObject(brush);

}
void Map::drawMap(HDC hdc)
{
	// draw snake
	for (auto iter = snake.body.begin(); iter != --snake.body.end(); ++iter)
	{
		drawSquare(hdc, genPixelPos(*iter));
	}
	drawSquare(hdc, genPixelPos(*(snake.body.rbegin())), 1);

	//draw food

	drawSquare(hdc, genPixelPos(food), 1);

	drawSquare(hdc, genPixelPos(forward_tail), 2);

}

RECT Map::genRECT()
{
	RECT rect;
	rect.top = rect.left = 0;
	rect.bottom = step*y_len;
	rect.right = step*x_len;
	return rect;
}

Map::Map(size_t x_len, size_t y_len, size_t step)
	:snake(3), step(step), x_len(x_len), y_len(y_len), matrix(nullptr)
{

}

void Map::genPane(HDC hdc)
{
	size_t x_pixel = x_len*step;
	size_t y_pixel = y_len*step;
	for (size_t x = 0; x <= x_pixel; x += step)
	{
		MoveToEx(hdc, x, 0, NULL);
		LineTo(hdc, x, y_pixel);
	}
	for (size_t y = 0; y <= y_pixel; y += step)
	{
		MoveToEx(hdc, 0, y, NULL);
		LineTo(hdc, x_pixel, y);
	}
}

#endif