#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <map>
#include <utility>
#include "vector.h"
#include "coordinates.h"

class TaticalMovingObject;
// Coordinates, Velocity
using Pair = std::pair<Coordinates, Vector>;
using Data = std::map<int, std::map<int, Pair>>;

/*!
 * \brief The DataManager class
 */
class DataManager
{
protected:
    /*!
     * \brief data
     */
    Data data;
public:
    /*!
     * \brief DataManager
     */
    DataManager();
    /*!
     * \brief setData
     * \param instant
     * \param objectID
     * \param pair_
     */
    void setData(int instant, int objectID, const Pair& pair_);
    /*!
     * \brief getData
     * \param instant
     * \param objectID
     * \return
     */
    Pair getData(int instant, int objectID);
};


#endif // DATAMANAGER_H
