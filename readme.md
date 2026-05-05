# Orderbook

Reconstructs a historical L2 order book from Databento market data.

## Setup

### 1. Get an API Key
Generate a Historical API key at https://databento.com/

### 2. Set the API Key
```bash
export API_KEY=your_api_key_here
```

### 3. Build
```bash
cd backend
mkdir build && cd build
cmake ..
make
```

### 4. Run
```bash
./example
```

## Dependencies
- Databento C++ client
- CMake