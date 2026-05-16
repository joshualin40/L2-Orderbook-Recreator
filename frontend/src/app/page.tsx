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
  const [orderbook, setOrderbook] = useState(null) // the orderbook Data 
  const [loading, setLoading] = useState(false)
  const [error, showError] = useState(false)
  const [loaded, setLoaded] = useState(false)
  const [empty, setEmpty] = useState(false)

  
  async function fetchData(currentTicker: string, currentDate: string, currenttime: string) {
    console.log("fetchData called", { currentTicker, currentDate, currenttime })
      try {
          showError(false)
          setOrderbook({bids: [], asks: []})
          setLoading(true)
          const response = await fetch(`/api/orderbook?date=${currentDate}&time=${currenttime}&ticker=${currentTicker}`)
          const data = await response.json()
          setOrderbook(data) // orderbook is a JS object
          setLoading(false)
          setLoaded(true)
      } catch (error) {
        console.log(error)
        showError(true)
        setLoaded(false)
      }
  }

  return (
    <main>
      <div className="searchbar-wrapper"> 
        <SearchBar orderbook = {orderbook} loaded = {loaded} onTickerChange={setTicker}/> 
      </div> 
      <div className="datepicker-wrapper">  
        <DatePicker onDateChange={setDate}/>
      </div> 
      <div className="timepicker-wrapper"> 
        <TimePicker onTimeChange={setTime} tickerName={ticker} date={date} onSubmit = {fetchData}/> 
      </div> 
      {loading && !error && <div>
        <p> Loading...</p> 
      </div>}
      {orderbook && 
      <div className="orderbook-wrapper"> 
        <Orderbook tickerName = {ticker} setEmpty = {setEmpty} orderbookData = {orderbook}/>
      </div> }
      {error && 
      <p> Error! Ensure that you have exported your API_KEY to the backend terminal and that your backend is running. </p>}
      {!loading && loaded && empty && <p> No Data</p> } 
    </main>
  )
}
// <Calendar onDateChange={setDate}/>
// <TimePicker onTimeChange={setTime}/>
// <Button onClick={handleClick}/> 
// {orderbook && <Orderbook data = {orderbook}/>}