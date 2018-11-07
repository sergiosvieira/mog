#ifndef __TATICAL_MOVING_OBJECT_H__
#define __TATICAL_MOVING_OBJECT_H__

#include <map>
#include <vector>
#include <qpoint.h>
#include <QString>
#include <vector>
#include <set>
#include "vector.h"
#include "coordinates.h"
#include "objectcategory.h"
#include "graphicsviewtype.h"

/*!
 * \brief The MovingPattern enum
 */
enum class MovingPattern
{
    Const_Velocity,
    Const_Acceleration,
    Const_Deceleration
};

/*!
 * \brief The Pattern struct
 */
struct Pattern
{
    static MovingPattern patternFromString(const std::string& str)
    {
        MovingPattern result = MovingPattern::Const_Velocity;
        if (str == "Constant Acceleration")
        {
            result = MovingPattern::Const_Acceleration;
        }
        else if (str == "Constant Deceleration")
        {
            result = MovingPattern::Const_Deceleration;
        }
        return result;
    }

    static std::string stringFromPattern(MovingPattern type)
    {
        if (type == MovingPattern::Const_Acceleration)
        {
            return "Constant Acceleration";
        }
        else if (type == MovingPattern::Const_Deceleration)
        {
            return "Constant Deceleration";
        }
        return "Constant Velocity";
    }
};

/*!
 * \brief map of air objects
 */
static const std::set<ObjectCategory> airObjectsType = {
    ObjectCategory::PassengerAirPlane,
    ObjectCategory::ExportationAirPlane,
    ObjectCategory::Helicopter,
    ObjectCategory::Missile,
    ObjectCategory::Cargo,
    ObjectCategory::Fighter
};

/*!
 * \brief map of land objects
 */
static const std::set<ObjectCategory> landObjectsType = {
    ObjectCategory::Battletank,
    ObjectCategory::Vehicle,
    ObjectCategory::Infantry
};

/*!
 * \brief map of water objects
 */

static const std::set<ObjectCategory> waterObjectsType = {
    ObjectCategory::FishingShip,
    ObjectCategory::NavalShip
};

/*!
 * \brief map of underwater objects
 */
static const std::set<ObjectCategory> underwaterObjectsType = {
    ObjectCategory::Submarine,
    ObjectCategory::UnderwaterRobot
};


using MapOfPatternToInstant = std::map<int, MovingPattern>;
/*!
 * \brief The PatternRecord struct
 */
struct PatternRecord
{
    int instant = 0;
    MovingPattern pattern = MovingPattern::Const_Velocity;
};

class CollectedEachInterface;

/*!
 * \brief The TaticalMovingObject class
 */
class TaticalMovingObject
{
public:
    using MapOfObject = std::map<unsigned int, TaticalMovingObject*>;
private:    
    /*!
     * \brief tatical moving object's name
     */
    QString name;
    CollectedEachInterface *collected = nullptr;
protected:
    /*!
     * \brief id
     */
    unsigned int id = 0;
    /*!
     * \brief position
     */
    Coordinates position;
    /*!
     * \brief velocity
     */
    Vector velocity;
    /*!
     * \brief initialTime
     */
    unsigned int initialTime = 0;
    /*!
     * \brief endTime
     */
    unsigned int endTime = 0;
    /*!
     * \brief acceleration
     */
    Vector acceleration;
    /*!
     * \brief direction
     */
    Vector direction;
    /*!
     * \brief type
     */
    ObjectCategory type;
    /*!
     * \brief patterns
     */
    MapOfPatternToInstant patterns;
    /*!
     * \brief lastPattern
     */
    MovingPattern lastPattern;
    /*!
     * \brief areaType
     */
    GraphicsViewType areaType;
public:
    /*!
     * \brief setAreaType
     * \param type
     */
    void setAreaType(GraphicsViewType type);
    /*!
     * \brief getAreaType
     * \return
     */
    GraphicsViewType getAreaType();
    /*!
     * \brief typeFromString
     * \param str
     * \return
     */
    static ObjectCategory typeFromString(const std::string &str);
    /*!
     * \brief stringFromType
     * \param type
     * \return
     */
    static std::string stringFromType(ObjectCategory type);
    /*!
     * \brief TaticalMovingObject
     */
    TaticalMovingObject();
    /*!
     * \brief TaticalMovingObject
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     * \param type
     */
    TaticalMovingObject(const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector& acceleration,
        ObjectCategory type,
        CollectedEachInterface *collected = nullptr);
    virtual ~TaticalMovingObject();
    void setCollected(CollectedEachInterface *collected);
    const CollectedEachInterface *getCollected() const;
    /*!
     * \brief getPosition
     * \return
     */
    Coordinates getPosition() const;
    /*!
     * \brief getVelocity
     * \return
     */
    Vector getVelocity() const;
    /*!
     * \brief getType
     * \return
     */
    ObjectCategory getType() const;
    /*!
     * \brief setType
     * \param type
     */
    void setType(ObjectCategory type)
    {
        this->type = type;
    }
    /*!
     * \brief getAcceleraton
     * \return
     */
    Vector getAcceleraton() const;
    /*!
     * \brief setDirection
     * \param u
     */
    void setDirection(const Vector& u);
    /*!
     * \brief getDirection
     * \return
     */
    Vector getDirection() const;
    /*!
     * \brief getID
     * \return
     */
    unsigned int getID() const;
    /*!
     * \brief getInitialTime
     * \return
     */
    unsigned int getInitialTime() const;
    /*!
     * \brief getEndTime
     * \return
     */
    unsigned int getEndTime() const;
    /*!
     * \brief setID
     * \param id
     */
    void setID(unsigned int id);
    /*!
     * \brief addPattern
     * \param pattern
     * \param time
     */
    void addPattern(const MovingPattern& pattern, int time);
    /*!
     * \brief getPattern
     * \param time
     * \return
     */
    MovingPattern getPattern(int time);
    /*!
     * \brief getAllPatternTime
     * \return
     */
    std::vector<int> getAllPatternTime();
    /*!
     * \brief setName
     * \param name
     */
    void setName(const QString &name)
    {
        this->name = name;
    }
    /*!
     * \brief getName
     * \return
     */
    QString getName() const
    {
        return this->name;
    }
};

#endif // __TATICAL_MOVING_OBJECT_H__
