#ifndef KODU_CONDITION_TIMER_H_
#define KODU_CONDITION_TIMER_H_

// C++ Library
#include <iostream>

// Kodu Library
#include "Kodu/Behaviors/KoduCondition.h"
#include "Kodu/General/GeneralMacros.h"
#include "Kodu/Generators/KoduGenerators.h"

/**
 * NAME:            KODU CONDITION TIMER
 * TYPE:            CONDITION
 * KODU EQUIVALENT: TIMER
 * DESCRIPTION:
 *      
**/

namespace Kodu {

    class KoduConditionTimer : public KoduCondition {
    public: 
        //! Constructor
        KoduConditionTimer(bool useNot, const NumericGenerator& kNumericGen)
          : KoduCondition("KoduConditionTimer"),
            numericGen(kNumericGen),
            timerThreshold(0.0f),
            notEnabled(useNot),
            needToInitialize(true),
            currentTime(0),
            startTime(0)
        { }
        
        //! Copy constructor
        KoduConditionTimer(const KoduConditionTimer& kCondition)
          : KoduCondition(kCondition),
            numericGen(kCondition.numericGen),
            timerThreshold(kCondition.timerThreshold),
            notEnabled(kCondition.notEnabled),
            needToInitialize(kCondition.needToInitialize),
            currentTime(kCondition.currentTime),
            startTime(kCondition.startTime)
        { }

        //! Destructor
        ~KoduConditionTimer() {
            // no explicit implementation
        }

        //! Assignment operator
        KoduConditionTimer& operator=(const KoduConditionTimer& kCondition) {
            if (this != &kCondition) {
                KoduCondition::operator=(kCondition);
                numericGen = kCondition.numericGen;
                timerThreshold = kCondition.timerThreshold;
                notEnabled = kCondition.notEnabled;
                needToInitialize = kCondition.needToInitialize;
                currentTime = kCondition.currentTime;
                startTime = kCondition.startTime;
            }
            return *this;
        }
        
        //! Checks if the timer threshold has been reached or exceeded
        virtual bool evaluate();
        
        //! Used to reinitialize certain variables each condition and action type
        virtual void reinitialize();
        
        //! Prints the attributes of a particular instance
        virtual void printAttrs();

        unsigned int getTimeInSec();

    private:
        NumericGenerator numericGen;    //!< returns a constant or random timer value
        float timerThreshold;           //!< amount of time needed to past
        bool notEnabled;                //!< states if the not modifier is in use
        bool needToInitialize;          //!< used to initialize start time
        unsigned int currentTime;       //!< most recent time request
        unsigned int startTime;         //!< used for time elapsed calculation
    };
}

#endif // KODU_CONDITION_TIMER_H_