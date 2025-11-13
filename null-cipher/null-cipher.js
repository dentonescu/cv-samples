/**
 * Null cipher demo wiring: attaches UI handlers and exposes encrypt/decrypt helpers.
 * The numeric key dictates how many filler letters are inserted or skipped.
 */
document.addEventListener('DOMContentLoaded', () => {

	/*
	 * Elements
	 */

	const nullCipherForm = document.querySelector('#nullCipherForm');
	const inputBox = document.querySelector('#input');
	const cipherKeyBox = document.querySelector('#cipherKey');
	const outputBox = document.querySelector('#output');
	const btnReset = document.querySelector('#btnReset');
	const btnEncrypt = document.querySelector('#btnEncrypt');
	const btnDecrypt = document.querySelector('#btnDecrypt');
	
	/*
	 * Event listeners
	 */
	 
	nullCipherForm.addEventListener('submit', (event) => {
		event.preventDefault(); // no submitting or reloading the page
	});
	
	// only allow digits in the cipher key box
	cipherKeyBox.addEventListener('input', () => {
		cipherKeyBox.value = cipherKeyBox.value.replace(/[^0-9]/g, '');
	});
	
	btnReset.addEventListener('click', () => {
		reset();
	});
	
	btnEncrypt.addEventListener('click', () => {
		encrypt();
	});
	
	btnDecrypt.addEventListener('click', () => {
		decrypt();
	});
	
	/*
	 * Functions
	 */
	
	/**
	 * Clear the UI so the next demo starts from a blank slate.
	 */
	function reset() {
    	inputBox.value = '';
    	cipherKeyBox.value = '';
    	outputBox.value = '';
	}

	/**
	 * Validate and normalise the cipher key supplied by the user.
	 * @returns {string|null} Sanitised key or null when validation fails.
	 */
	function getCipherKey() {
    	const cipherkey = cipherKeyBox.value.trim();
    	if (cipherkey.length === 0) {
        	window.alert('Please enter a cipher key (digits only) before continuing.');
        	return null;
    	}
    	if (!/^\d+$/.test(cipherkey)) {
        	window.alert('Cipher key must contain digits only.');
        	return null;
    	}
    	return cipherkey;
	}

	/**
	 * Return the numeric digit for a given cipher index, wrapping around as needed.
	 * @param {string} cipherkey Numeric key string, e.g. '3011'.
	 * @param {number} index Zero-based index into the repeating key.
	 * @returns {number} Parsed digit value between 0 and 9.
	 */
	function getCipherDigit(cipherkey, index) {
    	return parseInt(cipherkey.charAt(index % cipherkey.length), 10);
	}

	/**
	 * Recover the hidden message by skipping characters according to the key.
	 */
	function decrypt() {
    	const cipherkey = getCipherKey();
    	if (!cipherkey) {
        	return;
    	}
    	const ciphertext = inputBox.value.toUpperCase().replace(/\s+/g, '');
    	let result = '';
    	let j = 0;
    	for (let i = 0; i < ciphertext.length; ++i) {
        	if (i !== 0) {
            	const digit = getCipherDigit(cipherkey, j);
            	i += digit;
            	++j;
        	}
        	result += ciphertext.charAt(i);
    	}

    	outputBox.value = result;
	}

	/**
	 * Produce cover text by interleaving plaintext letters with random padding.
	 */
	function encrypt() {
    	const cipherkey = getCipherKey();
    	if (!cipherkey) {
        	return;
    	}
    	const plaintext = inputBox.value.toUpperCase().replace(/[^A-Z]+/g, '');
    	let result = '';
    	let j = 0;
    	for (let i = 0; i < plaintext.length; ++i) {
        	if (i !== 0) {
            	const digit = getCipherDigit(cipherkey, j);
            	result += padding(digit);
            	++j;
        	}
        	result += plaintext.charAt(i);
    	}
    	outputBox.value = result;
	}
	
	/**
	 * Generate ``digits`` random filler letters to disguise the signal text.
	 * @param {number} digits Amount of padding to add before the next plaintext letter.
	 * @returns {string} Randomised uppercase filler string.
	 */
	function padding(digits) {
    	let text = "";
    	const ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    	for (let i = 0; i < digits; ++i)
        	text += ALPHABET.charAt(Math.floor(Math.random() * ALPHABET.length));
    	return text;
	}

});
