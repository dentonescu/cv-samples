import { useState } from 'react';
import './App.css';
import { generateText } from './api';

function App() {
  const [isStatusOK, setIsStatusOK] = useState(true);
  const [isLoading, setIsLoading] = useState(false);
  const [prompt, setPrompt] = useState('');
  const [response, setResponse] = useState<string | null>(null);
  const [status, setStatus] = useState<string | null>(null);

  const handleSubmit = async (
    event: React.SyntheticEvent<HTMLFormElement>
  ) => {
    const msSubmissionStarted = Date.now();
    event.preventDefault();
    if (!prompt.trim()) {
      return;
    }
    setIsLoading(true);
    setStatus(null);
    try {
      const result = await generateText(prompt.trim());
      const msResponseDuration = Date.now() - msSubmissionStarted;
      setIsStatusOK(true);
      setResponse(result.output);
      setStatus(`Duration: ${msResponseDuration} ms.`);
    } catch (error) {
      const message =
        error instanceof Error ? error.message : 'Request failed';
      setIsStatusOK(false);
      setResponse(null);
      setStatus(message);
    } finally {
      setIsLoading(false);
    }
  };

  const statusClass = isStatusOK ? 'status status-ok' : 'status status-failure';

  return (
    <div className="app">
      <header className="app-header">
        <div className="title-stack">
          <h1>pwa-llm-poc</h1>
          <p className="subtitle">
            Simple UI for calling the Ollama backend.
          </p>
        </div>
      </header>

      <main className="panel">
        <form className="prompt-form" onSubmit={handleSubmit}>
          <label htmlFor="prompt">Prompt</label>
          <textarea
            id="prompt"
            name="prompt"
            value={prompt}
            onChange={(event: React.ChangeEvent<HTMLTextAreaElement>) =>
              setPrompt(event.target.value)
            }
            rows={5}
            placeholder="Enter a prompt for the LLM to process."
          />
          <div className="form-row">
            <button type="submit" disabled={isLoading || !prompt.trim()}>
              {isLoading ? 'Processing...' : 'Ask'}
            </button>
          </div>
        </form>

        <section className="response">
          <div className="response-header">
            <label htmlFor="response-body">Response</label>
            {isLoading ? <span className="loading">Thinking</span> : null}
          </div>
          <pre id="response-body" className="response-body">
            {response ?? 'No response yet.'}
          </pre>
          {status && (<div className={statusClass}>{status ?? ''}</div>)}
        </section>
      </main>
    </div>
  );
}

export default App;
