# Examples

Runnable demos built on top of **libdmotservices-c**.

| Example | Description |
|----------|--------------|
| `ex_signals` | Generates mock sine-wave signal data. |
| `ex_ui` | Minimal UI rendering test. |



## How to run

```bash
cd bin
./ex_signals
./ex_ui
```

All examples are self-contained and rely only on the static library.

## Screenshots

Screenshots.

### equalizer

![Text-mode equalizer](../img/equalizer.png)


## Notes

- Demonstrates real-time terminal visualization in pure C.
- Auto-detects terminal width.  
- ASCII characters ensure portability (`#` and `-`).  

