"use client"

import {useEffect, useState} from "react"
import './Orderbook.css'

export const Orderbook = ({tickerName, setEmpty, orderbookData}: {
  tickerName: string, 
  setEmpty: React.Dispatch<React.SetStateAction<boolean>>,
  orderbookData: {
    bids: Array<{price: number, quantity: number}>,
    asks: Array<{price: number, quantity: number}>
  }
}) =>
{
    if (!orderbookData?.bids?.length || !orderbookData?.asks?.length) {
      setEmpty(true)
    return <div></div>}

    setEmpty(false)
    const bids = orderbookData.bids // Object.entries() returns a 2D array 
     // where each inner array has two elements: the key (index 0) and the value (index 1).
    let formattedBids = bids.map(({price, quantity}) => [Number(price).toFixed(2), quantity])
    formattedBids = formattedBids.reduce((acc, [price, quantity]) => {
      const previousTotal = acc.length > 0 ? acc[acc.length - 1][2] : 0  // get last element's total, or 0
      const newTotal = Number(previousTotal) + Number(quantity)
      return [...acc, [price, quantity, newTotal]]
    }, [] as (string | number)[][]) 
    const maxBidTotal = Number(formattedBids.at(-1)[2])
    

    const asks = orderbookData.asks
    let formattedAsks = asks.map(({price, quantity}) => [Number(price).toFixed(2),quantity]) 
    formattedAsks = formattedAsks.reduce((acc, [price, quantity]) => {
      const previousTotal = acc.length > 0 ? acc[acc.length - 1][2] : 0
      const newTotal = Number(previousTotal) + Number(quantity) 
      return [...acc, [price, quantity, newTotal]]
    }, [] as (string | number)[][])
    const maxAskTotal = Number(formattedAsks.at(-1)[2])


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
                <th style={{textAlign:'left', paddingRight:'350px'}}>price</th>
                <th style={{textAlign:'left', paddingRight:'150px'}}>size</th>
                <th style={{textAlign:'left'}}>total</th>
              </tr>
            </thead>
                <tbody className="bidLevels">
              {formattedBids.map(([price, quantity, runningtotal], index) => {
                const pct = (Number(runningtotal) / maxBidTotal) * 100
                return (
                  <tr
                    key={index}
                    style={{
                    background: `linear-gradient(to left, rgba(0, 180, 80, 0.15) ${pct}%, transparent ${pct}%)`,
                    }}
                  >
                    <td style={{color: "green"}}>{price}</td>
                    <td>{quantity}</td>
                    <td>{runningtotal}</td>
                  </tr>
                )
              })}
              </tbody> 
          </table> 
          <table className="asksTable"> 
            <thead className="asksHeader">
              <tr>
                <th style={{textAlign:'left', paddingRight:'350px'}}>price</th>
                <th style={{textAlign:'left', paddingRight:'150px'}}>size</th>
                <th>total</th>
              </tr>
            </thead>
              <tbody className="askLevels"> 
                {formattedAsks.map(([price, quantity, runningtotal], index) => {
                const pct = (Number(runningtotal) / maxAskTotal) * 100
                return (
                <tr key={index} style={{
                      background: `linear-gradient(to right, rgba(220, 50, 50, 0.15) ${pct}%, transparent ${pct}%)`,
                        }}>
                  <td style={{color: 'red'}}>{price}</td> 
                  <td>{quantity}</td> 
                  <td>{runningtotal}</td> 
                </tr>)})}
              </tbody> 
          </table> 
        </div>
      </div> 


    )
}