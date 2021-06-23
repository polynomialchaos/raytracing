#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <sstream>
#include <raytracing/raytracing.h>

const number_t infinity = std::numeric_limits<number_t>::infinity();
const number_t pi = acos(-1.0);

inline number_t random_number_t()
{
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline number_t random_number_t(number_t min, number_t max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random_number_t();
}

inline number_t degrees_to_radians(number_t degrees)
{
    return degrees * pi / 180.0;
}

class ParallelStream
{
    std::ostringstream stream;

public:
    ParallelStream() {}
    template <class T>
    ParallelStream &operator<<(const T &inData)
    {
        stream << inData;
        return *this;
    }
    std::string toString() const
    {
        return stream.str();
    }
};

#endif /* UTILS_H */