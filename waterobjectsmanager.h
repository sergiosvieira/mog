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
     * \brief str
     * \return
     */
    virtual std::string str() override;
protected:
    /*!
     * \brief draw
     * \param painter
     */
    void draw(QPainter &painter) override;
};


#endif // WATEROBJECTSMANAGER_H
