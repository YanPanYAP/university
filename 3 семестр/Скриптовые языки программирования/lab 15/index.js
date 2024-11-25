document.getElementById('lastName').addEventListener('input', function() {
    if (this.value.length === 0) {
        this.style.borderColor = 'gray';
    } else if (this.value.length < 3) {
        this.style.borderColor = 'red';
    } else {
        this.style.borderColor = 'green';
    }
});

document.getElementById('name').addEventListener('input', function() {
    if (this.value.length === 0) {
        this.style.borderColor = 'gray';
    } else if (this.value.length < 2) {
        this.style.borderColor = 'red';
    } else {
        this.style.borderColor = 'green';
    }
});

document.getElementById('email').addEventListener('input', function() {
    const validDomains = ['@gmail.com', '@icloud.com'];
    const isValid = validDomains.some(domain => this.value.endsWith(domain));
    if (this.value.length === 0) {
        this.style.borderColor = 'gray';
    } else if (!isValid) {
        this.style.borderColor = 'red';
    } else {
        this.style.borderColor = 'green';
    }
});

document.getElementById('number').addEventListener('input', function() {
    const numberPattern = /^(80|\+375)(29|44|25)\d{7}$/;
    if (this.value.length === 0) {
        this.style.borderColor = 'gray';
    } else if (!numberPattern.test(this.value)) {
        this.style.borderColor = 'red';
    } else {
        this.style.borderColor = 'green';
    }
});

document.getElementById('myForm').addEventListener('submit', function(event) {
    const city = document.getElementById('city').value;
    const isBstu = document.getElementById('bstu').checked;
    const course = [...document.querySelectorAll('.radioKurs input')].find(radio => radio.checked)?.id === 'bstu3';

    if (city !== 'minsk' || !isBstu || !course) {
        const confirmation = confirm('Вы уверены в своём ответе?');
        if (!confirmation) {
            event.preventDefault();
        }
    }
});
