#pragma once

#include "imgui.h"

namespace ImPop {

// constexpr re-implementation of ImClamp.
// See https://github.com/ocornut/imgui/pull/7662
constexpr float Clamp(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

// constexpr re-implementation of ImGui::ColorConvertU32ToFloat4
constexpr ImVec4 ColorConvertU32ToFloat4(ImU32 in) {
    float s = 1.0f / 255.0f;
    return ImVec4(
        ((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_A_SHIFT) & 0xFF) * s
    );
}

// constexpr re-implementation of ImGui::ColorConvertFloat4ToU32
constexpr ImU32 ColorConvertFloat4ToU32(const ImVec4& in) {
    ImU32 out;
    out = ((ImU32)(in.x * 255.0f + 0.5f) & 0xFF) << IM_COL32_R_SHIFT;
    out |= ((ImU32)(in.y * 255.0f + 0.5f) & 0xFF) << IM_COL32_G_SHIFT;
    out |= ((ImU32)(in.z * 255.0f + 0.5f) & 0xFF) << IM_COL32_B_SHIFT;
    out |= ((ImU32)(in.w * 255.0f + 0.5f) & 0xFF) << IM_COL32_A_SHIFT;
    return out;
}

// constexpr re-implementation of ImGui::ColorConvertRBGtoHSV
constexpr void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
    float K = 0.0f;
    if (g < b) {
        std::swap(g, b);
        K = -1.0f;
    }
    if (r < g) {
        std::swap(r, g);
        K = -2.0f / 6.0f - K;
    }

    float chroma = r - (g < b ? g : b);
    out_h = fabsf(K + (g - b) / (6.0f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

// constexpr re-implementation of ImGui::ColorConvertHSVtoRGB
constexpr void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
    if (s == 0.0f) {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = (h - static_cast<int>(h));  // Equivalent to fmodf(h, 1.0f)
    h *= 6.0f;
    int i = static_cast<int>(h);
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i) {
        case 0: out_r = v; out_g = t; out_b = p; break;
        case 1: out_r = q; out_g = v; out_b = p; break;
        case 2: out_r = p; out_g = v; out_b = t; break;
        case 3: out_r = p; out_g = q; out_b = v; break;
        case 4: out_r = t; out_g = p; out_b = v; break;
        default: out_r = v; out_g = p; out_b = q; break;
    }
}

constexpr ImVec4 AdjustBrightness(ImVec4 color, float brightness) {
    // Ensure brightness is within the valid range [0, 1]
    brightness = (brightness < 0.0f) ? 0.0f : (brightness > 1.0f) ? 1.0f : brightness;

    // Adjust the color's RGB values by the brightness factor
    color.x *= brightness;
    color.y *= brightness;
    color.z *= brightness;

    // Ensure the RGB values remain within the valid range [0, 1]
    color.x = (color.x > 1.0f) ? 1.0f : color.x;
    color.y = (color.y > 1.0f) ? 1.0f : color.y;
    color.z = (color.z > 1.0f) ? 1.0f : color.z;

    return color;
}

// Helper function to convert RGB to HSL
constexpr void RGBToHSL(const ImVec4& color, float& outH, float& outS, float& outL) {
    float r = color.x;
    float g = color.y;
    float b = color.z;

    float max = fmaxf(fmaxf(r, g), b);
    float min = fminf(fminf(r, g), b);
    outL = (max + min) / 2.0f;

    if (max == min) {
        outH = outS = 0.0f; // achromatic
    } else {
        float d = max - min;
        outS = (outL > 0.5f) ? d / (2.0f - max - min) : d / (max + min);

        if (max == r) {
            outH = (g - b) / d + (g < b ? 6.0f : 0.0f);
        } else if (max == g) {
            outH = (b - r) / d + 2.0f;
        } else {
            outH = (r - g) / d + 4.0f;
        }
        outH /= 6.0f;
    }
}

// Helper function to convert HSL to RGB
constexpr ImVec4 HSLToRGB(float h, float s, float l, float a) {
    float r, g, b;

    auto hue2rgb = [](float p, float q, float t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    if (s == 0.0f) {
        r = g = b = l; // achromatic
    } else {
        float q = (l < 0.5f) ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return ImVec4(r, g, b, a);
}

// Constexpr function to adjust saturation
constexpr ImVec4 AdjustSaturation(const ImVec4& color, float saturation) {
    float h, s, l;
    RGBToHSL(color, h, s, l);

    // Clamp the saturation value
    saturation = (saturation < 0.0f) ? 0.0f : (saturation > 1.0f) ? 1.0f : saturation;

    // Set new saturation
    s = saturation;

    return HSLToRGB(h, s, l, color.w);
}

// Interpolate between two colors in HSV space and return RGB
constexpr ImVec4 InterpolateColorsHSV(const ImVec4& color1, const ImVec4& color2, float t) {
    // Clamp 't' to the range [0, 1] to avoid extrapolation
    t = Clamp(t, 0.0f, 1.0f);

    // Convert RGB to HSV
    float h1, s1, v1;
    float h2, s2, v2;
    ColorConvertRGBtoHSV(color1.x, color1.y, color1.z, h1, s1, v1);
    ColorConvertRGBtoHSV(color2.x, color2.y, color2.z, h2, s2, v2);

    // Handle hue wrap-around
    if (fabsf(h2 - h1) > 0.5f) {
        if (h1 < h2) {
            h1 += 1.0f;
        } else {
            h2 += 1.0f;
        }
    }

    // Interpolate H, S, and V components separately
    float h = h1 + t * (h2 - h1);
    float s = s1 + t * (s2 - s1);
    float v = v1 + t * (v2 - v1);

    // Wrap hue back to [0, 1]
    if (h >= 1.0f) h -= 1.0f;
    if (h < 0.0f) h += 1.0f;

    // Convert HSV back to RGB
    float r, g, b;
    ColorConvertHSVtoRGB(h, s, v, r, g, b);

    // Interpolate alpha
    float a = color1.w + t * (color2.w - color1.w);

    return ImVec4(r, g, b, a);
}

// Generic function to interpolate using a quadratic Bézier curve
constexpr float BezierInterpolate(float p0, float p1, float p2, float t) {
    float oneMinusT = 1.0f - t;
    return (oneMinusT * oneMinusT * p0) + (2.0f * oneMinusT * t * p1) + (t * t * p2);
}

// Interpolate between three colors using a quadratic Bézier curve in HSV space
constexpr ImVec4 BezierInterpolateColorsHSV(const ImVec4& color1, const ImVec4& color2, const ImVec4& color3, float t) {
    // Clamp 't' to the range [0, 1]
    t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

    // Convert RGB to HSV
    float h1, s1, v1;
    float h2, s2, v2;
    float h3, s3, v3;
    ColorConvertRGBtoHSV(color1.x, color1.y, color1.z, h1, s1, v1);
    ColorConvertRGBtoHSV(color2.x, color2.y, color2.z, h2, s2, v2);
    ColorConvertRGBtoHSV(color3.x, color3.y, color3.z, h3, s3, v3);

    // Interpolate HSV components using the generic Bézier interpolation function
    float h = BezierInterpolate(h1, h2, h3, t);
    float s = BezierInterpolate(s1, s2, s3, t);
    float v = BezierInterpolate(v1, v2, v3, t);

    // Convert interpolated HSV back to RGB
    float r, g, b;
    ColorConvertHSVtoRGB(h, s, v, r, g, b);

    // Interpolate alpha using the Bézier formula
    float a = BezierInterpolate(color1.w, color2.w, color3.w, t);

    return ImVec4(r, g, b, a);
}

// Constexpr function to generate a palette of ImU32 colors
template <size_t N>
constexpr std::array<ImU32, N> GeneratePalette(const ImVec4& color1, const ImVec4& color2, const ImVec4& color3) {
    std::array<ImU32, 16> palette = {};
    for (size_t i = 0; i < 16; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(N-1);
        ImVec4 interpolatedColor = BezierInterpolateColorsHSV(color1, color2, color3, t);
        palette[i] = ColorConvertFloat4ToU32(interpolatedColor);
    }
    return palette;
}

} // namespace ImPop
