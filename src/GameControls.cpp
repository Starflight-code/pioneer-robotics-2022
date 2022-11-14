#include "ajr/GameControls.hpp"

// NOTE: mtr_groups(groups) -> Is magic do not touch it
GameControls::GameControls(MtrGroups groups) : mtr_groups(groups)
{
    master = pros::Controller(pros::E_CONTROLLER_MASTER);
    drive_limiter = 1;
}

void GameControls::update_drive()
{
    int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
        right_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

    left_y = apply_limiter(left_y, drive_limiter);
    right_y = apply_limiter(right_y, drive_limiter);

    mtr_groups.left_mtr_group.set_speed(left_y);
    mtr_groups.right_mtr_group.set_speed(right_y);
}

void GameControls::run()
{
    update_drive();
}
