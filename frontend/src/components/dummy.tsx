"use client"
import React from "react";

import { useState } from "react"

export const Dummy = () => {
    const [date, setDate] = useState("")
    const [time, setTime] = useState("")
    const [ticker, setTicker] = useState("")
    const [orderbook, setOrderbook] = useState(null)
 
    function handleDateChange(event: React.ChangeEvent<HTMLInputElement>)
    {
        setDate(event.target.value)
    }

    function handleTimeChange(event: React.ChangeEvent<HTMLInputElement>)
    {
        setTime(event.target.value)
    }

    function handleTickerChange(event: React.ChangeEvent<HTMLInputElement>)
    {
        setTicker(event.target.value)
    }

async function handleClick() {
    try {
        const url = `http://127.0.0.1:8080/orderbook?date=${date}&time=${time}&ticker=${ticker}`
        const response = await fetch(url)
        const data = await response.json()
        setOrderbook(data)
    } catch (error) {
        console.log(error)
    }
}

    return (
        <div>
            <label> Enter date in YYYY-MM-DD Format</label>
                <input onChange = {handleDateChange}/>
            <label> Enter time in HH:MM:SS Format</label>
                <input onChange = {handleTimeChange}/>
            <label> Enter Ticker Symbol</label>
                <input onChange = {handleTickerChange}/> 
            <button onClick = {handleClick}> Submit </button>
            <h1> hello </h1>
            {orderbook && <pre>{JSON.stringify(orderbook)} </pre>}
        </div> 
    )
}