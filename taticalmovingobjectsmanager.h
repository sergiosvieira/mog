#ifndef __TATICAL_MOVING_OBJECTS_MANAGER_H__
#define __TATICAL_MOVING_OBJECTS_MANAGER_H__

#include <map>
#include <QRect>
#include "objectcategory.h"
#include "distributiontype.h"

class TaticalMovingObject;
class QImage;
struct CollectedEachInterface;

/*!
 * \brief The TaticalMovingObjectsManager class
 */
class TaticalMovingObjectsManager
{
public:
    using Map = std::map<unsigned int, TaticalMovingObject*>;
protected:
    static const QRect World;
    Map objMap;
    unsigned int endInstant = 0;
public:
    const Map &getObjects()
    {
        return this->objMap;
    }
    /*!
     * \brief TaticalMovingObjectsManager
     */
    TaticalMovingObjectsManager();
    /*!
     * \brief ~TaticalMovingObjectsManager
     */
    virtual ~TaticalMovingObjectsManager();
    /*!
     * \brief Adds an object
     * \param category
     * \param distribution
     * \param initialInstant
     * \param lifeTime
     */
    virtual void add(ObjectCategory category,
                     DistributionType distribution,
                     unsigned int initialInstant,
                     unsigned int lifeTime,
                     CollectedEachInterface *collected = nullptr) = 0;
    /*!
     * \brief del specified object
     * \param id
     */
    virtual void del(unsigned int id) = 0;
    /*!
     * \brief draw an object on canvas
     * \param id
     * \param image
     */
    virtual void show(unsigned int id, QImage *image) = 0;
    /*!
     * \brief updates object position
     * \param objects
     */
    virtual void update(unsigned int id) = 0;
    /*!
     * \brief String of object's properties (csv file format)
     */
    virtual std::string str() = 0;
    /*!
     * \brief get maximum instant of all objects created
     * \return
     */
    unsigned int getEndInstant() const
    {
        return this->endInstant;
    }
};

#endif // __TATICAL_MOVING_OBJECTS_MANAGER_H__
