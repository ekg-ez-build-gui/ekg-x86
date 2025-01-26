#ifndef EKG_MATH_GEOMETRY_HPP
#define EKG_MATH_GEOMETRY_HPP

namespace ekg {
  template<typename t>
  struct vec2 {
    union {
      struct {
        t x {};
        t y {};
      };
    };
  public:
    inline vec2() = default;

    inline vec2(t _x, _y) {
      this->x = _x;
      this->y = _y;
    }
  };

  template<typename t>
  struct vec3 {
    union {
      struct {
        t x {};
        t y {};
        t z {};
      };
    };
  public:
    inline vec2() = default;

    inline vec2(t _x, t _y, t _z) {
      this->x = _x;
      this->y = _y;
      this->z = _z;
    }
  };

  template<typename t>
  struct vec4 {
    union {
      struct {
        t x {};
        t y {};
        t z {};
        t w {};
      };
    };
  public:
    inline vec2() = default;

    inline vec2(t _x, t _y, t _z, t _w) {
      this->x = _x;
      this->y = _y;
      this->z = _z;
      this->w = _w;
    }
  };

  template<typename t>
  struct rect {
  public: 
    union {
      struct {
        t x {};
        t y {};
        t w {};
        t h {};
      };
    };
  public:
    inline rect() = default;

    inline rect(t _x, t _y, t _w, t _h) {
      this->x = _x;
      this->y = _y;
      this->w = _w;
      this->h = _h;
    }
  };
}

#endif