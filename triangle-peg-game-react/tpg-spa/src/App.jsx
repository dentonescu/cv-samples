import { useEffect, useState } from 'react';
import './App.css'
import Board from './components/Board'
import StatusBar from './components/StatusBar'
import { STATE_ALL_PEGS } from './logic/game'
import { loadStates } from './logic/states'

function App() {
  const [states, setStates] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [boardState, setBoardState] = useState(STATE_ALL_PEGS);

  useEffect(() => {
    loadStates()
      .then(setStates)
      .catch(setError)
      .finally(() => setLoading(false));
  }, []); // [] is an empty dependency array (i.e., run once)

  return (
    <main className="app-shell">
      <h1>Triangle Peg Game (React)</h1>

      {/* Loading message */}
      {loading && <p>Loading game states ...</p>}

      {/* Loading error message */}
      {!loading && error && <p className="error">{String(error)}</p>}

      {/* Game UI */}
      {!loading && !error && <Board boardState={boardState} states={states} setBoardState={setBoardState} />}
      {!loading && !error && <StatusBar boardState={boardState} states={states} />}

    </main>
  );
}

export default App;
