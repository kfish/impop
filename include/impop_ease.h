#pragma once

namespace ImPop {

float Linear(float t) {
    return t;
}

float QuadEaseIn(float t) {
    return t * t;
}

float QuadEaseOut(float t) {
    return t * (2 - t);
}

float QuadEaseInOut(float t) {
    return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

float ExpoEaseIn(float t) {
    return t == 0 ? 0 : pow(2, 10 * (t - 1));
}

float ExpoEaseOut(float t) {
    return t == 1 ? 1 : 1 - pow(2, -10 * t);
}

float ExpoEaseInOut(float t) {
    if (t == 0) return 0;
    if (t == 1) return 1;
    return t < 0.5f ? 0.5f * pow(2, 20 * t - 10) : 1 - 0.5f * pow(2, -20 * t + 10);
}

float LogEaseIn(float t) {
    return log(1 + 9 * t) / log(10);
}

float LogEaseOut(float t) {
    return 1 - log(1 + 9 * (1 - t)) / log(10);
}

float BounceEaseOut(float t) {
    if (t < 1 / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2 / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5 / 2.75) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

float BounceEaseIn(float t) {
    return 1 - BounceEaseOut(1 - t);
}

float BounceEaseInOut(float t) {
    return t < 0.5f ? BounceEaseIn(t * 2) * 0.5f : BounceEaseOut(t * 2 - 1) * 0.5f + 0.5f;
}

} // namespace ImPop
