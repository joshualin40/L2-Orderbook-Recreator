# Orderbook

Reconstructs a L2 orderbook from [Databento](https://databento.com/) market data given a date, time, and ticker symbol. Takes L3 historical Market-By-Order (MBO) data and constructs the L2 orderbook.

Built with a C++ backend and React frontend.

### Demo
https://github.com/user-attachments/assets/6d31dbef-1d62-4d62-b6ad-522281c03088

## Overview 

### How it works

L2: Level 2 market data shows the total size available at each price level, for a fixed number of levels on each side.
L3: Level 3 market data shows every individual order in the book, including each order's ID, size, side, action (add, cancel, modify, trade), and position in the queue.

To reconstruct an L2 order book from L3 data, a full day of historical time series data is streamed from the Databento API and processed by a custom Orderbook class. A snapshot of the book's state is taken every 5 minutes. Given the user's inputted time, the most recent snapshot before that time is loaded and the events between it and the requested time are replayed. The top 10 bid and ask levels are then aggregated and returned as JSON.

### Orderbook and Order Classes

#### Order
Each order contains a timestamp (nanoseconds), order ID, quantity, price, side (B/A/N), and action (A/C/M/R/T/F/N). Also contains various getter and setter methods. 

#### Orderbook 
The Orderbook class has three containers. Bids and asks are each stored in a **`std::map`** keyed by price, where every price level holds its resting orders as **`unique_ptr`**s in a **`deque`**. All live orders are additionally stored in an **`unordered_map`** keyed by order ID.

A **map** was chosen because it keeps price levels sorted automatically, the best bid and ask are always at `begin()`, and iterating through the map gives the top *n* levels. Its size is the number of price levels rather than the number of orders, so lookups stay cheap.

A **deque** (double-ended queue) holds the orders at each level because position in the deque represents queue priority: new orders arrive at the back and fills consume from the front. A deque gives O(1) at both ends, whereas a **vector** would pay O(n) shifting elements on every fill.

**`unique_ptr`** gives each order a stable address that survives deque reallocation, and makes erases and price changes cheap, moving an order between levels shuffles a pointer instead of copying the whole object.

The **`unordered_map`** exists because cancel and modify messages carry only an order ID. Without it, finding an order's side and price level would mean scanning both maps while the **`unordered_map`** allows O(1) lookup for a specific order.

### Architecture

### Frontend

A form collecting a date, time, and ticker. Pressing **Create Orderbook** calls `fetchData`, which clears the previous book, flips on a loading state, and issues a GET request to a Next.js API route:

```tsx
const response = await fetch(`/api/orderbook?date=${currentDate}&time=${currenttime}&ticker=${currentTicker}`)
const data = await response.json()
setOrderbook(data)
```

The response is parsed into a JS object and stored in React state, which triggers a re-render of the depth ladder.

### API route

The browser never contacts the C++ server directly. A Next.js Route Handler sits in between, reading the query parameters and forwarding them on:

```ts
const { searchParams } = new URL(request.url)
const date = searchParams.get('date')
const time = searchParams.get('time')
const ticker = searchParams.get('ticker')

const response = await fetch(`http://127.0.0.1:8080/orderbook?date=${date}&time=${time}&ticker=${ticker}`)
const data = await response.json()
return Response.json(data)
```

This acts as a proxy. The browser only ever talks to its own origin, so the same-origin policy never comes into play, and the C++ server's port is never exposed to the client.

### Backend

The `cpp-httplib` library turns `main()` into an HTTP server. It registers the `/orderbook` route:

```cpp
svr.Get("/orderbook", [](const Request& req, Response& res) { ... });
```

This stores the lambda rather than running it, so the server knows which function to call when a request arrives at that path. It then blocks on `listen()`:

```cpp
svr.listen("0.0.0.0", 8080);
```

This call never returns — the server holds a socket on port 8080, matching each incoming request against the registered route and invoking the handler. The handler reads the three parameters, reconstructs the book, and returns the top 10 levels per side as JSON.
## Setup

### Prerequisites

**1. Get an API Key**  
Generate a Historical API key at https://databento.com/

**2. Set the API key in your shell**
```bash
export API_KEY=your_api_key_here
```

### Backend

**3. Build**
```bash
cd backend
mkdir build && cd build
cmake ..
make
```

**4. Run**
```bash
./example
```

### Frontend (Open a new terminal while backend runs)

**5. Install dependencies and start**
```bash
cd frontend
npm install
npm run dev
```

Opens at `http://localhost:3000` by default.

## Dependencies

- [Databento C++ client](https://github.com/databento/databento-cpp)
- CMake
- React
- Node.js

# Third Party Libraries

## react-calendar
- Author: Wojcieh Maj
- Source: https://github.com/wojtekmaj/react-calendar
- License: MIT

## cpp-httplib
- Author: Yuji Hirose
- Source: https://github.com/yhirose/cpp-httplib
- License: MIT

## nlohmann/json
- Author: Niels Lohmann
- Source: https://github.com/nlohmann/json
- License: MIT

## Databento C++ Client
- Source: https://github.com/databento/databento-cpp
- License: Apache 2.0
