#ifndef LANDOBJECTSMANAGER_H
#define LANDOBJECTSMANAGER_H


#include "taticalmovingobjectsmanager.h"

/*!
 * \brief The LandObjectsManager class
 */
class LandObjectsManager: public TaticalMovingObjectsManager
{
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
     * \brief del a specified land object
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
     * \brief String of land object's properties (csv file format)
     * \return
     */
    virtual std::string str() override;
};


#endif // LANDOBJECTSMANAGER_H
