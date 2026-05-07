import {SearchBarContainer} from '@/components/SearchBarContainer'
import {Calendar} from '@/components/Calendar'
import './page.css'
import { Inter } from "next/font/google"

const inter = Inter({ subsets: ["latin"] })

export default function Page() {
  return (
    <main>
      <SearchBarContainer/>
      <Calendar/> 
    </main>
  )
}