#include <cassert>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/Context.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  {
    Request req;
    Response res;

    Context ctx(req, res);

    assert(ctx.valid());

    ctx.request().set_path("/test");
    ctx.response().set_status(Status::ok);

    assert(ctx.request().path() == "/test");
    assert(ctx.response().status() == Status::ok);
  }

  {
    Request req;
    Response res;

    Context ctx(req, res);

    ctx.clear();
    assert(!ctx.valid());
  }

  return 0;
}
