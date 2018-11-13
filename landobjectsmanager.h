#ifndef LANDOBJECTSMANAGER_H
#define LANDOBJECTSMANAGER_H


#include "taticalmovingobjectsmanager.h"

/*!
 * \brief The LandObjectsManager class
 */
class LandObjectsManager: public TaticalMovingObjectsManager
{
protected:
    void draw(QPainter& painter) override;
public:
    /*!
     * \brief adds a land object
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
     * \brief String of land object's properties (csv file format)
     * \return
     */
    virtual std::string str() override;
};


#endif // LANDOBJECTSMANAGER_H
