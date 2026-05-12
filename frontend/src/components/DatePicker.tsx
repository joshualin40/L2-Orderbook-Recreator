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
        let year = value.getFullYear()
        let month = value.getMonth() + 1
        let day = value.getDay() 
        onDateChange({year} + "-" + {month} + "-" + {day})
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