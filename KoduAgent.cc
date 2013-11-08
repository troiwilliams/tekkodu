// Tekkodu
#include "Kodu/KoduAgent.h"
#include "Kodu/Primitives/PerceptionSearch.h"
#include "Kodu/PerceptualTasks/PerceptualTaskBase.h"

// Tekkotsu
#include "DualCoding/ShapeTypes.h"
#include "Shared/get_time.h"
#include "Shared/mathutils.h"

namespace Kodu {
    /// ================================ Static initializations ================================ ///
    const float KoduAgent::kLocalizationDistanceThreshold = 1000.0f;

    /// ================================ Grasper functions ================================ ///
    bool KoduAgent::hasNewObjectToGrab() const {
        return (gripperObject.isValid() && !targetObjectIsInGripper && !agentIsAttemptingGrab);
    }

    bool KoduAgent::isAttemptingGrab() const {
        return agentIsAttemptingGrab;
    }

    bool KoduAgent::isHoldingAnObject() const {
        return targetObjectIsInGripper;
    }

    void KoduAgent::setIsAttemptingGrabFlag(bool bval) {
        agentIsAttemptingGrab = bval;
    }

    void KoduAgent::setTargetInGripperFlag(bool bval) {
        targetObjectIsInGripper = bval;
    }

    /// ================================ Motion functions ================================ ///
    bool KoduAgent::hasMotionCommand() const {
        return currMotionCmd.isValid();
    }

    bool KoduAgent::isWalking() const {
        return agentIsWalking;
    }

    bool KoduAgent::needsToLocalize() const {
        return (distanceTravelled >= kLocalizationDistanceThreshold);
    }

    /// ================================ Scoring functions ================================ ///
    bool KoduAgent::hasNewScoreChanges() const {
        return (!scoreQueue.empty());
    }

    /// ================================ Page functions ================================ ///
    KoduPage* KoduAgent::getCurrentPage() const {
        return pages[currPageIndex];
    }

    KoduPage* KoduAgent::getPage(unsigned int pageNumber) const {
        return getPageInPos(pageNumber - 1);
    }

    KoduPage* KoduAgent::getPageInPos(unsigned int pageIndex) const {
        if (pageIndex < pages.size()) {
            return pages[pageIndex];
        } else {
            std::cout << "Page index \"" << pageIndex << "\" is out of bounds!\nReturning NULL...\n";
            return NULL;
        }
    }

    bool KoduAgent::hasNewPageNumber() const {
        return (newReqdPage > 0);
    }

    /// ================================ Speech functions ================================ ///
    bool KoduAgent::hasTextToSay() const {
        return (!stringToSpeak.empty());
    }

    /// ================================ Sound functions ================================ ///
    bool KoduAgent::hasSoundsToPlay() const {
        return (!playQueue.empty());
    }

    /// ================================ Gaze functions ================================ ///
    const DualCoding::Shape<DualCoding::PolygonData>& KoduAgent::getGazePolygon() const {
        return agentGazePolygon;
    }

    void KoduAgent::generateGazePolygon() {
        // SA: the search area (in degrees)--(from -SA/2 to SA/2)
        const float kSearchArea = 160.0f;
        // FoV: the (reduced) field of view
        // (reduced to compensate for any inaccuracies in moving the head)
        // the actual field of view is approx. 75 degrees
        const float kFoV = 70.0f;
        // N_GA: the minimum number of times the robot will have to move it's head to view the search area
        int numbOfGazeAngles = ceil(kSearchArea / kFoV);
        // delta_GA: the change in angle each time the robot moves it's head
        float deltaGazeAngle = ceil(kSearchArea / numbOfGazeAngles);
        // GA<N_GA>: the vector of gaze angles
        std::vector<float> gazeAngles;
        // GA[0]: (-SA / 2) + (delta_GA / 2)
        gazeAngles.push_back((-1 * (kSearchArea / 2)) + (deltaGazeAngle / 2));
        // GA[i]: GA[i - 1] + delta_GA
        for (int i = 1; i < numbOfGazeAngles; i++)
            gazeAngles.push_back(gazeAngles[i - 1] + deltaGazeAngle);

        // create the gaze points using the gaze angles
        std::vector<DualCoding::Point> gazePoints;
        const int numbOfRadii = 2;
        float radii[numbOfRadii] = {
            2000.0f,        // the distant radius
             500.0f         // the nearby radius
        };

        for (int radiiIndex = 0; radiiIndex < numbOfRadii; radiiIndex++) {
            for (int gaIndex = 0; gaIndex < numbOfGazeAngles * 2; gaIndex++) {
                gazePoints.push_back(DualCoding::Point(
                    cos(mathutils::deg2rad(gazeAngles[gaIndex])) * radii[radiiIndex],  // x-value
                    sin(mathutils::deg2rad(gazeAngles[gaIndex])) * radii[radiiIndex],  // y-value
                    0.0f,                                                              // z-value
                    DualCoding::egocentric                     // point is relative to agent body
                    ));
            }
        }
        // create a polygon search area
        NEW_SHAPE(gazePolygon, DualCoding::PolygonData,
            new DualCoding::PolygonData(DualCoding::VRmixin::localShS, gazePoints, false));
        // make sure the polygon is not an obstacle (in the localShS) nor viewable (in the local view)
        gazePolygon->setObstacle(false);
        gazePolygon->setViewable(false);
        // assign gazePolygon to agentaGzePoints
        agentGazePolygon = gazePolygon;
    }
    
}