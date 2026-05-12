"use client"

import React from "react";
import Calendar from 'react-calendar'
import './DatePicker.css'

interface DatePickerProps {
  onDateChange: (value: Date) => void;
}

export const DatePicker = ({onDateChange}) => {
    const startDate = new Date(2018, 5, 1)
    const endDate = new Date()

    function handleClick(value: Date)
    {
        let year = value.getFullYear().toString().padStart(2, '0')
        let month = (value.getMonth() + 1).toString().padStart(2, '0')
        let day = (value.getDate()).toString().padStart(2, '0')
        onDateChange(year + "-" + month + "-" + day) // In JavaScript/TypeScript, 
        // {year} by itself creates an object with a property called year — it's shorthand for { year: year }
    }

    return (
        <div className = "parent"> 
            <Calendar 
            calendarType="hebrew"
            minDate = {startDate}
            maxDate = {endDate}
            onClickDay = {handleClick}
            tileDisabled={({ date }) => date.getDay() === 0 || date.getDay() === 6}/>
        </div> 
    )

}