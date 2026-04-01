#include <iostream>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/middleware.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  Request req;
  req.set_path("/admin");

  Response res;
  Context ctx(req, res);

  Pipeline pipeline;

  pipeline.use([](Context &ctx, Next)
               {
                 if (ctx.request().path() == "/admin")
                 {
                   ctx.response().set_status(Status::forbidden);
                   ctx.response().text("Access denied");
                   return;
                 } });

  pipeline.use([](Context &ctx, Next)
               { ctx.response().text("Welcome"); });

  pipeline.run(ctx);

  std::cout << "Status: " << to_int(ctx.response().status()) << "\n";
  std::cout << "Body: " << ctx.response().body() << "\n";

  return 0;
}
