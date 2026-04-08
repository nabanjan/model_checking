import { Link } from 'react-router-dom'

export default function FormalVerification() {
  return (
    <div className="min-h-screen bg-gray-100 p-8">
      <Link 
        to="/" 
        className="text-blue-600 hover:text-blue-800 mb-6 inline-block"
      >
        ← Back to Home
      </Link>
      
      <div className="max-w-4xl mx-auto">
        <h1 className="text-4xl font-bold text-gray-900 mb-8">Formal Verification</h1>
        
        <div className="bg-white rounded-lg shadow-lg p-8 mb-8">
          <h2 className="text-2xl font-bold text-green-600 mb-4">What is Formal Verification?</h2>
          <p className="text-gray-700 mb-4">
            Formal verification is the process of mathematically proving the correctness of a system 
            with respect to a formal specification. Unlike testing, which can only show the presence 
            of bugs, formal verification can prove their absence.
          </p>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          <Link to="/formal-verification/model-checking" className="bg-green-100 border-l-4 border-green-500 p-6 rounded hover:bg-green-200 transition-colors">
            <h3 className="text-xl font-bold text-gray-900 mb-3">Model Checking</h3>
            <p className="text-gray-700">
              Automatically explores all possible states of a system to verify temporal logic properties.
            </p>
          </Link>

          <div className="bg-blue-50 border-l-4 border-blue-500 p-6 rounded">
            <h3 className="text-xl font-bold text-gray-900 mb-3">Theorem Proving</h3>
            <p className="text-gray-700">
              Uses mathematical proofs to verify system correctness with respect to axioms and inference rules.
            </p>
          </div>

          <div className="bg-purple-50 border-l-4 border-purple-500 p-6 rounded">
            <h3 className="text-xl font-bold text-gray-900 mb-3">Static Analysis</h3>
            <p className="text-gray-700">
              Analyzes code without executing it to find potential errors and security vulnerabilities.
            </p>
          </div>

          <div className="bg-orange-50 border-l-4 border-orange-500 p-6 rounded">
            <h3 className="text-xl font-bold text-gray-900 mb-3">Abstract Interpretation</h3>
            <p className="text-gray-700">
              Over-approximates program behavior to verify properties over all possible executions.
            </p>
          </div>
        </div>
      </div>
    </div>
  )
}
