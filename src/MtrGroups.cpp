#include "ajr/MtrGroups.hpp"
#include "var.cpp"

MtrGroups::MtrGroups()
{
    // --------------------------------------------------
    // Motor Group(s) for driving
    // --------------------------------------------------

    left_mtr_group = MtrGroup(num_motors_pre_side, left_side_mtr_ports);
    right_mtr_group = MtrGroup(num_motors_pre_side, right_side_mtr_ports);

    // --------------------------------------------------
    // Motor Group(s) for flywheel
    // --------------------------------------------------

    flywheel_mtr_group = MtrGroup(flywheel_mtr_count, flywheel_mtr_ports);
}

void MtrGroups::setup()
{
    left_mtr_group.alt_reverse();
    right_mtr_group.alt_reverse();
}
