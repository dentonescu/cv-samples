# heapmonj frontend

Angular UI to poll the backend and chart JVM heap usage with Chart.js.

## Status
- Not yet scaffolded. Will consume `/api/v1/heap/current` and `/api/v1/heap/history` once backend is ready.

## Planned work
- Generate Angular project.
- Add models/services for heap endpoints; poll every few seconds.
- Build a Chart.js line chart for heap usage over time.
- Configure proxy/CORS to reach the backend; add Dockerfile for static hosting.
