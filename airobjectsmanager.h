#ifndef __AIR_OBJECTS_MANAGER_H
#define __AIR_OBJECTS_MANAGER_H

#include "taticalmovingobjectsmanager.h"

/*!
 * \brief The AirObjectsManager class
 */
class AirObjectsManager: public TaticalMovingObjectsManager
{
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
     * \brief del a specified air object
     * \param id
     */
    void del(unsigned int id);
    /*!
     * \brief show
     * \param id
     */
    void show(unsigned int id, QImage *image);
    /*!
     * \brief update
     * \param id
     */
    void update(unsigned int id);
    /*!
     * \brief String of air object's properties (csv file format)
     */
    virtual std::string str() override;
};

#endif /* __AIR_OBJECTS_MANAGER_H */
