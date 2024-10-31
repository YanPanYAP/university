//задание 2
function volume(length) {
    return function(width) {
        return function(height) {
        return length * width * height;
        };
    };
}

const fixed = volume(10);
console.log(fixed(5)(2));
console.log(fixed(4)(6));

// Задание 3
function* moveObject(startX = 0, startY = 0) {
    let x = startX;
    let y = startY;

    const commands = ['left', 'up' , 'right', 'up', 'down'];
    let i = 0;
    
    while (i < commands.length) {
        let command = commands[i];
        console.log(`Введена команда: ${command}`);
        
        switch (command) {
            case 'left':
                x -= 10;
                break;
            case 'right':
                x += 10;
                break;
            case 'up':
                y += 10;
                break;
            case 'down':
                y -= 10;
                break;
            default:
                console.log("Неверная команда");
                continue;
        }
        
        console.log(`Текущие координаты: (${x}, ${y})`);
        i++;  // Переход к следующей команде
    }
}

// Пример использования:
const move = moveObject();
move.next();  // Начало движения