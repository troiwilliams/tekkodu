// Kodu Library
#include "Kodu/Primitives/KoduConditionBump.h"
#include "Kodu/General/GeneralMacros.h"

namespace Kodu {

    const int KoduConditionBump::kMaxDistanceAwayToSenseBump = 275; // millimeters

    bool KoduConditionBump::evaluate() {
        bool rv = false;
        DualCoding::Shape<DualCoding::CylinderData> _refdObject;
        
        // get the closest object that matches what this condition is searching for
        _refdObject = getClosestObjectMatching(objColor, searchLocation);
        
        // If there is one valid remaining and it is within some distance to the agent,
        // then the robot will react to that object
        if (_refdObject.isValid()
            && calcDistanceFromAgentToObject(_refdObject) <= kMaxDistanceAwayToSenseBump)
        {
            std::cout << "Bumped a(n) " << getObjectColor() << " " << getObjectType() << "!\n";
            ObjectKeeper::tempObject = _refdObject;
            ObjectKeeper::isValid = true;
            rv = true;
        }
        
        if (notModifierEnabled)
            return (!rv);
        else
            return rv;
    }
/*
    bool KoduConditionBump::agentIsNearMatchingObject() {
        DualCoding::Shape<DualCoding::CylinderData> _refdObject;
        // get the closest object that matches what this condition is searching for
        _refdObject = getClosestObjectMatching(objColor, searchLocation);
        
        // If there is one valid remaining and it is within some distance to the agent,
        // then the robot will react to that object
        std::cout << "Checking if the agent is near a matching object...";
        if (_refdObject.isValid()
            && calcDistanceFromAgentToObject(_refdObject) <= kMaxDistanceAwayToSenseBump)
        {
            std::cout << "it's near!\n";
            refdObject = _refdObject;
        } else {
            std::cout << "nada.\n";
            refdObject = ObjectKeeper::invalidObject;
        }
    }
*/
    const std::string& KoduConditionBump::getObjectColor() const {
        return objColor;
    }

    const std::string& KoduConditionBump::getObjectType() const {
        return objType;
    }

    const DualCoding::Shape<DualCoding::CylinderData>& KoduConditionBump::getTargetObject() {
        return refdObject;
    }

    bool KoduConditionBump::isSameTypeAs(const KoduPrimitive* kPrimitive) {
        return (dynamic_cast<const KoduConditionBump*>(kPrimitive) != NULL);
    }

    void KoduConditionBump::reinitialize() {
        // TODO (9/Aug/2013)
        // do I need to clear the Shape object?
    }
    
    void KoduConditionBump::printAttrs() const {
        KoduCondition::printAttrs();
        // not enabled?
        PRINT_ATTRS("Not enabled", notModifierEnabled);
        // object color and type
        std::cout << "Object color and type: " << objColor << " " << objType << std::endl;
        // search region
        /*
        std::cout << "Search region:";
        if (searchRegion == SRG_UNRESTRICTED) {
            std::cout << " unrestricted\n";
        } else {
            if (searchRegion & SRG_TO_LEFT) {
                std::cout << " to_left";
            } else if (searchRegion & SRG_TO_RIGHT) {
                std::cout << " to_right";
            }

            if (searchRegion & SRG_IN_FRONT) {
                std::cout << " in_front";
            } else if (searchRegion & SRG_BEHIND) {
                std::cout << " behind";
            }
            std::cout << std::endl;
        }
        */
        // referenced object...
    }
}
