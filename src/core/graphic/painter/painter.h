#include "shape/shape.h"

#include <map>
#include <string>

namespace painter {

class Window;

class BasicPainter
{
public:
  void LogicOr (){};
  void LogicAnd (){};
  void LogicNot (){};
  void Error (){};

  virtual void Arc (){};
  virtual void Line (){};
  virtual void Circle (){};
  virtual void Text (){};

private:
  virtual Window OpenWindow (){};
  virtual void Update (){};

  std::map<const std::string&, shape::Object> obj_table
};

}  // namespace painter