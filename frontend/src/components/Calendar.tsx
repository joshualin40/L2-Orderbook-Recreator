"use client"

import React from "react";
import { useState } from "react"
import './Calendar.css'

export const Calendar = () => {
    const [date, setDate] = useState(new Date())
    const daysInMonth = new Date(date.getFullYear(), date.getMonth() + 1, 0).getDate();
    const days = Array.from({ length: daysInMonth }, (_, i) => i + 1);

    function changeMonth(val: number)
    {
        const newDate = new Date(date);
        newDate.setMonth(date.getMonth() + val);
        setDate(newDate);
    }



    return (
        <div className="calendar-wrapper">
            <div className="header">
                <button onClick={() => changeMonth(-1)}>←</button>
                    <h1>
                        {date.toLocaleString('default', { month: 'long' })} {date.getDate()}
                    </h1>
                <button onClick={() => changeMonth(1)}>→</button>
            </div>
            <div className="days-row">
                <label> Su </label>
                <label> Mo </label>
                <label> Tu </label>
                <label> We </label>
                <label> Th </label>
                <label> Fr </label>
                <label> Sa </label>
            </div>
            <div className="days-grid">
                {days.map((day) => (
                <button key={day}>{day}</button>
                ))}
            </div>
        </div>
    )

}