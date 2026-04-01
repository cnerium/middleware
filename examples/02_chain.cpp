#include <iostream>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/middleware.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  Request req;
  Response res;
  Context ctx(req, res);

  Pipeline pipeline;

  pipeline.use([](Context &ctx, Next next)
               {
                 ctx.response().set_header("X-App", "Cnerium");
                 std::cout << "Middleware 1\n";
                 next(); });

  pipeline.use([](Context &ctx, Next next)
               {
                 ctx.response().set_header("X-Version", "0.1.0");
                 std::cout << "Middleware 2\n";
                 next(); });

  pipeline.use([](Context &ctx, Next)
               {
                 ctx.response().text("Hello from chained middleware");
                 std::cout << "Middleware 3\n"; });

  pipeline.run(ctx);

  std::cout << "Body: " << ctx.response().body() << "\n";
  std::cout << "X-App: " << ctx.response().header("X-App") << "\n";
  std::cout << "X-Version: " << ctx.response().header("X-Version") << "\n";

  return 0;
}
