#include "axcell.h"

AxRange AxCell::range() const
{
    return m_range;
}

int AxCell::row() const
{
    return m_row;
}

int AxCell::col() const
{
    return m_col;
}

AxCell::AxCell(int row, int col)
{

    m_row = row;
    m_col = col;
}
