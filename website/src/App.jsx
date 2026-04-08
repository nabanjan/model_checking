export default function App() {
  return (
    <div className="min-h-screen bg-gray-100 p-8">
      <h1 className="text-4xl font-bold text-gray-900 text-center mb-8">Model Checking Dashboard</h1>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6 max-w-6xl mx-auto">
        {/* Synthesis */}
        <div className="bg-blue-500 text-white rounded-lg p-8 shadow-lg hover:shadow-xl transition-shadow">
          <h2 className="text-2xl font-bold mb-4">Synthesis</h2>
          <p className="text-blue-100">Transform high-level specifications into circuit implementations.</p>
        </div>

        {/* Formal Verification */}
        <div className="bg-green-500 text-white rounded-lg p-8 shadow-lg hover:shadow-xl transition-shadow">
          <h2 className="text-2xl font-bold mb-4">Formal Verification</h2>
          <p className="text-green-100">Mathematically prove correctness of systems against specifications.</p>
        </div>

        {/* Placement */}
        <div className="bg-purple-500 text-white rounded-lg p-8 shadow-lg hover:shadow-xl transition-shadow">
          <h2 className="text-2xl font-bold mb-4">Placement</h2>
          <p className="text-purple-100">Optimize component positioning for performance and efficiency.</p>
        </div>

        {/* Routing */}
        <div className="bg-orange-500 text-white rounded-lg p-8 shadow-lg hover:shadow-xl transition-shadow">
          <h2 className="text-2xl font-bold mb-4">Routing</h2>
          <p className="text-orange-100">Plan and optimize signal paths between circuit components.</p>
        </div>
      </div>
    </div>
  )
}
