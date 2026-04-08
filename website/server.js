import express from 'express'
import cors from 'cors'
import { exec } from 'child_process'
import path from 'path'
import { fileURLToPath } from 'url'

const __dirname = path.dirname(fileURLToPath(import.meta.url))

const app = express()
app.use(cors())
app.use(express.json())

app.post('/api/run-model-check', (req, res) => {
  const { formula } = req.body
  
  const cwd = path.resolve(__dirname, '..')
  const exePath = path.join(cwd, 'build', 'model_check')
  
  const cmd = `${exePath} "${formula || ''}" "${path.join(cwd, 'STS.txt')}" 2>&1`
  
  exec(cmd, { cwd, maxBuffer: 10 * 1024 * 1024 }, (error, stdout, stderr) => {
    if (error) {
      res.json({ success: false, output: stdout + stderr || error.message })
      return
    }
    res.json({ success: true, output: stdout + stderr })
  })
})

app.listen(3001, () => {
  console.log('Model check server running on port 3001')
})