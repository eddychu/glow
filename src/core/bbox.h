#pragma once
#include <glm/glm.hpp>
struct BBox {
    glm::vec3 min;
    glm::vec3 max;

    BBox() : min(glm::vec3(FLT_MAX)), max(glm::vec3(-FLT_MAX)) {}

    void expand(const glm::vec3 &p) {
        min = glm::min(min, p);
        max = glm::max(max, p);
    }

    void expand(const BBox &b) {
        min = glm::min(min, b.min);
        max = glm::max(max, b.max);
    }

    glm::vec3 center() const { return (min + max) * 0.5f; }
};
