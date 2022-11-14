#include "pros/misc.hpp"
#include "MtrGroups.hpp"
#include "apply_limiter.hpp"

class GameControls
{
private:
    pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
    float drive_limiter;
    MtrGroups mtr_groups;

    void update_drive();

public:
    GameControls();

    GameControls(MtrGroups groups);

    void run();
};
