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
	
	function reset() {
    	inputBox.value = '';
    	cipherKeyBox.value = '';
    	outputBox.value = '';
	}

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

	function getCipherDigit(cipherkey, index) {
    	return parseInt(cipherkey.charAt(index % cipherkey.length), 10);
	}

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
	
	function padding(digits) {
    	let text = "";
    	const ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    	for (let i = 0; i < digits; ++i)
        	text += ALPHABET.charAt(Math.floor(Math.random() * ALPHABET.length));
    	return text;
	}

});
