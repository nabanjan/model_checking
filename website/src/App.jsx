import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Home from './Home'
import FormalVerification from './FormalVerification'
import ModelChecking from './ModelChecking'

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/formal-verification" element={<FormalVerification />} />
        <Route path="/formal-verification/model-checking" element={<ModelChecking />} />
      </Routes>
    </BrowserRouter>
  )
}

export default App
