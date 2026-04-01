/**
 * @file Middleware.hpp
 * @brief cnerium::middleware — Middleware callable definition
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the primary middleware callable type used by the Cnerium
 * middleware pipeline.
 *
 * A middleware receives:
 *   - a Context object representing the current request/response state
 *   - a Next continuation used to advance execution
 *
 * Design goals:
 *   - Simple and expressive API
 *   - Header-only
 *   - Easy lambda integration
 *   - Deterministic synchronous execution
 *
 * Notes:
 *   - If a middleware does not call next(), the pipeline stops
 *   - Middleware can inspect and mutate both request and response
 *   - Middleware is represented as a std::function for maximum ergonomics
 *
 * Usage:
 * @code
 *   using namespace cnerium::middleware;
 *
 *   Middleware mw = [](Context &ctx, Next next)
 *   {
 *     ctx.response().set_header("X-App", "Cnerium");
 *     next();
 *   };
 * @endcode
 */

#pragma once

#include <functional>

#include <cnerium/middleware/Context.hpp>
#include <cnerium/middleware/Next.hpp>

namespace cnerium::middleware
{
  /**
   * @brief Primary middleware callable type.
   *
   * Signature:
   *   void(Context&, Next)
   */
  using Middleware = std::function<void(Context &, Next)>;

} // namespace cnerium::middleware
