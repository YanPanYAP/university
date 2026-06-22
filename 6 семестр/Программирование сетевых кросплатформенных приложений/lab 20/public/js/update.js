document.addEventListener('DOMContentLoaded', () => {
    const nameInput = document.getElementById('editName');
    const deleteBtn = document.getElementById('deleteBtn');

    nameInput.addEventListener('input', () => {
        if (nameInput.value.length > 0) {
            deleteBtn.disabled = true;
        }
    });
});