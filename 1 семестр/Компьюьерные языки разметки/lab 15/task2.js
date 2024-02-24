let form = document.querySelector("form");
let footer = document.querySelector("footer");


function fn(){
    footer.innerHTML = form.elements[0].value+ " " +form.elements[1].value+ " " +form.elements[2].value+ " " +form.elements[3].value+ " " +form.elements[4].value+ " " +form.elements[5].value;
}