/**
 * @file middleware.hpp
 * @brief cnerium::middleware — Main public header for the Middleware module
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Aggregates the main public headers of the Cnerium Middleware module.
 *
 * Include this file when you want access to the full high-level middleware API:
 *   - version information
 *   - execution context
 *   - next continuation
 *   - middleware callable type
 *   - middleware pipeline
 *
 * Usage:
 * @code
 *   #include <cnerium/middleware/middleware.hpp>
 *
 *   using namespace cnerium::http;
 *   using namespace cnerium::middleware;
 *
 *   Request req;
 *   Response res;
 *   Context ctx(req, res);
 *
 *   Pipeline pipeline;
 *   pipeline.use([](Context &ctx, Next next)
 *   {
 *     ctx.response().set_header("X-App", "Cnerium");
 *     next();
 *   });
 *
 *   pipeline.run(ctx);
 * @endcode
 */

#pragma once

#include <cnerium/middleware/version.hpp>
#include <cnerium/middleware/Context.hpp>
#include <cnerium/middleware/Next.hpp>
#include <cnerium/middleware/Middleware.hpp>
#include <cnerium/middleware/Pipeline.hpp>
