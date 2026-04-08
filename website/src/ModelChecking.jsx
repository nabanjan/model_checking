import { Link } from 'react-router-dom'
import { useState, useRef, useEffect } from 'react'

export default function ModelChecking() {
  const [formula, setFormula] = useState('')
  const [output, setOutput] = useState('')
  const [loading, setLoading] = useState(false)
  const outputRef = useRef(null)

  useEffect(() => {
    if (outputRef.current) {
      outputRef.current.scrollTop = 0
    }
  }, [output])

  const runModelCheck = async () => {
    setLoading(true)
    setOutput('Running model checker...\n\n')
    
    try {
      const res = await fetch('/api/run-model-check', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ formula })
      })
      const text = await res.text()
      try {
        const data = JSON.parse(text)
        setOutput(data.success ? data.output : `Error: ${data.output}`)
      } catch {
        setOutput(`Server error: ${text}`)
      }
    } catch (err) {
      setOutput(`Error: ${err.message}`)
    }
    
    setLoading(false)
  }

  return (
    <div className="min-h-screen bg-gray-100 p-8">
      <Link 
        to="/formal-verification" 
        className="text-blue-600 hover:text-blue-800 mb-6 inline-block"
      >
        ← Back to Formal Verification
      </Link>
      
      <div className="max-w-4xl mx-auto">
        <h1 className="text-4xl font-bold text-gray-900 mb-8">Model Checking</h1>
        
        <div className="bg-white rounded-lg shadow-lg p-8 mb-8">
          <h2 className="text-2xl font-bold text-green-600 mb-4">What is Model Checking?</h2>
          <p className="text-gray-700 mb-4">
            Model checking is an automated verification technique that systematically explores all possible 
            states of a system to determine if a given specification (usually expressed in temporal logic) 
            holds true. It was pioneered by Edmund Clarke, E. Allen Emerson, and Joseph Sifakis, who 
            received the Turing Award for this work.
          </p>
        </div>

        <div className="bg-white rounded-lg shadow-lg p-8 mb-8">
          <h2 className="text-2xl font-bold text-gray-900 mb-4">Key Concepts</h2>
          
          <div className="space-y-6">
            <div>
              <h3 className="text-xl font-semibold text-purple-600 mb-2">State Space</h3>
              <p className="text-gray-700">
                The collection of all possible states that a system can be in. Model checking exhaustively 
                explores this space to verify properties.
              </p>
            </div>

            <div>
              <h3 className="text-xl font-semibold text-blue-600 mb-2">Temporal Logic</h3>
              <p className="text-gray-700">
                A formal language used to express properties over time. Common logics include CTL 
                (Computation Tree Logic) and LTL (Linear Temporal Logic).
              </p>
            </div>

            <div>
              <h3 className="text-xl font-semibold text-orange-600 mb-2">State Transition System (STS)</h3>
              <p className="text-gray-700">
                A mathematical model consisting of states and transitions between them, representing the 
                behavior of a system.
              </p>
            </div>
          </div>
        </div>

        <div className="bg-green-50 border-l-4 border-green-500 p-6 rounded mb-8">
          <h3 className="text-xl font-bold text-gray-900 mb-3">CTL Operators</h3>
          <div className="grid grid-cols-2 gap-4 text-gray-700">
            <div><strong>EX φ</strong> - Exists a path where φ holds next</div>
            <div><strong>AX φ</strong> - All paths: φ holds next</div>
            <div><strong>EF φ</strong> - Exists a path where φ eventually holds</div>
            <div><strong>AF φ</strong> - All paths: φ eventually holds</div>
            <div><strong>EG φ</strong> - Exists a path where φ always holds</div>
            <div><strong>AG φ</strong> - All paths: φ always holds</div>
            <div><strong>E[φ U ψ]</strong> - Exists path: φ holds until ψ holds</div>
            <div><strong>A[φ U ψ]</strong> - All paths: φ holds until ψ holds</div>
          </div>
        </div>

        <div className="bg-white rounded-lg shadow-lg p-8 mb-8">
          <h2 className="text-2xl font-bold text-gray-900 mb-4">Our Implementation</h2>
          <p className="text-gray-700 mb-4">
            This project implements a CTL model checker that reads State Transition Systems from files 
            and validates CTL formulas against them.
          </p>
          <div className="bg-gray-100 rounded p-4 font-mono text-sm">
            <p className="text-gray-800">Formula syntax:</p>
            <ul className="text-gray-600 list-disc list-inside mt-2">
              <li>Atomic propositions: lowercase letters (a, b, c...)</li>
              <li>Negation: !</li>
              <li>Conjunction: .</li>
              <li>Disjunction: +</li>
              <li>AF, EF, EG, AG, EU: temporal operators</li>
            </ul>
          </div>
        </div>

        <div className="bg-white rounded-lg shadow-lg p-8">
          <h2 className="text-2xl font-bold text-gray-900 mb-6">Demo</h2>
          
          <div className="mb-4">
            <label className="block text-gray-700 font-semibold mb-2">Formula:</label>
            <input
              type="text"
              value={formula}
              onChange={(e) => setFormula(e.target.value)}
              placeholder="e.g., AF!c0 or EU!c2Uc0.c1"
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-green-500 font-mono"
            />
          </div>
          
          <button
            onClick={runModelCheck}
            disabled={loading}
            className="bg-green-600 text-white px-6 py-3 rounded-lg font-semibold hover:bg-green-700 disabled:bg-gray-400 transition-colors"
          >
            {loading ? 'Running...' : 'Run Model Checker'}
          </button>
          
          {output && (
            <div className="mt-6">
              <h3 className="text-lg font-semibold text-gray-900 mb-2">Output:</h3>
              <pre ref={outputRef} className="bg-gray-900 text-green-400 p-4 rounded-lg overflow-x-auto overflow-y-auto font-mono text-sm max-h-64">
                {output}
              </pre>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}
