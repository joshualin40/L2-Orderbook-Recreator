"use client"

import {useEffect, useState} from "react"

export const Orderbook = ({tickerName, orderbookData}) =>
{
    const [data, setData] = useState(null)
    
    return (
      <div> 
        <div className="header"> 
          <h1> 
          {tickerName}
          </h1> 
        </div>
        <div className="stats"> 
            <p> Best Bid: {orderbookData["bids"][0]}</p>
        </div>
      </div> 


    )
}