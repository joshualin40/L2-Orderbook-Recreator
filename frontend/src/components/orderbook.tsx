"use client"

import {useEffect, useState} from "react"
import './Orderbook.css'

export const Orderbook = ({tickerName, orderbookData}: {tickerName: string, orderbookData: {bids: Record<string, number>, asks: Record<string, number>}}) =>
{
    const bids = Object.entries(orderbookData.bids) // Object.entries() returns a 2D array 
     // where each inner array has two elements: the key (index 0) and the value (index 1).
    let formattedBids = bids.map(([price, quantity]) => ([Number(price).toFixed(2),quantity]) )
    formattedBids = formattedBids.reduce((acc, [price, quantity]) => {
      const previousTotal = acc.length > 0 ? acc[acc.length - 1][2] : 0  // get last element's total, or 0
      const newTotal = Number(previousTotal) + Number(quantity)
      return [...acc, [price, quantity, newTotal]]
    }, [] as (string | number)[][]) // calculate the running total for each element of the 2d array 


    const asks = Object.entries(orderbookData.asks)
    let formattedAsks = asks.map(([price, quantity]) => ([Number(price).toFixed(2),quantity]) )

    formattedAsks = formattedAsks.reduce((acc, [price, quantity]) => {
      const previousTotal = acc.length > 0 ? acc[acc.length - 1][2] : 0
      const newTotal = Number(previousTotal) + Number(quantity) 
      return [...acc, [price, quantity, newTotal]]
    }, [] as (string | number)[][])

    const spread = (Number(formattedAsks[0][0]) - Number(formattedBids[0][0])).toFixed(2)

    return (
      <div> 
        <div className="header"> 
          <h1> 
          {tickerName} 
          </h1> 
        </div>
        <div className="stats">
          <p style={{color: 'green'}}> Bid: {formattedBids[0][0]}</p> 
          <p style={{color: 'red'}}> Ask: {formattedAsks[0][0]}</p> <p>Spread: {spread} </p>
        </div>
        <div className="orderbookLevels">
          <table className="bidsTable"> 
            <thead className="bidsHeader">
              <tr>
                <th>Price</th>
                <th>Size</th>
                <th>Total</th>
              </tr>
            </thead>
              <tbody className="bidLevels"> 
                {formattedBids.map(([price, quantity, runningtotal], index) =>
                   <tr key={index} style={{position:"relative"}}> 
                   <div style={{position: "absolute", backgroundColor:"green", width: "10px"}}> </div>
                   <td>{price}</td> 
                   <td>{quantity}</td> 
                   <td>{runningtotal}</td> 
                   </tr>)}
              </tbody> 
          </table> 
          <table className="asksTable"> 
            <thead className="asksHeader">
              <tr>
                <th>Price</th>
                <th>Size</th>
                <th>Total</th>
              </tr>
            </thead>
              <tbody className="askLevels"> 
                {formattedAsks.map(([price, quantity, runningtotal], index) => 
                <tr key={index}> <td>{price}</td> <td>{quantity}</td> <td>{runningtotal}</td> </tr>)}
              </tbody> 
          </table> 
        </div>
      </div> 


    )
}