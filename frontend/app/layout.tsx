import type { Metadata } from 'next'
import './globals.css'

export const metadata: Metadata = {
  title: 'CTA Bus Predictor',
  description: 'Find CTA bus stops and arrival predictions',
}

export default function RootLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <html lang="en">
      <body>{children}</body>
    </html>
  )
}

