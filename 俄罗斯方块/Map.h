#pragma once

#include "defines.h"

class Map
{
public:
	Map()
	{
		for (int i = 0; i < GAME_ROW; i++)
			m_rows[i] = g_iInitBackGround[i];
	}
	~Map()
	{

	}
	unsigned int getRow(size_t index) const
	{
		return m_rows[index];
	}
	void changeRow(size_t index, int data)
	{
		m_rows[index] = data;
	}
	int TryDeleteLines()
	{
		int i = -1;
		for (i = GAME_ROW - 2; i > 0; i--)
		{
			if (m_rows[i] == 0xFFF00000)
			{
				int j = 0;
				for (j = i; j > 0; j--)
				{
					m_rows[j] = m_rows[j - 1];
				}
				m_rows[0] = g_iInitBackGround[0];
				i++;
			}
		}
		return 0;
	}


private:
	unsigned int m_rows[GAME_ROW];
};

