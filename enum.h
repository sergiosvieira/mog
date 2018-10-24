#ifndef ENUM_H
#define ENUM_H

#include <string>

/*!
 * Enum base interface
 */
template <typename T>
struct Enum
{
    Enum(){}
    Enum(const T& type,
         std::string value):
        type(type), value(value){}
    const T& getType() const
    {
        return this->type;
    }
    const std::string& getValue() const
    {
        return this->value;
    }
    const std::string& getCategory() const
    {
        return this->category;
    }
protected:
    T type;
    std::string value;
    std::string category;
};

#endif // ENUM_H
