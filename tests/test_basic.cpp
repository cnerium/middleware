#include <cassert>
#include <string_view>

#include <cnerium/middleware/middleware.hpp>

int main()
{
  using namespace cnerium::middleware;

  static_assert(version_major == 0);
  static_assert(version_minor == 1);
  static_assert(version_patch == 0);
  static_assert(version_string() == std::string_view("0.1.0"));
  static_assert(version_number() == 100);

  {
    Pipeline pipeline;
    assert(pipeline.empty());
    assert(pipeline.size() == 0);
  }

  {
    Context ctx;
    assert(!ctx.valid());
  }

  return 0;
}
