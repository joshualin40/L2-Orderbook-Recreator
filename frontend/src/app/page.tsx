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
  const [orderbook, setOrderbook] = useState({bids: [], asks: []}) // the orderbook Data 
  const [loading, setLoading] = useState(false)
  const [error, showError] = useState(false)

  
  async function fetchData(currenttime: string) {
    console.log("fetchData called", { ticker, date, time })
      try {
          showError(false)
          setOrderbook(null)
          setLoading(true)
          const response = await fetch(`/api/orderbook?date=${date}&time=${currenttime}&ticker=${ticker}`)
          const data = await response.json()
          setOrderbook(data) // orderbook is a JS object
          setLoading(false)
      } catch (error) {
        console.log(error)
        showError(true)
      }
  }

  return (
    <main>
      <div className="searchbar-wrapper"> 
        <SearchBar orderbook = {orderbook} onTickerChange={setTicker}/> 
      </div> 
      <div className="datepicker-wrapper">  
        <DatePicker onDateChange={setDate}/>
      </div> 
      <div className="timepicker-wrapper"> 
        <TimePicker onTimeChange={setTime} onSubmit = {fetchData}/> 
      </div> 
      {loading && !error && <div>
        <p> Loading...</p> 
      </div>}
      {orderbook && 
      <div className="orderbook-wrapper"> 
        <Orderbook tickerName = {ticker} orderbookData = {orderbook}/>
      </div> }
      {error && 
      <p> Error! Ensure that you have exported your API_KEY to the backend terminal and that your backend is running. </p>}
    </main>
  )
}
// <Calendar onDateChange={setDate}/>
// <TimePicker onTimeChange={setTime}/>
// <Button onClick={handleClick}/> 
// {orderbook && <Orderbook data = {orderbook}/>}