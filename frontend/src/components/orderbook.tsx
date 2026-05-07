"use client"

import {useEffect, useState} from "react"

export const Orderbook = () =>
{
    const [data, setData] = useState(null)

    useEffect(() => {
        fetch('/api/orderbook')
      .then(res => res.json())
      .then(data => setData(data))
      .catch(err => console.error(err))
    }, [])


}