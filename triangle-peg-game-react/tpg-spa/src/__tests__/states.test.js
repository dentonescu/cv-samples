import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const statesPath = path.resolve(__dirname, '../../public/states.json');
const states = JSON.parse(fs.readFileSync(statesPath, 'utf-8'));

assert.equal(states.length, 32768, 'expected 2^15 states');
assert.equal(states[0].state, 0, 'first state should be 0');
assert.equal(states[states.length - 1].state, 32767, 'last state should be 32767');

for (const s of states) {
  assert.equal(s.state >>> 15, 0, 'state should not have bits outside 15-hole board');
  assert.equal(typeof s.peg_count, 'number', 'peg_count should be a number');
  assert.ok(Array.isArray(s.future_states), 'future_states should be an array');
}

console.log('Precomputed states sanity checks passed.');
