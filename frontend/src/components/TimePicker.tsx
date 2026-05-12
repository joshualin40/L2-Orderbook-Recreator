"use client"

import {useState} from "react"
import './TimePicker.css'

export const TimePicker = ({onTimeChange, onSubmit}) => 
{
    const [hours, setHours] = useState("08") 
    const [minutes, setMinutes] = useState("01")
    const [seconds, setSeconds] = useState("01")
    const hoursList = Array.from({ length: 16}, (_, i) => i + 8)
    const minutesList = Array.from({ length: 60}, (_, i) => i + 1)
    const secondsList = Array.from({ length: 60}, (_, i) => i + 1)

    function handleClick()
    {
        const hourspadded = hours.toString().padStart(2, '0');
        const minutespadded = minutes.toString().padStart(2, '0');
        const secondspadded = seconds.toString().padStart(2, '0');
        onTimeChange(hourspadded + ":" + minutespadded + ":" + secondspadded)
        onSubmit()
    }

    function handleHourChange(event: React.ChangeEvent<HTMLSelectElement>)
    {
        setHours(event.target.value)
    }

    function handleMinuteChange(event: React.ChangeEvent<HTMLSelectElement>)
    {
        setMinutes(event.target.value)
    }

    function handleSecondChange(event: React.ChangeEvent<HTMLSelectElement>)
    {
        setSeconds(event.target.value)
    }
    return (
        <div className="wrapper"> 
            <div className="sessions">
                <div className="pill">
                    <div className="label">Pre-market</div>
                    <div className="time">08:00 – 13:30 UTC</div>
                </div>
                <div className="pill">
                    <div className="label">Main-session</div>
                    <div className="time">13:30 – 20:00 UTC</div>
                </div>
                <div className="pill">
                    <div className="label">After-hours</div>
                    <div className="time">20:00 – 00:00 UTC</div>
                </div>
            </div>
            <div className="time-input-wrapper"> 
                <select className="input-dropdown" onChange = {handleHourChange}>
                    {hoursList.map((item, index) => <option key={index} value={item}> {item.toString().padStart(2, '0')}</option>)}
                </select>
                <p> : </p> 
                <select className="input-dropdown" onChange = {handleMinuteChange}>
                    {minutesList.map((item,index) => <option key={index} value={item}> {item.toString().padStart(2, '0')}</option>)}
                </select> 
                <p> : </p> 
                <select className="input-dropdown" onChange = {handleSecondChange}> 
                    {secondsList.map((item,index) => <option key={index} value={item}>  {item.toString().padStart(2, '0')}</option>)}
                </select> 

                <button className="button" onClick={handleClick}> Load Orderbook </button> 
            </div>
                
            
        </div> 
    )
}
