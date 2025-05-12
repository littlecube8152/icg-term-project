#ifndef INTERVAL_H_
#define INTERVAL_H_

class Interval {
public:
    float min, max;

    Interval();
    Interval(float min, float max);
    float size() const;
    bool contains(float x) const;
    bool surrounds(float x) const;
    float clamp(float x) const;

    static const Interval empty, universe, positive;
};

#endif
