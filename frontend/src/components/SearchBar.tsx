import React from "react";

import './Searchbar.css'
import { useState, useEffect, useRef } from "react"




export const SearchBar = ({orderbook, onTickerChange}) => {
    const MAX_SIZE = 5; 
    const [input, setInput] = useState("")
    const [tickers, setTickers] = useState<string[]>([])
    const [filtered, setFiltered] = useState<string[]>([])
    const [recent, setRecent] = useState<string[]>([])
    const containerRef = useRef(null)
    const [showError, setShowError] = useState(false)
    const [showDropdown, setShowDropdown] = useState(false)
    const [isValid, setIsValid] = useState(true)


    function handleChange(event: React.ChangeEvent<HTMLInputElement>)
    {
        const newInput = event.target.value
        setShowDropdown(true)
        setInput(newInput); 
        setFiltered(tickers.filter((ticker) => ticker.startsWith(newInput.toUpperCase())).slice(-MAX_SIZE))
    }
    

    function hasInput()
    {
        return (input != "")
    }
    function handleClick(item: string) 
    {
        setShowError(false)
        setInput(item)
        setFiltered([]) 
        setRecent([...recent, item])
        if (!orderbook)
            onTickerChange(item)
    }
    function handleBlur(event)
    {   
        if (input == "") 
        {
            setShowError(false)
            return
        }
        if (containerRef.current.contains(event.relatedTarget)) return 
        setShowDropdown(false)
        if (isValidTicker(input))
        {
            onTickerChange(input)
            setShowError(false)
        }
        else
        {
            setShowError(true); 
        }
    }
    function isValidTicker(item: string) {
        if (!tickers.includes(item))
            setIsValid(false)
        return tickers.includes(item)
    }

    useEffect(() => { // performs side effects in function components
        const fetchTickers = async () => {
            const response = await fetch('https://raw.githubusercontent.com/rreichel3/US-Stock-Symbols/main/nasdaq/nasdaq_tickers.json')
            if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
            }
            const data = await response.json()
            setTickers(data)
        }
        fetchTickers()
    }, [])

    return (
        <div className="input-wrapper"
            onBlur={handleBlur}
            ref={containerRef}>
            <div className="search-wrapper"> 
                <img src="/search-button.svg" width={15} height={15} alt="search" />
                    <input
                    value={input}
                    placeholder="Search stock or company..."
                    onChange={handleChange}
                    />
                {showError && <span className="error-badge">Invalid ticker</span>}   
            </div>
            {hasInput() && showDropdown && filtered.length > 0 && (
            <div className="result-wrapper">
                <ul>
                {filtered.map((item, index) => (
                    <li
                    className="listItem"
                    key={index}
                    tabIndex={0}
                    onClick={() => handleClick(item)}
                    >
                    {item}
                    </li>
                ))}
                </ul>
            </div>
            )}
        </div>
        );
};

