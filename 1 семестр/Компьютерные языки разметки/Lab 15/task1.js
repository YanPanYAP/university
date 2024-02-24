let firstImg = document.getElementById("first");

firstImg.addEventListener("mouseover", firstFunction);

function firstFunction(){
    document.getElementById("fig").style.display = "block";
}

firstImg.addEventListener("mouseout", secondFunction);

function secondFunction(){
    document.getElementById("fig").style.display = "none";
}

let secondImg = document.getElementById("second");

secondImg.addEventListener("click", thirdFunction);

function thirdFunction(){
    document.getElementById("second").style.border = "10px solid red";
}

secondImg.addEventListener("dblclick", fourthFunction);

function fourthFunction(){
    document.getElementById("second").style.border = "none";
}

let thirdImg = document.getElementById("third");
let fourthImg = document.getElementById("fourth");

fourthImg.addEventListener("mouseover", fifthFunction);
fourthImg.addEventListener("mouseout", sixthFunction);

function fifthFunction(){
    document.getElementById("fourth").src = "./image-4.png";
    document.getElementById("third").src = "./image-5.png";
}

function sixthFunction(){
    document.getElementById("fourth").src = "./image-5.png";
    document.getElementById("third").src = "./image-4.png";
}
