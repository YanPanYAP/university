var numSelected = null;
var tileSelected = null;

var errors = 0;

var board = [];
var solution = [];

window.onload = function() {
    generateNewGame();
    setGame();
}

document.getElementById("newGameBtn").addEventListener("click", function() {
    generateNewGame();
    updateBoard();
});

function generateNewGame() {
    board = generateBoard();
    solution = generateSolution(board);
    errors = 0;
    document.getElementById("errors").innerText = errors;
}

function generateBoard() {
    let board = Array(9).fill().map(() => Array(9).fill(0));
    
    if (solveSudoku(board)) {
        let numToRemove = Math.floor(Math.random() * 40) + 30;
        for (let i = 0; i < numToRemove; i++) {
            let row = Math.floor(Math.random() * 9);
            let col = Math.floor(Math.random() * 9);
            board[row][col] = 0;
        }
    }
    return board;
}

function generateSolution(board) {
    let solution = JSON.parse(JSON.stringify(board));
    solveSudoku(solution);
    return solution;
}

function solveSudoku(board) {
    for (let r = 0; r < 9; r++) {
        for (let c = 0; c < 9; c++) {
            if (board[r][c] === 0) {
                for (let num = 1; num <= 9; num++) {
                    if (isSafe(board, r, c, num)) {
                        board[r][c] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[r][c] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}


function isSafe(board, row, col, num) {
    for (let i = 0; i < 9; i++) {
        if (board[row][i] === num || board[i][col] === num) {
            return false;
        }
    }

    let startRow = row - row % 3;
    let startCol = col - col % 3;
    for (let r = startRow; r < startRow + 3; r++) {
        for (let c = startCol; c < startCol + 3; c++) {
            if (board[r][c] === num) {
                return false;
            }
        }
    }
    return true;
}

function updateBoard() {
    const boardContainer = document.getElementById("board");
    boardContainer.innerHTML = "";

    setGame();
}

function setGame() {
    const digitsContainer = document.getElementById("digits");
    digitsContainer.innerHTML = ""; 

    for (let i = 1; i <= 9; i++) {
        let number = document.createElement("div");
        number.id = i;
        number.innerText = i;
        number.addEventListener("click", selectNumber);
        number.classList.add("number");
        digitsContainer.appendChild(number);
    }

    const boardContainer = document.getElementById("board");
    for (let r = 0; r < 9; r++) {
        for (let c = 0; c < 9; c++) {
            let tile = document.createElement("div");
            tile.id = r.toString() + "-" + c.toString();

            // Если клетка имеет значение, отобразим его
            if (board[r][c] !== 0) {
                tile.innerText = board[r][c];
                tile.classList.add("tile-start");
            }

            if (r === 2 || r === 5) {
                tile.classList.add("horizontal-line");
            }
            if (c === 2 || c === 5) {
                tile.classList.add("vertical-line");
            }

            tile.addEventListener("click", selectTile);
            tile.classList.add("tile");
            boardContainer.appendChild(tile);
        }
    }
}

function selectNumber() {
    if (numSelected !== null) {
        numSelected.classList.remove("number-selected");
    }
    numSelected = this;
    numSelected.classList.add("number-selected");
}

function selectTile() {
    if (numSelected) {
        if (this.innerText !== "") {
            return;
        }

        let coords = this.id.split("-");
        let r = parseInt(coords[0]);
        let c = parseInt(coords[1]);

        if (solution[r][c] == numSelected.id) {
            this.innerText = numSelected.id;
            this.style.backgroundColor = "lightgreen";
        } else {
            errors += 1;
            document.getElementById("errors").innerText = errors;
            this.style.backgroundColor = "salmon";
        }

        setTimeout(() => {
            this.style.backgroundColor = "";
        }, 1000);
    }
}

document.getElementById("clearBoardBtn").addEventListener("click", function() {
    clearUserInput();
    updateBoard();
});

function clearUserInput() {
    const boardContainer = document.getElementById("board");
    const tiles = boardContainer.getElementsByClassName("tile");

    errors = 0;
    document.getElementById("errors").innerText = errors;

    for (let tile of tiles) {
        let coords = tile.id.split("-");
        let r = parseInt(coords[0]);
        let c = parseInt(coords[1]);

        if (board[r][c] === 0) {
            tile.innerText = "";
            tile.style.backgroundColor = "";
        }
    }
}

document.getElementById("showAnswer").addEventListener("click", function() {
    showAnswers();
});

function showAnswers() {
    const boardContainer = document.getElementById("board");
    const tiles = boardContainer.getElementsByClassName("tile");

    for (let tile of tiles) {
        let coords = tile.id.split("-");
        let r = parseInt(coords[0]);
        let c = parseInt(coords[1]);

        if (board[r][c] === 0) {
            tile.innerText = solution[r][c];
            tile.style.backgroundColor = "lightgreen";
        }
    }
}
