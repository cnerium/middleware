#include <cassert>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/Middleware.hpp>
#include <cnerium/middleware/Context.hpp>
#include <cnerium/middleware/Next.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  {
    Request req;
    Response res;
    Context ctx(req, res);

    bool called = false;

    Middleware mw = [&](Context &, Next)
    {
      called = true;
    };

    mw(ctx, Next{});

    assert(called);
  }

  {
    Request req;
    Response res;
    Context ctx(req, res);

    bool next_called = false;

    Middleware mw = [&](Context &, Next next)
    {
      next();
    };

    Next next([&]()
              { next_called = true; });

    mw(ctx, next);

    assert(next_called);
  }

  {
    Request req;
    Response res;
    Context ctx(req, res);

    Middleware mw = [](Context &ctx, Next)
    {
      ctx.response().text("Hello");
    };

    mw(ctx, Next{});

    assert(ctx.response().body() == "Hello");
  }

  return 0;
}
