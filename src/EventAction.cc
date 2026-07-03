#include "EventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>

EventAction::EventAction()
: G4UserEventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    // Nothing yet
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    int eventID = event->GetEventID();
    int totalEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

    const int barWidth = 40;

    double progress = double(eventID + 1) / totalEvents;
    int pos = int(progress * barWidth);

    std::cout << "\r[";

    for (int i = 0; i < barWidth; i++)
    {
        if (i < pos)
            std::cout << "█";
        else
            std::cout << " ";
    }

    std::cout << "] "
              << std::setw(3) << int(progress * 100)
              << "% ("
              << eventID + 1
              << "/"
              << totalEvents
              << ")"
              << std::flush;

    if (eventID + 1 == totalEvents)
        std::cout << std::endl;
}