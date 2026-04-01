/**
 * @file Pipeline.hpp
 * @brief cnerium::middleware — Middleware pipeline execution container
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the middleware pipeline responsible for storing and executing
 * middleware functions in order.
 *
 * A pipeline is responsible for:
 *   - Registering middleware
 *   - Preserving insertion order
 *   - Executing middleware as a chain
 *   - Supporting short-circuit behavior
 *
 * Design goals:
 *   - Header-only
 *   - Simple and predictable execution model
 *   - Deterministic synchronous behavior
 *   - Easy future extension for error handling or async support
 *
 * Notes:
 *   - Middleware is executed in insertion order
 *   - If a middleware does not call next(), execution stops
 *   - A default-constructed pipeline is valid and empty
 *
 * Usage:
 * @code
 *   using namespace cnerium::middleware;
 *
 *   Pipeline pipeline;
 *
 *   pipeline.use([](Context &ctx, Next next)
 *   {
 *     ctx.response().set_header("X-App", "Cnerium");
 *     next();
 *   });
 *
 *   pipeline.use([](Context &ctx, Next)
 *   {
 *     ctx.response().text("Hello");
 *   });
 *
 *   pipeline.run(ctx);
 * @endcode
 */

#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include <cnerium/middleware/Middleware.hpp>

namespace cnerium::middleware
{
  /**
   * @brief Ordered container of middleware with execution support.
   *
   * Stores middleware in insertion order and runs them as a chain.
   */
  class Pipeline
  {
  public:
    using value_type = Middleware;
    using storage_type = std::vector<value_type>;
    using iterator = storage_type::iterator;
    using const_iterator = storage_type::const_iterator;

    /// @brief Default constructor.
    Pipeline() = default;

    /**
     * @brief Register a middleware in the pipeline.
     *
     * Middleware is executed in the same order it is added.
     *
     * @param middleware Middleware callable
     * @return Pipeline& Self for chaining
     */
    Pipeline &use(Middleware middleware)
    {
      middleware_.push_back(std::move(middleware));
      return *this;
    }

    /**
     * @brief Execute the pipeline against a context.
     *
     * Middleware is invoked in insertion order. If one middleware
     * does not call next(), execution stops there.
     *
     * @param ctx Middleware execution context
     */
    void run(Context &ctx) const
    {
      run_from(0, ctx);
    }

    /**
     * @brief Returns the number of registered middleware.
     *
     * @return std::size_t Middleware count
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return middleware_.size();
    }

    /**
     * @brief Returns true if no middleware is registered.
     *
     * @return true if empty
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return middleware_.empty();
    }

    /**
     * @brief Reserve storage for middleware entries.
     *
     * @param n Number of middleware to reserve
     */
    void reserve(std::size_t n)
    {
      middleware_.reserve(n);
    }

    /**
     * @brief Remove all registered middleware.
     */
    void clear() noexcept
    {
      middleware_.clear();
    }

    /**
     * @brief Returns const access to the middleware storage.
     *
     * @return const storage_type& Registered middleware
     */
    [[nodiscard]] const storage_type &middleware() const noexcept
    {
      return middleware_;
    }

    /**
     * @brief Returns mutable access to the middleware storage.
     *
     * @return storage_type& Registered middleware
     */
    [[nodiscard]] storage_type &middleware() noexcept
    {
      return middleware_;
    }

    /// @brief Mutable begin iterator.
    [[nodiscard]] iterator begin() noexcept
    {
      return middleware_.begin();
    }

    /// @brief Mutable end iterator.
    [[nodiscard]] iterator end() noexcept
    {
      return middleware_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator begin() const noexcept
    {
      return middleware_.begin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator end() const noexcept
    {
      return middleware_.end();
    }

    /// @brief Const begin iterator.
    [[nodiscard]] const_iterator cbegin() const noexcept
    {
      return middleware_.cbegin();
    }

    /// @brief Const end iterator.
    [[nodiscard]] const_iterator cend() const noexcept
    {
      return middleware_.cend();
    }

  private:
    storage_type middleware_{};

    void run_from(std::size_t index, Context &ctx) const
    {
      if (index >= middleware_.size())
      {
        return;
      }

      const auto next = Next([this, index, &ctx]()
                             { run_from(index + 1, ctx); });

      middleware_[index](ctx, next);
    }
  };

} // namespace cnerium::middleware
