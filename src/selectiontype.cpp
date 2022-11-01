#include "selectiontype.h"

SelectionType::SelectionType()
{

}

SelectionType::Type SelectionType::getType()
{
    return type;
}

void SelectionType::setType(SelectionType::Type i)
{
    type = i;
}

void SelectionType::setType(short i)
{
    type = static_cast<Type>(i);
}

QStringList SelectionType::getTypeDescriptions()
{
    return descriptions;
}

QString SelectionType::getDescription()
{
    return descriptions.at(type);
}
