/**
 * @file Next.hpp
 * @brief cnerium::middleware — Middleware continuation callback
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the continuation callback used to advance execution
 * to the next middleware in the pipeline.
 *
 * A Next object represents the "call next middleware" operation.
 *
 * Design goals:
 *   - Simple API
 *   - Lightweight callable wrapper
 *   - Easy integration with std::function-based middleware
 *   - Deterministic synchronous execution
 *
 * Notes:
 *   - Calling next() transfers execution to the next middleware
 *   - If next() is not called, the pipeline stops early
 *   - Default-constructed Next is empty and safe to query with valid()
 *
 * Usage:
 * @code
 *   using namespace cnerium::middleware;
 *
 *   Next next([] {
 *     // continue pipeline
 *   });
 *
 *   if (next.valid())
 *   {
 *     next();
 *   }
 * @endcode
 */

#pragma once

#include <functional>
#include <utility>

namespace cnerium::middleware
{
  /**
   * @brief Continuation callback for middleware chaining.
   *
   * Wraps a callable that advances execution to the next middleware.
   */
  class Next
  {
  public:
    using callback_type = std::function<void()>;

    /// @brief Default constructor creates an empty continuation.
    Next() = default;

    /**
     * @brief Construct from a callable continuation.
     *
     * @param callback Function called when advancing to the next middleware
     */
    explicit Next(callback_type callback)
        : callback_(std::move(callback))
    {
    }

    /**
     * @brief Invoke the continuation.
     *
     * Does nothing if the continuation is empty.
     */
    void operator()() const
    {
      if (callback_)
      {
        callback_();
      }
    }

    /**
     * @brief Returns true if a continuation is set.
     *
     * @return true if callable is present
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return static_cast<bool>(callback_);
    }

    /**
     * @brief Returns true if no continuation is set.
     *
     * @return true if empty
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return !valid();
    }

    /**
     * @brief Reset to an empty continuation.
     */
    void clear() noexcept
    {
      callback_ = nullptr;
    }

    /**
     * @brief Explicit boolean conversion.
     *
     * Allows usage such as:
     * @code
     *   if (next) { next(); }
     * @endcode
     *
     * @return true if callable is present
     */
    explicit operator bool() const noexcept
    {
      return valid();
    }

  private:
    callback_type callback_{};
  };

} // namespace cnerium::middleware
