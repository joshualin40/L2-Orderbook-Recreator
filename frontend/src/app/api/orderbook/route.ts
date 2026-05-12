

export async function GET(request: Request) { // request: what was sent to nextjs and contains all information about the fetch 
                                            // eg request.url gets the url, and then you can pair it with searchParams.get

    const { searchParams } = new URL(request.url) // a standard way to parse query parameters from an incoming request URL
    const date = searchParams.get('date')
    const time = searchParams.get('time')
    const ticker = searchParams.get('ticker')
    const response = await fetch(`http://127.0.0.1:8080/orderbook?date=${date}&time=${time}&ticker=${ticker}`); // use backticks `, 
    //     which tell JS to evaluate anything inside ${} as a variable.
    const data = await response.json();
    return Response.json(data);
}
