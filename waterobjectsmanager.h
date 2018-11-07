#ifndef WATEROBJECTSMANAGER_H
#define WATEROBJECTSMANAGER_H


#include "taticalmovingobjectsmanager.h"

/*!
 * \brief The WaterObjectsManager class
 */
class WaterObjectsManager: public TaticalMovingObjectsManager
{
public:
    /*!
     * \brief add an water object
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
     * \brief del a specified water object
     * \param id
     */
    void del(unsigned int id);
    /*!
     * \brief draw a water object on canvas
     * \param id
     */
    void show(unsigned int id, QImage *image);
    /*!
     * \brief updates a water object position
     * \param id
     */
    void update(unsigned int id);
    virtual std::string str() override;
};


#endif // WATEROBJECTSMANAGER_H
