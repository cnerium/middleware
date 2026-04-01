#include <cassert>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/Pipeline.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  {
    Pipeline pipeline;
    assert(pipeline.empty());

    Request req;
    Response res;
    Context ctx(req, res);

    pipeline.run(ctx); // should not crash
  }

  {
    Pipeline pipeline;

    int order = 0;

    pipeline.use([&](Context &, Next next)
                 {
                   assert(order == 0);
                   order = 1;
                   next(); });

    pipeline.use([&](Context &, Next next)
                 {
                   assert(order == 1);
                   order = 2;
                   next(); });

    Request req;
    Response res;
    Context ctx(req, res);

    pipeline.run(ctx);

    assert(order == 2);
  }

  {
    Pipeline pipeline;

    bool second_called = false;

    pipeline.use([&](Context &, Next)
                 {
                   // stop chain
                 });

    pipeline.use([&](Context &, Next)
                 { second_called = true; });

    Request req;
    Response res;
    Context ctx(req, res);

    pipeline.run(ctx);

    assert(!second_called);
  }

  {
    Pipeline pipeline;

    pipeline.use([](Context &ctx, Next next)
                 {
                   ctx.response().set_header("X-Test", "1");
                   next(); });

    pipeline.use([](Context &ctx, Next)
                 { ctx.response().text("OK"); });

    Request req;
    Response res;
    Context ctx(req, res);

    pipeline.run(ctx);

    assert(ctx.response().header("X-Test") == "1");
    assert(ctx.response().body() == "OK");
  }

  {
    Pipeline pipeline;

    pipeline.reserve(4);

    pipeline.use([](Context &, Next next)
                 { next(); });
    pipeline.use([](Context &, Next next)
                 { next(); });

    assert(pipeline.size() == 2);

    std::size_t count = 0;
    for (const auto &mw : pipeline)
    {
      (void)mw;
      ++count;
    }

    assert(count == 2);
  }

  {
    Pipeline pipeline;

    pipeline.use([](Context &, Next) {});
    pipeline.use([](Context &, Next) {});

    assert(pipeline.size() == 2);

    pipeline.clear();

    assert(pipeline.empty());
    assert(pipeline.size() == 0);
  }

  return 0;
}
