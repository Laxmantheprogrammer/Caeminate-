#include "action.hh"

#include "DetectorConstruction.hh"
#include "generator.hh"
#include "run.hh"
#include "RunMessenger.hh"
#include "EventAction.hh"      // <-- ADD THIS

ActionInitialization::ActionInitialization(DetectorConstruction* det)
: fDetector(det)
{}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    PrimaryGenerator* generator = new PrimaryGenerator(fDetector);
    SetUserAction(generator);

    RunAction* runAction = new RunAction();
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);

    new RunMessenger(runAction);
}