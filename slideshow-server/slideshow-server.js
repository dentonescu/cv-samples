function resizeMedia() {
    const media = document.getElementById('media');
    const video = document.getElementById('video');
    const vw = window.innerWidth;
    const vh = window.innerHeight;
    const element = media.style.display !== 'none' ? media : video;

    if (!element || (!element.naturalWidth && !element.videoWidth)) return;

    const naturalWidth = element.naturalWidth || element.videoWidth;
    const naturalHeight = element.naturalHeight || element.videoHeight;

    const aspectRatio = naturalWidth / naturalHeight;
    const viewportRatio = vw / vh;

    let width, height;

    if (aspectRatio > viewportRatio) {
        width = vw;
        height = vw / aspectRatio;
    } else {
        height = vh;
        width = vh * aspectRatio;
    }

    element.style.width = width + 'px';
    element.style.height = height + 'px';
}

function enterFullscreen() {
        const docEl = document.documentElement;
        if (docEl.requestFullscreen) {
                docEl.requestFullscreen();
        } else if (docEl.webkitRequestFullscreen) {
                docEl.webkitRequestFullscreen();
        } else if (docEl.msRequestFullscreen) {
                docEl.msRequestFullscreen();
        }
}

document.addEventListener('DOMContentLoaded', function () {
        window.addEventListener('resize', resizeMedia);
        document.getElementById('media').addEventListener('load', resizeMedia);
        document.getElementById('video').addEventListener(
            'loadedmetadata', resizeMedia);
        resizeMedia();

document.addEventListener('click', () => {
        enterFullscreen();
        const vid = document.getElementById("video");
        vid.muted = false;
        vid.volume = 0.1;
}, {once: false});

});


// The constants FILES and INTERVAL are declared outside this script.
 
let currentTimeout;
let currentPosition = -1;
let filesShown = [];
let overlayTimeout;
let isSlideShowPaused = false;

function showPrev() {
		if (isSlideShowPaused)
			return;
        if (currentPosition > 0) {
                currentPosition -= 2; // we go back two because showNext goes up by one
                showNext();
        }
}

function togglePause() {
	if (isSlideShowPaused) {
		unpauseSlideShow();
		if (currentTimeout) {
            clearTimeout(currentTimeout);
    	}
    	currentTimeout = setTimeout(showNext, INTERVAL);
	} else {
		pauseSlideShow();
	}
}

function pauseSlideShow() {
	isSlideShowPaused = true;
}

function unpauseSlideShow() {
	isSlideShowPaused = false;
}

function showNext() {
    if (currentTimeout) {
            clearTimeout(currentTimeout);
    }
	if (isSlideShowPaused) {
		return;
	}
    ++currentPosition;
    let file = '';
    if (currentPosition >= filesShown.length) {
            file = FILES[Math.floor(Math.random() * FILES.length)];
            filesShown.push(file);
    } else {
            file = filesShown[currentPosition];
    }
    const img = document.getElementById("media");
    const vid = document.getElementById("video");
    if (file.match(/\\.(3g2|3gp|avi|flv|m4a|mkv|mov|mp4|mpg|ogg|webm|wmv)$/i)) {
        vid.src = file;
        vid.style.display = "block";
        img.style.display = "none";
        vid.onended = showNext;
        vid.play();
    } else {
        img.src = file;
        img.style.display = "block";
        vid.style.display = "none";
        currentTimeout = setTimeout(showNext, INTERVAL);
    }
}

function volumeUp() {
    const video = document.getElementById('video');
    video.volume += 0.1;
    if (video.volume > 1)
        video.volume = 1;
    showVolumeOverlay(video.volume);
}

function volumeDown() {
    const video = document.getElementById('video');
    video.volume -= 0.1;
    if (video.volume < 0)
        video.volume = 0;
    showVolumeOverlay(video.volume);
}

function showVolumeOverlay(volume) {
        volumeOverlay.textContent = Math.round(volume * 100) + '%';
        volumeOverlay.style.opacity = 1;
        clearTimeout(overlayTimeout);
        overlayTimeout = setTimeout(() => {
                volumeOverlay.style.opacity = 0;
        }, 1000);
}

document.body.addEventListener("keydown",
        function (e) {
                if (e.key === 'ArrowLeft' || e.key === '4') {
                        showPrev();
                } else if (e.key === 'ArrowRight' || e.key === '6') {
                        showNext();
                } else if (e.key === 'ArrowUp' || e.key === '2') {
                        volumeUp();
                } else if (e.key === 'ArrowDown' || e.key === '8') {
                        volumeDown();
                } else if (e.key === ' ') {
                	e.preventDefault(); // stops page scrolling when space is pressed
                	togglePause();
                }
        }
);

let touchStartX = 0;
let touchEndX = 0;
let touchStartY = 0;
let touchEndY = 0;

document.addEventListener("touchstart", e => {
                pauseSlideShow();
                touchStartX = e.changedTouches[0].screenX;
                touchStartY = e.changedTouches[0].screenY
        }
);

document.addEventListener("touchend", e => {
                unpauseSlideShow();
                touchEndX = e.changedTouches[0].screenX;
                touchEndY = e.changedTouches[0].screenY;
                if (touchEndX < touchStartX - 50) {
                        showNext();	// swipe left
                } else if (touchEndX > touchStartX + 50) {
                        showPrev();	// swipe right
                }
                const deltaX = Math.abs(touchEndX - touchStartX);
                const deltaY = Math.abs(touchEndY - touchStartY);
                if (deltaY > deltaX) {
                        // make sure the user is primarily swiping vertically
                        if (touchEndY < touchStartY - 50) {
                                volumeUp(); // swipe up
                        } else if (touchEndY > touchStartY + 50) {
                                volumeDown(); // swipe down
                        }
                }
        }
);

showNext();
