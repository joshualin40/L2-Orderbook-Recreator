"use client"

import {SearchBar} from '@/components/SearchBar'
import {DatePicker} from '@/components/DatePicker'
import {TimePicker} from '@/components/TimePicker'
import {Orderbook} from '@/components/Orderbook'
import './page.css'
import { Inter } from "next/font/google"
import {useState} from "react"

const inter = Inter({ subsets: ["latin"] })

export default function Page() {
  const [ticker, setTicker] = useState("")
  const [date, setDate] = useState("") // string
  const [time, setTime] = useState("")
  const [orderbook, setOrderbook] = useState("") // the orderbook Data itself

  
  async function fetchData() {
    if (!ticker || !date || !time) return
    console.log("fetchData called", { ticker, date, time })
      try {
          const response = await fetch(`/api/orderbook?date=${date}&time=${time}&ticker=${ticker}`)
          const data = await response.json()
          setOrderbook(data) // orderbook is a JS object
      } catch (error) {
          console.log(error)
      }
  }

  return (
    <main>
      <div className="searchbar-wrapper"> 
        <SearchBar onTickerChange={setTicker}/> 
      </div> 
      <div className="datepicker-wrapper">  
        <DatePicker onDateChange={setDate}/>
      </div> 
      <div className="timepicker-wrapper"> 
        <TimePicker onTimeChange={setTime} onSubmit = {fetchData}/> 
      </div> 
      {orderbook && 
      <div className="orderbook-wrapper"> 
        <Orderbook tickerName = {ticker} orderbookData = {orderbook}/>
      </div> }
    </main>
  )
}
// <Calendar onDateChange={setDate}/>
// <TimePicker onTimeChange={setTime}/>
// <Button onClick={handleClick}/> 
// {orderbook && <Orderbook data = {orderbook}/>}