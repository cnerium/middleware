#include <iostream>
#include <string_view>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/middleware.hpp>

int main()
{
  using namespace cnerium::http;
  using namespace cnerium::middleware;

  auto run_case = [](std::string_view path)
  {
    Request req;
    req.set_method(Method::Get);
    req.set_path(std::string(path));

    Response res;
    Context ctx(req, res);

    Pipeline pipeline;

    pipeline.use([](Context &ctx, Next next)
                 {
                   ctx.response().set_header("X-Powered-By", "Cnerium");
                   next(); });

    pipeline.use([](Context &ctx, Next next)
                 {
                   if (ctx.request().path() == "/admin")
                   {
                     ctx.response().set_status(Status::forbidden);
                     ctx.response().text("Access denied");
                     return;
                   }

                   next(); });

    pipeline.use([](Context &ctx, Next next)
                 {
                   if (ctx.request().path() == "/hello")
                   {
                     ctx.response().set_status(Status::ok);
                     ctx.response().text("Hello from middleware pipeline");
                     return;
                   }

                   next(); });

    pipeline.use([](Context &ctx, Next)
                 {
                   ctx.response().set_status(Status::not_found);
                   ctx.response().text("Route not handled"); });

    pipeline.run(ctx);

    std::cout << "GET " << path << "\n";
    std::cout << "  Status: " << to_int(ctx.response().status())
              << " " << reason_phrase(ctx.response().status()) << "\n";
    std::cout << "  X-Powered-By: " << ctx.response().header("X-Powered-By") << "\n";
    std::cout << "  Body: " << ctx.response().body() << "\n\n";
  };

  run_case("/hello");
  run_case("/admin");
  run_case("/unknown");

  return 0;
}
