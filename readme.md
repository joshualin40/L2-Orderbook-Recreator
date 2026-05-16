# Orderbook

Reconstructs a L2 orderbook from [Databento](https://databento.com/) market data given a date, time, and ticker symbol. Takes L3 historical Market-By-Order (MBO) data and constructs the L2 orderbook.

Built with a C++ backend and React frontend.

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