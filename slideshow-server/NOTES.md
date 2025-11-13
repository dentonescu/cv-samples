# NOTES

Developer notes for `slideshow-server`, the Python HTTP slideshow generator. This documents the build/test plan, design decisions, and future enhancements.

## Current stage
- Feature-complete for demos: FastAPI backend, CLI, Docker packaging, and pytest suites are live.
- Current focus is UI polish, caching options, and broader test coverage while keeping deployment guides current.

## Current File Layout
```
slideshow-server/
├─ README.md                     # Project overview and quickstart
├─ NOTES.md                      # Developer notes and roadmap
├─ VERSION                       # Release number
├─ Dockerfile
├─ Makefile
├─ run-demo.sh                   # Launches bundled demo slideshow
├─ slideshow-server.py           # Legacy runner wrapper
├─ slideshow-server.html         # Static demo page
├─ slideshow-server.css
├─ slideshow-server.js
├─ slideshow_server/             # Python package
│  ├─ __init__.py
│  ├─ __main__.py
│  ├─ server.py                  # FastAPI app and routes
│  ├─ media.py                   # Slideshow domain logic
│  ├─ cli.py                     # CLI helpers
│  └─ README.md
├─ tests/
│  ├─ README.md
│  ├─ test_media.py
│  └─ test_server.py
├─ img/
│  ├─ README.md
│  ├─ slideshow-server-01.png
│  └─ slideshow-server-02.png
├─ publicdomainpictures.net/     # Bundled sample media
│  ├─ README.md
│  ├─ beach-wallpaper-1862007223.jpg
│  ├─ nature-wallpaper-2924502136.jpg
│  └─ winter-wallpaper-3962742168.jpg
├─ dist/
│  ├─ slideshow-server-0.1.0/    # Packaged distribution contents
│  └─ slideshow-server-0.1.0.tar.gz
├─ build/
│  └─ slideshow_server/…         # PyInstaller/bytecode artifacts
├─ .gitignore / .gitattributes
└─ slideshow-server.logo         # Branding asset
```

## Implementation Plan
1. **Core server polish**  
   - Keep the single-file entry point (`slideshow-server.py`) lean; extract reusable pieces into `slideshow_server/` without breaking CLI simplicity.  
   - Add graceful shutdown handling and better logging (structured messages, request summary).  
   - Support additional media metadata (captions, EXIF-based ordering) while keeping the default random shuffle.
2. **UI/UX enhancements**  
   - Improve responsive layout for mobile (touch gestures already supported; verify on narrow screens).  
   - Add optional overlay for filenames/descriptions toggled by keyboard.  
   - Provide theming support and keyboard shortcuts cheat sheet.
3. **Performance & caching**  
   - Implement simple caching headers and conditional GET support to avoid re-downloading assets.  
   - Consider pre-generating thumbnails for large directories with a background worker.  
   - Document best practices when serving from slow storage or remote mounts.
4. **Testing & QA**  
   - Maintain pytest suite in `tests/` covering routing, directory scanning, and error cases.  
   - Add integration test using `httpx` or `requests` + `pytest-asyncio` to verify static asset delivery.  
   - Ensure Docker image runs tests as part of `docker build --target test`.
5. **Packaging & distribution**  
   - Keep `make dist` generating tarballs with source, assets, and demo media.  
   - Provide `pipx`/`pip` install instructions that expose a console script entry point.  
   - Document Docker usage (`docker compose up --build slideshow-server`) and environment variables.

## Domain Notes
- Relies on Python’s `http.server` + custom routing; ensure MIME types cover common image/video formats.  
- `ffmpeg` is required for streaming/transcoding certain videos—document fallback behaviour when unavailable.  
- Watch out for blocking file IO; consider threadpool if future features require concurrent processing.

## Implementation Timeline
- **2025-07-26 – 2025-07-27: MVP feature burst**  
  Initial commit and first playable server shipped core slideshow functionality, quickly followed by auth/context support, responsive image scaling, fullscreen, keyboard navigation, and mobile swipe gestures. Video streaming via `ffmpeg` arrived the next day with graceful teardown and on-screen volume controls.
- **2025-07-30 – 2025-08-29: Operational hardening**  
  Auto-restart tolerances, nginx configuration, numeric keypad shortcuts, and pause controls rounded out the first deployment-ready build.
- **2025-10-08 – 2025-11-02: Portfolio refinement**  
  Assets split into dedicated CSS/HTML/JS files, private artifacts were scrubbed, and documentation refreshed. The November work added `.gitattributes`, pytest coverage with refactors, bytecode build targets, and Docker packaging to align with the broader portfolio tooling.

## Testing Checklist
- `make test` and `pytest` pass; coverage stays above agreed threshold.  
- `run-demo.sh` works on a clean machine (ensures sample media packaged correctly).  
- Docker build/test targets complete successfully before publishing updated images.

## Related Documentation
- Project overview: [README.md](README.md)  
- Portfolio context: [../README.md](../README.md)
