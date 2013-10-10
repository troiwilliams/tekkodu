#ifndef VISUAL_WALK_PROGRESS_TASK_H_
#define VISUAL_WALK_PROGRESS_TASK_H_

// INCLUDES
// c++
#include <vector>

// tekkotsu
#include "DualCoding/ShapeRoot.h"

namespace Kodu {

    // tekkodu forward declarations
    class PerceptualTaskBase;

    /**
     * ASSUMPTIONS:
     * - The target shape passed to the constructor is in the world shape space.
     *
    **/
    class VisualWalkProgressTask : public PerceptualTaskBase {
    public:
        //! Constructor #1
        VisualWalkProgressTask(const DualCoding::ShapeRoot& kTargetShape)
          : PerceptualTaskBase(PT_VIS_WALK_PROGRESS, ++idCount),
            errorCount(0),
            agentPosDuringFirstError(),
            targets()
        {
            targets.push_back(kTargetShape);
        }

        //! Constructor #2
        VisualWalkProgressTask(const std::vector<DualCoding::ShapeRoot>& kTargetShapes)
          : PerceptualTaskBase(PT_VIS_WALK_PROGRESS, ++idCount),
            errorCount(0),
            agentPosDuringFirstError(),
            targets(kTargetShapes)
        { }

        //! Copy constructor
        VisualWalkProgressTask(const VisualWalkProgressTask& kTask)
          : PerceptualTaskBase(kTask),
            errorCount(kTask.errorCount),
            agentPosDuringFirstError(kTask.agentPosDuringFirstError),
            targets(kTask.targets)
        { }

        //! Destructor
        ~VisualWalkProgressTask() {
            // no explicit implementation
        }

        //! Assignment operator
        VisualWalkProgressTask& operator=(const VisualWalkProgressTask& kTask) {
            if (this != &kTask) {
                PerceptualTaskBase::operator=(kTask);
                errorCount = kTask.errorCount;
                agentPosDuringFirstError = kTask.agentPosDuringFirstError;
                targets = kTask.targets;
            }
            return *this;
        }

        //! Checks if the VisualWalkProgressTask can execute
        virtual bool canExecute(const KoduWorld&);

        //! Examines the results from the MapBuilder request to see if the robot made "progress"
        virtual void examineTaskResults();

        //! Dynamically generates the point the agent should fixate on
        virtual const DualCoding::MapBuilderRequest& getMapBuilderRequest();

        //! Checks if the task is complete
        virtual bool taskIsComplete(const KoduWorld&);

        //! The max number of cumulative errors (error: when the agent did not find a match)
        static unsigned int const kMaxErrorOccurences;
    private:
        static unsigned int idCount;                //!< Used to generate id numbers
        unsigned int errorCount;                    //!< Cumulative error count
        DualCoding::Point agentPosDuringFirstError; //!< Position of agent at each "first" cumulative error
        std::vector<DualCoding::ShapeRoot> targets; //!< The shapes the agent needs to look at
    };

}

#endif // VISUAL_WALK_PROGRESS_TASK_H_