#ifndef UTILS_HPP
#define UTILS_HPP

inline bool isTargetReached(int currentCost, int bestKnown, double targetPercent) {
    double threshold = bestKnown * (1.0 + targetPercent / 100.0);
    return currentCost <= threshold;
}

#endif // UTILS_HPP
