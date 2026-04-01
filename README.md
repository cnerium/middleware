# cnerium/middleware

Composable middleware system for the Cnerium web framework.

**Header-only. Deterministic. Pipeline-based.**

---

## Download

https://vixcpp.com/registry/pkg/cnerium/middleware

---

## Overview

`cnerium/middleware` provides a lightweight middleware pipeline built on top of `cnerium/http`.

It is responsible for:

- request/response interception
- execution flow control
- chaining logic
- early termination (short-circuit)

It is designed to be:

- minimal
- predictable
- independent from transport layers

No server required.
No router required.

Just middleware.

---

## Why cnerium/middleware?

Most projects end up re-implementing:

- middleware chaining
- execution flow (next)
- request interception
- response mutation
- short-circuit logic

These implementations are often:

- tightly coupled to frameworks
- hard to test
- inconsistent
- difficult to reuse

`cnerium/middleware` fixes this by providing a clean, reusable middleware pipeline.

---

## Dependency

Depends on:

- `cnerium/http`

This ensures:

- shared request/response model
- consistent data flow
- seamless integration with router and server layers

---

## Installation

### Using Vix

```bash
vix add @cnerium/middleware
vix install
```

### Manual

```bash
git clone https://github.com/cnerium/middleware.git
```

Add `include/` to your project.

---

## Core Concepts

### Context

```cpp
Request req;
Response res;

Context ctx(req, res);
```

Provides access to:

- request
- response

---

### Middleware

```cpp
Middleware mw = [](Context &ctx, Next next)
{
  ctx.response().set_header("X-App", "Cnerium");
  next();
};
```

---

### Next

```cpp
next();
```

Advances execution to the next middleware.

If not called → pipeline stops.

---

### Pipeline

```cpp
Pipeline pipeline;

pipeline.use(mw);
pipeline.use(...);

pipeline.run(ctx);
```

---

## Execution Model

```
mw1 → mw2 → mw3 → end
```

---

## Typical Flow

```cpp
Pipeline pipeline;

pipeline.use([](Context &ctx, Next next)
{
  ctx.response().set_header("X-App", "Cnerium");
  next();
});

pipeline.use([](Context &ctx, Next)
{
  ctx.response().text("Hello");
});

pipeline.run(ctx);
```

---

## Example

```cpp
#include <iostream>

#include <cnerium/http/http.hpp>
#include <cnerium/middleware/middleware.hpp>

using namespace cnerium::http;
using namespace cnerium::middleware;

int main()
{
  Request req;
  Response res;
  Context ctx(req, res);

  Pipeline pipeline;

  pipeline.use([](Context &ctx, Next next)
  {
    ctx.response().set_header("X-App", "Cnerium");
    next();
  });

  pipeline.use([](Context &ctx, Next)
  {
    ctx.response().text("Hello from middleware");
  });

  pipeline.run(ctx);

  std::cout << ctx.response().body() << "\n";

  return 0;
}
```

---

## Short-Circuit Example

```cpp
pipeline.use([](Context &ctx, Next)
{
  if (ctx.request().path() == "/admin")
  {
    ctx.response().text("Access denied");
    return;
  }
});
```

---

## Execution Rules

- middleware is executed in insertion order
- calling next() continues execution
- not calling next() stops the pipeline
- request/response are shared

---

## Complexity

| Operation | Complexity |
|----------|-----------|
| middleware execution | O(n) |
| pipeline insertion | amortized O(1) |

---

## Design Philosophy

- minimal middleware core
- deterministic execution
- no hidden behavior
- framework-agnostic

---

## Tests

```bash
vix build
vix test
```

---

## License

MIT License
Copyright (c) Gaspard Kirira

