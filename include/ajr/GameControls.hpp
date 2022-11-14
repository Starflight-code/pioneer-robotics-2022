#include "pros/misc.hpp"
#include "MtrGroups.hpp"
#include "apply_limiter.hpp"

class GameControls
{
private:
    /**
     * The robot controller
     */
    pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

    /**
     * Limit the speed of the driving motors.
     * Value from `0.0-1.0`, with `0.75` being `75%` of drive power
     */
    float drive_limiter;

    /**
     * Class of motor groups
     */
    MtrGroups mtr_groups;

    /**
     * Updates the drive motors based on the controller inputs
     */
    void update_drive();

public:
    /**
     * Default constructor for weired c++ things
     */
    GameControls();

    /**
     * Create GameControls with given parameter MtrGroups
     */
    GameControls(MtrGroups groups);

    /**
     * Updates the controller and related methods
     */
    void run();
};
