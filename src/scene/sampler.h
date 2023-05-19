#include <scene/sid.h>

struct Sampler : public SceneObject {
  enum class Filter {
    Nearest,
    Linear,
  };
  enum class Wrap { Repeat, MirroredRepeat, ClampToEdge, ClampToBorder };

  static Filter filter_from_int(int i) {
    switch (i) {
    case 9728:
      return Filter::Nearest;
    case 9729:
      return Filter::Linear;
    default:
      return Filter::Nearest;
    }
  }

  static Wrap wrap_from_int(int i) {
    switch (i) {
    case 10497:
      return Wrap::Repeat;
    case 33648:
      return Wrap::MirroredRepeat;
    case 33071:
      return Wrap::ClampToEdge;
    case 33069:
      return Wrap::ClampToBorder;
    default:
      return Wrap::Repeat;
    }
  }

  Filter min_filter{Filter::Nearest};
  Filter mag_filter{Filter::Nearest};
  Wrap wrap_s{Wrap::Repeat};
  Wrap wrap_t{Wrap::Repeat};
  Wrap wrap_r{Wrap::Repeat};
};
