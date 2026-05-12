"use client"

import {useEffect, useState} from "react"

export const Orderbook = ({tickerName, orderbookData}) =>
{

    return (
      <div> 
        <div className="header"> 
          <h1> 
          {tickerName}
          </h1> 
        </div>
        <div className="stats">
          <p> Object.keys(orderbookData["bids"])[0]</p>
        </div>
      </div> 


    )
}