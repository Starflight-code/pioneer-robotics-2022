#include "ajr/apply_limiter.hpp"

int apply_limiter(int amount, float limiter)
{
    return int(amount * limiter);
}
