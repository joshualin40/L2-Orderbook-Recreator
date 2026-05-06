"use client"

import { useState } from "react"
import './SearchBarContainer.css'
import {SearchBar} from './SearchBar'


export const SearchBarContainer = () => {
   

    return (<div className="search-bar-container">
                <SearchBar/>
            </div>
        )
}
