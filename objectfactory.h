#ifndef OBJECTTYPEFACTORY_H
#define OBJECTTYPEFACTORY_H


#include <functional>
#include <map>
#include "objectcategory.h"

class ObjectInfo;

/*!
 * \brief The ObjectFactory class
 */
class ObjectFactory
{
    using FuncMap = std::map<ObjectCategory, std::function<ObjectInfo *()>>;
    /*!
     * \brief map_
     */
    static const FuncMap map_;
    /*!
     * \brief instantiate a object
     */
    template<class T>
    static T *makeObject()
    {
        return new T();
    }
public:
    /*!
     * \brief make
     * \param category
     * \return
     */
    static ObjectInfo *make(ObjectCategory category);
};

#endif // OBJECTTYPEFACTORY_H
