export async function GET(request: Request) {
    const response = await fetch('http://localhost:8080/orderbook');
    const data = await response.json();
    return Response.json(data);
}
