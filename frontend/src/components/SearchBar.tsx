import React from "react";

import './Searchbar.css'
import { useState, useEffect } from "react"



export const SearchBar = () => {
    const MAX_SIZE = 5; 
    const [input, setInput] = useState("")
    const [tickers, setTickers] = useState<string[]>([])
    const [filtered, setFiltered] = useState<string[]>([])
    const [recent, setRecent] = useState<string[]>([])

    function handleChange(event: React.ChangeEvent<HTMLInputElement>)
    {
        const newInput = event.target.value
        setInput(newInput); 
        setFiltered(tickers.filter((ticker) => ticker.startsWith(newInput.toUpperCase())).slice(-MAX_SIZE))
    }

    function hasInput()
    {
        return (input != "")
    }

    function handleClick(item) 
    {
        setInput(item)
        setFiltered([]) 
        setRecent([...recent, item])
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
        <div className="input-wrapper">
            <img src="/search-button.svg" width={15} height={15} alt="search" />
            <input
            value={input}
            placeholder="Search stock or company name..."
            onChange={handleChange}
            />
            {hasInput() && filtered.length > 0 && (
            <div className="result-wrapper">
                <ul>
                {filtered.map((item, index) => (
                    <li
                    className="listItem"
                    key={index}
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

