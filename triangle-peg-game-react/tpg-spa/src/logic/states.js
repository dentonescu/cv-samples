// states.js: Board states

let cache = null;
let pending = null;

export function loadStates() {
    if (cache) return Promise.resolve(cache);
    if (pending) return pending;
    pending = fetch('/states.json')
        .then(res => {
            if (!res.ok) throw new Error(`HTTP ${res.status}`);
            return res.json();
        })
        .then(json => {
            cache = json;
            pending = null;
            return cache;
        })
        .catch(err => {
            pending = null;
            console.dir(err);
            throw err;
        });
    return pending;
}
