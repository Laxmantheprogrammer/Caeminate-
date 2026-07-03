#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;
};

#endif