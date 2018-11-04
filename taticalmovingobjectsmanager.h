#ifndef __TATICAL_MOVING_OBJECTS_MANAGER_H__
#define __TATICAL_MOVING_OBJECTS_MANAGER_H__

#include <map>
#include <QRect>
#include "objectcategory.h"
#include "distributiontype.h"

class TaticalMovingObject;
class QImage;

/*!
 * \brief The TaticalMovingObjectsManager class
 */
class TaticalMovingObjectsManager
{
protected:
    static const QRect World;
    std::map<unsigned int, TaticalMovingObject*> objMap;
public:
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
                     unsigned int lifeTime) = 0;
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
};

#endif // __TATICAL_MOVING_OBJECTS_MANAGER_H__
