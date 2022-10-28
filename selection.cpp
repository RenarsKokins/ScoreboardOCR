#include "selection.h"

Selection::Selection()
{

}

QString Selection::getName()
{
    return name;
}

SelectionType Selection::getType()
{
    return type;
}

QRect Selection::getSelectionPos()
{
    return rect;
}

void Selection::setName(QString name)
{
    this->name = name;
}

void Selection::setType(SelectionType type)
{
    this->type = type;
}

void Selection::setSelectionPos(int x, int y, int w, int h)
{
    rect.setX(x);
    rect.setY(y);
    rect.setWidth(w);
    rect.setHeight(h);
}

void Selection::addNumber(cv::Mat img)
{
    numbers.append(img);
}

void Selection::clearNumbers()
{
    numbers.clear();
}

QVector<cv::Mat> *Selection::getNumbers()
{
    return &numbers;
}
