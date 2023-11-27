#include <string>

namespace shape {

enum class Color
{
  black = 0,
};

struct Point
{
  double x{0};
  double y{0};

  Point() = default;
  Point(double xx, double yy) : x{xx}, y{yy} {};
};

class Object
{
public:
  Object(const std::string& name) : name_{name} {};
  Object(const std::string& name, Point* center)
      : name_{name}, center_{center} {};

  const std::string& name () { return name_; };

  Point center () { return *center_; };

  double size () { return size_; };

  void set_size (double size) { size_ = size; };

  Color color () { return color_; };

  void set_color (Color color) { color_ = color; };

  virtual void move (const Point& delta){};

private:
  const std::string& name_{};
  Point* center_{};
  double size_{1.0};
  Color color_ = Color::black;
};

class Arc : public Object
{
public:
  Arc(const std::string& name, Point* center, double width, double high,
      double start_angle, double end_angle)
      : Object{name, center}, width_{width}, high_{high},
        start_angle_{start_angle}, end_angle_{end_angle} {};

  double width () { return width_; };

  void set_width (double width) { width_ = width; };

  double high () { return high_; };

  void set_high (double high) { high_ = high; };

  double start_angle () { return start_angle_; };

  void set_start_anble (double start_angle) { start_angle_ = start_angle; };

  double end_angle () { return end_angle_; };

  void set_end_angle (double end_angle) { end_angle_ = end_angle; };

private:
  double width_{.0};
  double high_{.0};
  double start_angle_{0};
  double end_angle_{180};
};

class Line : public Object
{
public:
  Line(const std::string& name, Point* p1, Point* p2)
      : Object{name}, p1_{p1}, p2_{p2} {};

  Point p1 () { return *p1_; };

  void set_p1 (const Point& p1) { p1_ = &Point{p1}; };

  Point p2 () { return *p2_; };

  void set_p2 (const Point& p2) { p2_ = &Point{p2}; };

private:
  Point* p1_{};
  Point* p2_{};
};

class Circle : public Object
{
public:
  Circle(const std::string& name, Point* center, double radius)
      : Object{name, center}, radius_{radius} {};
  Circle(const std::string& name, Point* center, double radius, bool fill)
      : Object{name, center}, radius_{radius}, fill_{fill} {};

  double radius () { return radius_; };

  void set_radius (double radius) { radius_ = radius; };

  bool fill () { return fill_; };

  void set_fill (bool fill) { fill_ = fill; };

private:
  double radius_{.0};
  bool fill_{false};
};

class Text : public Object
{
public:
  Text(const std::string& name, Point* center, std::string* text)
      : Object{name, center}, text_{text} {};

  std::string text () { return *text_; };

  void set_text (const std::string& text) {};

private:
  double width_{.0};
  double high_{.0};
  std::string* text_{};

  void calc_border() {};
};

}  // namespace shape