document.addEventListener('DOMContentLoaded', () => {
    class Todolist {
        constructor() {
            this.tasks = [];
            this.filter = 'all';
            this.init();
        }

        init() {
            this.inputField = document.querySelector('.addTaskContainer input');
            this.addButton = document.querySelector('.addTaskContainer button');
            this.tasksContainer = document.querySelector('.tasks');
            this.filterButtons = document.querySelectorAll('.sortContainer button');

            this.addButton.addEventListener('click', () => this.addTask());
            this.filterButtons[0].addEventListener('click', () => this.setFilter('all'));
            this.filterButtons[1].addEventListener('click', () => this.setFilter('completed'));
            this.filterButtons[2].addEventListener('click', () => this.setFilter('incomplete'));

            this.filterButtons[0].classList.add('active');
        }

        addTask() {
            const taskText = this.inputField.value.trim();
            if (taskText === '') return alert('Введите задачу!');
            
            const newTask = {
                id: Date.now(),
                text: taskText,
                completed: false,
            };
            this.tasks.push(newTask);
            this.inputField.value = '';
            this.renderTasks();
        }

        deleteTask(id) {
            this.tasks = this.tasks.filter(task => task.id !== id);
            this.renderTasks();
        }

        toggleComplete(id) {
            const task = this.tasks.find(task => task.id === id);
            if (task) task.completed = !task.completed;
            this.renderTasks();
        }

        editTask(id) {
            const newTaskText = prompt('Введите новое название задачи:');
            if (newTaskText) {
                const task = this.tasks.find(task => task.id === id);
                if (task) task.text = newTaskText.trim();
                this.renderTasks();
            }
        }

        setFilter(filter) {
            this.filter = filter;
            this.renderTasks();
        
            this.filterButtons.forEach(button => {
                const buttonText = button.textContent.trim().toLowerCase();
        
                if (
                    (filter === 'all' && buttonText === 'все') ||
                    (filter === 'completed' && buttonText === 'выполненные') ||
                    (filter === 'incomplete' && buttonText === 'невыполненные')
                ) {
                    button.classList.add('active');
                } else {
                    button.classList.remove('active');
                }
            });
        }
        

        renderTasks() {
            this.tasksContainer.innerHTML = '';
            let filteredTasks = this.tasks;
        
            if (this.filter === 'completed') {
                filteredTasks = this.tasks.filter(task => task.completed);
            } else if (this.filter === 'incomplete') {
                filteredTasks = this.tasks.filter(task => !task.completed);
            }
        
            filteredTasks.forEach(task => {
                const taskDiv = document.createElement('div');
                taskDiv.className = 'task-item';
        
                const checkbox = document.createElement('input');
                checkbox.type = 'checkbox';
                checkbox.checked = task.completed;
                checkbox.addEventListener('change', () => this.toggleComplete(task.id));
        
                const taskText = document.createElement('span');
                taskText.textContent = task.text;
                taskText.style.textDecoration = task.completed ? 'line-through' : 'none';
        
                const deleteButton = document.createElement('button');
                deleteButton.textContent = 'Удалить';
                deleteButton.style.border = '1px solid red';
                deleteButton.addEventListener('click', () => this.deleteTask(task.id));
        
                const editButton = document.createElement('button');
                editButton.textContent = 'Редактировать';
                editButton.style.border = '1px solid blue';
                editButton.addEventListener('click', () => this.editTask(task.id));
        
                // Структура
                taskDiv.appendChild(checkbox);
                taskDiv.appendChild(taskText);
                taskDiv.appendChild(editButton);
                taskDiv.appendChild(deleteButton);
        
                this.tasksContainer.appendChild(taskDiv);
            });
        }        
    }

    new Todolist();
});