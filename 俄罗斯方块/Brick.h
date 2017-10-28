#pragma once

#include <stdlib.h>

#include "defines.h"
#include "Map.h"

class Brick
{
public:
	Brick(const Map &map)
		: m_map(map)
	{
		m_type = (rand() % BRICK_TYPES_NUM);
		m_rotate = rand() % 4;
		m_data[0] = g_iBrickPool[m_type][m_rotate] >> 24 & 0x000000FF;
		m_data[1] = g_iBrickPool[m_type][m_rotate] >> 16 & 0x000000FF;
		m_data[2] = g_iBrickPool[m_type][m_rotate] >> 8 & 0x000000FF;
		m_data[3] = g_iBrickPool[m_type][m_rotate] & 0x000000FF;
		m_axisRow = 0;
		m_axisCol = GAME_COL / 2;
	}
	~Brick() {}

	int operator[](size_t index) const
	{
		return m_data[index];
	}

	Brick& operator=(const Brick& other)
	{
		m_data[0] = other.m_data[0];
		m_data[1] = other.m_data[1];
		m_data[2] = other.m_data[2];
		m_data[3] = other.m_data[3];
		m_type = other.m_type;
		m_rotate = other.m_rotate;
		m_axisRow = other.m_axisRow;
		m_axisCol = other.m_axisCol;
		return *this;
	}

	int getAxisRow() const
	{
		return m_axisRow;
	}

	int getAxisCol() const
	{
		return m_axisCol;
	}

	int TryRotate()
	{
		Brick tmpBrick(*this);
		int i = 0;

		m_rotate = (m_rotate + 1) % 4;
		m_data[0] = g_iBrickPool[m_type][m_rotate] >> 24 & 0x000000FF;
		m_data[1] = g_iBrickPool[m_type][m_rotate] >> 16 & 0x000000FF;
		m_data[2] = g_iBrickPool[m_type][m_rotate] >> 8 & 0x000000FF;
		m_data[3] = g_iBrickPool[m_type][m_rotate] & 0x000000FF;

		for (i = 0; i < 4; i++)
		{
			if (m_map.getRow(m_axisRow + i)
				& (m_data[i] << (24 - m_axisCol)))
			{
				*this = tmpBrick;
				return 1;
			}
		}
		return 0;
	}

	int TryMove(int x, int y)
	{
		Brick tmpBrick(*this);
		int i = 0;

		m_axisCol += x;
		m_axisRow += y;

		for (i = 0; i < 4; i++)
		{
			if (m_map.getRow(m_axisRow + i)
				& (m_data[i] << (24 - m_axisCol)))
			{
				*this = tmpBrick;
				return 1;
			}
		}

		return 0;
	}
private:
	int m_data[4];
	int m_type;
	int m_rotate;
	int m_axisRow;
	int m_axisCol;
	const Map& m_map;
};

