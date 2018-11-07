#ifndef COLLECTEDEACH_H
#define COLLECTEDEACH_H

#include "enum.h"

/*!
 * \brief The CollectedEach enum
 */
enum class CollectedEach
{
    FiveSeconds,
    OneSecond
};

/*!
 * \brief The CollectedEachInterface struct
 */
struct CollectedEachInterface: public Enum<CollectedEach>
{
    /*!
     * \brief getStep
     * \return
     */
    unsigned int getStep() const
    {
        return this->step;
    }
protected:
    /*!
     * \brief step
     */
    unsigned int step = 1;
};

/*!
 * \brief The FiveSeconds struct
 */
struct FiveSeconds: public CollectedEachInterface
{
    /*!
     * \brief FiveSeconds Constructor
     */
    FiveSeconds()
    {
        this->type = CollectedEach::FiveSeconds;
        this->value = "5 Seconds";
        this->step = 5;
    }
};

/*!
 * \brief The OneSecond struct
 */
struct OneSecond: public CollectedEachInterface
{
    /*!
     * \brief OneSecond constructor
     */
    OneSecond()
    {
        this->type = CollectedEach::OneSecond;
        this->value = "1 Second";
        this->step = 1;
    }
};

#endif // COLLECTEDEACH_H
