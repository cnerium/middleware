/**
 * @file Context.hpp
 * @brief cnerium::middleware — Middleware execution context
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the context shared across middleware execution.
 *
 * A Context represents a single request/response lifecycle.
 *
 * Responsibilities:
 *   - Provide access to the HTTP request
 *   - Provide access to the HTTP response
 *   - Act as a shared mutable state across middleware
 *
 * Design goals:
 *   - Lightweight
 *   - No ownership (non-owning references)
 *   - Zero overhead abstraction
 *   - Easy to extend in future versions
 *
 * Notes:
 *   - Context does NOT own Request/Response
 *   - Lifetime must be managed by the caller
 *
 * Usage:
 * @code
 *   using namespace cnerium::http;
 *   using namespace cnerium::middleware;
 *
 *   Request req;
 *   Response res;
 *
 *   Context ctx(req, res);
 *
 *   ctx.request().set_path("/users");
 *   ctx.response().set_status(Status::ok);
 * @endcode
 */

#pragma once

#include <cassert>

#include <cnerium/http/Request.hpp>
#include <cnerium/http/Response.hpp>

namespace cnerium::middleware
{
  /**
   * @brief Execution context for middleware pipeline.
   *
   * Wraps a Request and Response reference.
   */
  class Context
  {
  public:
    using request_type = cnerium::http::Request;
    using response_type = cnerium::http::Response;

    /**
     * @brief Construct a context from request and response.
     *
     * @param req HTTP request (non-owning)
     * @param res HTTP response (non-owning)
     */
    Context(request_type &req, response_type &res) noexcept
        : request_(&req), response_(&res)
    {
    }

    /// @brief Default constructor (invalid state).
    Context() = default;

    /**
     * @brief Returns mutable access to the request.
     *
     * @return request_type& HTTP request
     */
    [[nodiscard]] request_type &request() noexcept
    {
      assert(request_ && "Context::request() called on null");
      return *request_;
    }

    /**
     * @brief Returns const access to the request.
     *
     * @return const request_type& HTTP request
     */
    [[nodiscard]] const request_type &request() const noexcept
    {
      assert(request_ && "Context::request() called on null");
      return *request_;
    }

    /**
     * @brief Returns mutable access to the response.
     *
     * @return response_type& HTTP response
     */
    [[nodiscard]] response_type &response() noexcept
    {
      assert(response_ && "Context::response() called on null");
      return *response_;
    }

    /**
     * @brief Returns const access to the response.
     *
     * @return const response_type& HTTP response
     */
    [[nodiscard]] const response_type &response() const noexcept
    {
      assert(response_ && "Context::response() called on null");
      return *response_;
    }

    /**
     * @brief Returns true if context is valid.
     *
     * @return true if both request and response are set
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return request_ != nullptr && response_ != nullptr;
    }

    /**
     * @brief Reset the context to empty state.
     */
    void clear() noexcept
    {
      request_ = nullptr;
      response_ = nullptr;
    }

  private:
    request_type *request_{nullptr};
    response_type *response_{nullptr};
  };

} // namespace cnerium::middleware
