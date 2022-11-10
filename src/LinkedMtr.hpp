#include "include.cpp"

class LinkedMtr
{
private:
    int mtr_count;
    std::vector<pros::Motor> motors;

public:
    LinkedMtr(const int mtr_count, const int mtr_ports[], bool altReverse);

    void set_speed(int speed);
    void altReverse();
    float get_actual_velocity();
};
