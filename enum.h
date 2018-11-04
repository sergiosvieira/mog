#ifndef ENUM_H
#define ENUM_H

#include <string>

/*!
 * Enum base interface
 */
template <typename T>
struct Enum
{
    /*!
     * \brief Enum constructor
     */
    Enum(){}
    /*!
     * \brief Enum constructor
     * \param type
     * \param value
     */
    Enum(const T& type,
         std::string value):
        type(type), value(value){}
    /*!
     * \brief getType
     * \return
     */
    const T& getType() const
    {
        return this->type;
    }
    /*!
     * \brief getValue
     * \return
     */
    const std::string& getValue() const
    {
        return this->value;
    }
    /*!
     * \brief getCategory
     * \return
     */
    const std::string& getCategory() const
    {
        return this->category;
    }
protected:      
    T type;
    /*!
     * \brief value
     */
    std::string value;
    /*!
     * \brief category
     */
    std::string category;
};

#endif // ENUM_H
