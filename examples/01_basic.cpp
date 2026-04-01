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

  pipeline.use([](Context &ctx, Next)
               { ctx.response().text("Hello from middleware"); });

  pipeline.run(ctx);

  std::cout << "Response body: " << ctx.response().body() << "\n";

  return 0;
}
