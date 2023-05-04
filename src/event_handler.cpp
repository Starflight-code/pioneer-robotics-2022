#ifndef pid_cpp_
#define pid_cpp_
#include "PID.cpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <functional>
#include <list>
#include <vector>
#endif

#ifndef algorithms_cpp_
#define algorithms_cpp_
#include "algorithms.cpp"
#endif

#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#endif

/**
 * Tracks the status of a toggle switch
 */
class ToggleTracker {

private:
    bool held;
    bool previousState;

public:
    bool currentState;
    bool modifed;

    /** Initialzes the toggle tracker.
     * @param state the initial toggle state (boolean)
     */
    ToggleTracker() {
        held = false;
        currentState = false;
        previousState = currentState;
    }

    /** Initialzes the toggle tracker.
     * @param state the initial toggle state (boolean)
     */
    ToggleTracker(bool state) {
        held = false;
        currentState = state;
        previousState = currentState;
    }

    /** Updates the tracker with the current button value
     *  @param held current value of the button (boolean)
     */
    void updateTracker(bool held) {
        if(held && held != previousState) {
            currentState = !currentState;
            modifed = true;
        } else {
            modifed = false;
        }
        previousState = held;
    }
};

class MegaWrapper {
public:
    Motor_Class Motors;
    Robot Preset;
};

class EventHandler {
    Motor_Class* motors;
    Robot* preset;
    MegaWrapper wrapper;

    enum eventType {
        hold,
        toggle,
        press
    };
    std::vector<std::function<void(MegaWrapper)>> functions;
    std::vector<std::function<void(MegaWrapper)>> functionsIfDisabled;
    std::vector<pros::controller_digital_e_t> buttons;
    std::vector<eventType> eventHandlingType;
    std::vector<ToggleTracker> trackers;
    pros::Controller* master;

public:
    EventHandler() {
        motors = new Motor_Class;
        preset = &motors->preset;
        master = new pros::Controller(pros::E_CONTROLLER_MASTER);
    }

    void addEvent(std::function<void(MegaWrapper)> eventService, pros::controller_digital_e_t button, eventType type) {
        functions.push_back(eventService);
        functionsIfDisabled.push_back(NULL);
        buttons.push_back(button);
        eventHandlingType.push_back(type);

        if(type == toggle || type == press) {
            trackers.push_back(ToggleTracker());
        } else {
            trackers.push_back(NULL);
        }
    }

    void addEvent(std::function<void(MegaWrapper)> eventService, std::function<void(MegaWrapper)> eventServiceIfOff, pros::controller_digital_e_t button, eventType type) {
        functions.push_back(eventService);
        functionsIfDisabled.push_back(eventServiceIfOff);
        buttons.push_back(button);
        eventHandlingType.push_back(type);

        if(type == toggle || type == press) {
            trackers.push_back(ToggleTracker());
        } else {
            trackers.push_back(NULL);
        }
    }

    void checkEvents() {
        for(int i = 0; i < functions.size(); i++) {
            switch(eventHandlingType[i]) {

            case toggle:
                if(trackers[i].currentState) {
                    functions[i](wrapper);
                } else {
                    if(functionsIfDisabled[i] != NULL) {
                        functionsIfDisabled[i](wrapper);
                    }
                }
                trackers[i].updateTracker(master->get_digital(buttons[i]));
                break;

            case hold:
                if(master->get_digital(buttons[i])) {
                    functions[i](wrapper);
                } else {
                    if(functionsIfDisabled[i] != NULL) {
                        functionsIfDisabled[i](wrapper);
                    }
                }
                break;

            case press:
                if(trackers[i].modifed && master->get_digital(buttons[i])) {
                    functions[i](wrapper);
                } else {
                    if(functionsIfDisabled[i] != NULL) {
                        functionsIfDisabled[i](wrapper);
                    }
                }
                trackers[i].updateTracker(master->get_digital(buttons[i]));
                break;
            }
        }
    }
};