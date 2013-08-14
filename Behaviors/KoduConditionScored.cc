#include "Kodu/Behaviors/KoduConditionScored.h"

namespace Kodu {

    bool KoduConditionScored::evaluate() {
        // boolean return value
        bool rv = false;
        // get the latest score
        int score = ScoreKeeper::checkScoreValue(designator);
        if (numericsWereSpecified) {
            // get a (constant/random) value from the numeric generator
            int value = numericGen.getNumericValue();
            // check the inequality type
            // equals
            if (type == CT_EQUALS && score == value) {
                rv = true;
            }
            // not equals
            else if (type == CT_NOT_EQUALS && score != value) {
                rv = true;
            }
            // greater than or equal to
            else if (type == CT_GT_EQUAL && score >= value) {
                rv = true;
            }
            // lesser than or equal to
            else if (type == CT_LT_EQUAL && score <= value) {
                rv = true;
            }
            // above
            else if (type == CT_ABOVE && score > value) {
                rv = true;
            }
            // below
            else if (type == CT_BELOW && score < value) {
                rv = true;
            }
        }
        // numerics were not specified
        else {
            if (score != lastRecordedValue) {
                lastRecordedValue = score;
                rv = true;
            }
        }
        // check if the "not" modifier is enabled
        if (notEnabled)
            rv = !rv;
        return rv;
    }
    
    void KoduConditionScored::reinitialize() {
        KoduCondition::reinitialize();
    }

    void KoduConditionScored::printAttrs() {
        KoduCondition::printAttrs();
        PRINT_ATTRS("Not enabled", notEnabled);
        PRINT_ATTRS("Numerics were specified", numericsWereSpecified);
        std::cout << "Inequality: ";
        switch (type) {
            case CT_EQUALS:
                std::cout << "equals";
                break;

            case CT_NOT_EQUALS:
                std::cout << "not equals";
                break;

            case CT_GT_EQUAL:
                std::cout << "greater than or equal to (>=)";
                break;
            
            case CT_LT_EQUAL:
                std::cout << "lesser than or equal to (<=)";
                break;

            case CT_ABOVE:
                std::cout << "above";
                break;

            case CT_BELOW:
                std::cout << "below";
                break;
        }
        std::cout << std::endl;
        numericGen.printAttrs();
        std::cout << "Scored designator: " << designator << std::endl;
    }
} // end of Kodu namespace