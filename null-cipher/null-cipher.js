document.addEventListener('DOMContentLoaded', () => {
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
	
	function decrypt() {
    	let ciphertext = inputBox.value.toUpperCase().replace(/\s+/g, '');
    	let cipherkey = cipherKeyBox.value;
    	let result = '';
    	let j = 0;
    	for (let i = 0; i < ciphertext.length; ++i) {
        	if (i != 0) {
            	let digit = parseInt(cipherkey.charAt(j), 10);
            	i += digit;
            	++j;
        	}
        	result += ciphertext.charAt(i);
    	}
	
    	outputBox.value = result;
	}
	
	function encrypt() {
    	let plaintext = inputBox.value.toUpperCase().replace(/[^A-Z]+/g, '');
    	let cipherkey = cipherKeyBox.value;
    	let result = '';
    	let j = 0;
    	for (let i = 0; i < plaintext.length; ++i) {
        	if (i != 0) {
            	let digit = parseInt(cipherkey.charAt(j), 10);
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
