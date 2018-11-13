#ifndef __AIR_OBJECTS_MANAGER_H
#define __AIR_OBJECTS_MANAGER_H

#include "taticalmovingobjectsmanager.h"

/*!
 * \brief The AirObjectsManager class
 */
class AirObjectsManager: public TaticalMovingObjectsManager
{
protected:
    void draw(QPainter& painter) override;
public:
    /*!
     * \brief add an air object
     * \param category
     * \param distribution
     * \param initialInstant
     * \param lifeTime
     */
    void add(ObjectCategory category,
             DistributionType distribution,
             unsigned int initialInstant,
             unsigned int lifeTime,
             CollectedEachInterface *collected = nullptr) override;
    /*!
     * \brief String of air object's properties (csv file format)
     */
    virtual std::string str() override;
};

#endif /* __AIR_OBJECTS_MANAGER_H */
