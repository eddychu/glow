#pragma once
#include <scene/sid.h>
#include <scene/sampler.h>
#include <variant>
#include <vector>


typedef std::vector<unsigned char> LDR;
typedef std::vector<float> HDR;

typedef std::variant<LDR, HDR> PixelDataArray;

struct Texture : public SceneObject {
    Texture() = default;
    uint32_t width;
    uint32_t height;
    uint32_t component;
    PixelDataArray data;
    Sampler sampler;
};

struct TextureCube : public SceneObject {
    TextureCube() = default;
    uint32_t width;
    uint32_t height;
    uint32_t component;
    std::vector<PixelDataArray> data;
    Sampler sampler;
};